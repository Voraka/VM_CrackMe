// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "MainDlg.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <cassert>
#include <boost/crc.hpp>
#include "SelfCheck.h"
#include "NineTowers.h"
#include "ufmod.h"
#include "rc6.h"

#ifdef __SMC__
	#pragma comment(linker, "/SECTION:.text,RWE")
	// 此处去掉对齐方式，改为默认的0x1000对齐，以兼容win7 64以上系统
	// #pragma comment (linker,"/ALIGN:512")
	// 合并段
	#pragma comment(linker, "/MERGE:.rdata=.text")
	//#pragma comment(linker, "/MERGE:.data=.text")
	#pragma comment(linker, "/MERGE:.tls=.text")
	#pragma comment(linker, "/MERGE:.gfids=.text")
#endif

#ifdef __SMC__
	/**********************************************/
	// 此处直接赋值为零即可，需要编译完成后进行后续处理
	// 此处要特别注意，若分配的空间不够，会导致最后崩溃！
	/**********************************************/
	BYTE byteMoveThere[0x1200]{ 0xcc };
	BYTE Code[0x500]{ 0xcc };
#endif

// 自定义CRC32             
typedef boost::crc_optimal<32, 0x04C27EB9, 0xFFFFFFFF, 0xFFFFFFFF, true, true>crc_32_self;

CMainDlg::CMainDlg()
{ 
	Initialize();
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
		if (pMsg->wParam == VK_RETURN)
			if (pMsg->hwnd == GetDlgItem(IDC_EDIT1).m_hWnd)
			{
				PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON1, BN_CLICKED));
				return TRUE;
			}
	return CWindow::IsDialogMessage(pMsg);
}

// 将信号与插槽的断开放在OnIdle里面是个非常隐蔽的地方！！！
BOOL CMainDlg::OnIdle()
{

#ifdef __SMC__
	byte Key[] = {0xA3, 0x2D, 0x5C, 0x9D, 0x4E, 0xF7, 0x8B, 0xCF};
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	DecTarget2(dwStart, dwEnd - dwStart, Key, 8);
__lbEncS:
#endif
	UIUpdateChildWindows();
#ifdef __SMC__
	if (IsHardbreakDetected() || IsSoftbreakDetected())
	{
		if (1 < m_sig.num_slots())
		{
			m_sig.disconnect_all_slots();
			m_sig.connect(std::bind(&CMainDlg::Leave, this));
			goto __lbEncE;
		}
	}

__lbEncE:
	EncTarget2(dwStart, dwEnd - dwStart, Key, 8);
#endif
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
#ifdef __SMC__	
	// set icons
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	CSelfCheck sc;
	//sc.DecryptAndFillTarget(dwStart, dwEnd - dwStart);
	DWORD dwCrc32 = sc.ComputeFileCodeCRC();

	//EncTarget((DWORD)Code, dwLen, (PBYTE)&dwCrc32, sizeof(dwCrc32));
	DWORD dwSize = dwEnd - dwStart;
	DecTarget1((DWORD)Code, dwSize, (PBYTE)&dwCrc32, sizeof(dwCrc32));
	for (DWORD dw = 0; dw != dwSize; dw++)
		((PBYTE)dwStart)[dw] = Code[dw];

__lbEncS:
#endif
	CEdit edt = GetDlgItem(IDC_EDIT1);
	USHORT uMsg[] = { 0x9AD2,  0x6E01,  0x7436,  0xB5AD,  0x6A3C,  
					  0xA264,  0xA3A7,  0x04F8,  0x7166,  0x9E5F,  
					  0xA261,  0x82A6,  0x04F5,  0xA589,  0xA26D,  
					  0xB58D,  0x04FC, 0 };
	for (int i = 0; i < 17; i++)
		uMsg[i] ^= (0xFBED + i);
	edt.SetCueBannerText(LPCTSTR(uMsg), FALSE);

	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);
#ifdef __SMC__
	// 先解码，再运行！
	m_sig.connect(std::bind(&CMainDlg::DecStartGame, this));
#endif
	m_sig.connect(std::bind(&CMainDlg::StartGame, this));
#ifdef __SMC__
	m_sig.connect(std::bind(&CMainDlg::EncStartGame, this));
#endif
	CenterWindow();
	FillPath();
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	m_edit = GetDlgItem(IDC_EDIT1);
	pLoop->AddMessageFilter(this);
#ifdef __SMC__
	// 设定每200毫秒进行一次硬软断点检测
	m_pSetTimer(m_hWnd, 50, 200, NULL);
#endif
	pLoop->AddIdleHandler(this);
	UIAddChildWindowContainer(m_hWnd);
	GetDlgItem(IDC_BUTTON1).SetFocus();
#ifdef __SMC__
__lbEncE:
	// 执行完成以后抽空此处代码
	for (DWORD dw = 0; dw != dwSize; dw++)
		((PBYTE)dwStart)[dw] = 0xCC;
#endif
	return FALSE;
}

void CMainDlg::FillPath()
{
	// 随机运动轨迹用于加密迷宫，九重妖塔数据
	 m_lstPath.push_back({ CPoint(8215,-6537), CPoint(8282, -6194) });
	 m_lstPath.push_back({ CPoint(8282,-6194), CPoint(8349, -5851) });
	 m_lstPath.push_back({ CPoint(8349,-5851), CPoint(8390, -6178) });
	 m_lstPath.push_back({ CPoint(8390,-6178), CPoint(8765, -6741) });
	 m_lstPath.push_back({ CPoint(8765,-6741), CPoint(9140, -7304) });
	 m_lstPath.push_back({ CPoint(9140,-7304), CPoint(9707, -6999) });
	 m_lstPath.push_back({ CPoint(9707,-6999), CPoint(9416, -6944) });
	 m_lstPath.push_back({ CPoint(9416,-6944), CPoint(9105, -7270) });
	 m_lstPath.push_back({ CPoint(9105,-7270), CPoint(9480, -7833) });
	 m_lstPath.push_back({ CPoint(9480,-7833), CPoint(9185, -7718) });
	 m_lstPath.push_back({ CPoint(9185,-7718), CPoint(8890, -7603) });
	 m_lstPath.push_back({ CPoint(8890,-7603), CPoint(8957, -7260) });
	 m_lstPath.push_back({ CPoint(8957,-7260), CPoint(9524, -6955) });
	 m_lstPath.push_back({ CPoint(9524,-6955), CPoint(9229, -6840) });
	 m_lstPath.push_back({ CPoint(9229,-6840), CPoint(9270, -7167) });
	 m_lstPath.push_back({ CPoint(9270,-7167), CPoint(9837, -6862) });
	 m_lstPath.push_back({ CPoint(9837,-6862), CPoint(10212, -7425) });
	 m_lstPath.push_back({ CPoint(10212,-7425), CPoint(9921, -7370) });
	 m_lstPath.push_back({ CPoint(9921,-7370), CPoint(9630, -7315) });
	 m_lstPath.push_back({ CPoint(9630,-7315), CPoint(9925, -7364) });
	 m_lstPath.push_back({ CPoint(9925,-7364), CPoint(10220, -7413) });
	 m_lstPath.push_back({ CPoint(10220,-7413), CPoint(9929, -7358) });
	 m_lstPath.push_back({ CPoint(9929,-7358), CPoint(9634, -7243) });
	 m_lstPath.push_back({ CPoint(9634,-7243), CPoint(9675, -7570) });
	 m_lstPath.push_back({ CPoint(9675,-7570), CPoint(9384, -7515) });
	 m_lstPath.push_back({ CPoint(9384,-7515), CPoint(9451, -7172) });
	 m_lstPath.push_back({ CPoint(9451,-7172), CPoint(9140, -7498) });
	 m_lstPath.push_back({ CPoint(9140,-7498), CPoint(8829, -7824) });
	 m_lstPath.push_back({ CPoint(8829,-7824), CPoint(9396, -7519) });
	 m_lstPath.push_back({ CPoint(9396,-7519), CPoint(9963, -7214) });
	 m_lstPath.push_back({ CPoint(9963,-7214), CPoint(9652, -7540) });
	 m_lstPath.push_back({ CPoint(9652,-7540), CPoint(9947, -7589) });
	 m_lstPath.push_back({ CPoint(9947,-7589), CPoint(9988, -7916) });
	 m_lstPath.push_back({ CPoint(9988,-7916), CPoint(9693, -7801) });
	 m_lstPath.push_back({ CPoint(9693,-7801), CPoint(9988, -7850) });
	 m_lstPath.push_back({ CPoint(9988,-7850), CPoint(10363, -8413) });
	 m_lstPath.push_back({ CPoint(10363,-8413), CPoint(10930, -8108) });
	 m_lstPath.push_back({ CPoint(10930,-8108), CPoint(10619, -8434) });
	 m_lstPath.push_back({ CPoint(10619,-8434), CPoint(10324, -8319) });
	 m_lstPath.push_back({ CPoint(10324,-8319), CPoint(10365, -8646) });
	 m_lstPath.push_back({ CPoint(10365,-8646), CPoint(10740, -9209) });
	 m_lstPath.push_back({ CPoint(10740,-9209), CPoint(10429, -9535) });
	 m_lstPath.push_back({ CPoint(10429,-9535), CPoint(10470, -9862) });
	 m_lstPath.push_back({ CPoint(10470,-9862), CPoint(10511, -10189) });
	 m_lstPath.push_back({ CPoint(10511,-10189), CPoint(10806, -10238) });
	 m_lstPath.push_back({ CPoint(10806,-10238), CPoint(10873, -9895) });
	 m_lstPath.push_back({ CPoint(10873,-9895), CPoint(11168, -9944) });
	 m_lstPath.push_back({ CPoint(11168,-9944), CPoint(11543, -10507) });
	 m_lstPath.push_back({ CPoint(11543,-10507), CPoint(12110, -10202) });
	 m_lstPath.push_back({ CPoint(12110,-10202), CPoint(11819, -10147) });
	 m_lstPath.push_back({ CPoint(11819,-10147), CPoint(11528, -10092) });
	 m_lstPath.push_back({ CPoint(11528,-10092), CPoint(12095, -9787) });
	 m_lstPath.push_back({ CPoint(12095,-9787), CPoint(11784, -10113) });
	 m_lstPath.push_back({ CPoint(11784,-10113), CPoint(11493, -10058) });
	 m_lstPath.push_back({ CPoint(11493,-10058), CPoint(11560, -9715) });
	 m_lstPath.push_back({ CPoint(11560,-9715), CPoint(11601, -10042) });
	 m_lstPath.push_back({ CPoint(11601,-10042), CPoint(11976, -10605) });
	 m_lstPath.push_back({ CPoint(11976,-10605), CPoint(12043, -10262) });
	 m_lstPath.push_back({ CPoint(12043,-10262), CPoint(12110, -9919) });
	 m_lstPath.push_back({ CPoint(12110,-9919), CPoint(11799, -10245) });
	 m_lstPath.push_back({ CPoint(11799,-10245), CPoint(12094, -10294) });
	 m_lstPath.push_back({ CPoint(12094,-10294), CPoint(11799, -10179) });
	 m_lstPath.push_back({ CPoint(11799,-10179), CPoint(12094, -10228) });
	 m_lstPath.push_back({ CPoint(12094,-10228), CPoint(11799, -10113) });


	// 按照指定的注册码，生成的运行轨迹
	  m_lstMidPath.push_back({ CPoint(8215,-6537), CPoint(8590, -7100) });
	  m_lstMidPath.push_back({ CPoint(8590,-7100), CPoint(9157, -6795) });
	  m_lstMidPath.push_back({ CPoint(9157,-6795), CPoint(8846, -7121) });
	  m_lstMidPath.push_back({ CPoint(8846,-7121), CPoint(9413, -6816) });
	  m_lstMidPath.push_back({ CPoint(9413,-6816), CPoint(9454, -7143) });
	  m_lstMidPath.push_back({ CPoint(9454,-7143), CPoint(9749, -7192) });
	  m_lstMidPath.push_back({ CPoint(9749,-7192), CPoint(9454, -7077) });
	  m_lstMidPath.push_back({ CPoint(9454,-7077), CPoint(9749, -7126) });
	  m_lstMidPath.push_back({ CPoint(9749,-7126), CPoint(10316, -6821) });
	  m_lstMidPath.push_back({ CPoint(10316,-6821), CPoint(10611, -6870) });
	  m_lstMidPath.push_back({ CPoint(10611,-6870), CPoint(10678, -6527) });
	  m_lstMidPath.push_back({ CPoint(10678,-6527), CPoint(10745, -6184) });
	  m_lstMidPath.push_back({ CPoint(10745,-6184), CPoint(10434, -6510) });
	  m_lstMidPath.push_back({ CPoint(10434,-6510), CPoint(10123, -6836) });
	  m_lstMidPath.push_back({ CPoint(10123,-6836), CPoint(10690, -6531) });
	  m_lstMidPath.push_back({ CPoint(10690,-6531), CPoint(10379, -6857) });
	  m_lstMidPath.push_back({ CPoint(10379,-6857), CPoint(10420, -7184) });
	  m_lstMidPath.push_back({ CPoint(10420,-7184), CPoint(10129, -7129) });
	  m_lstMidPath.push_back({ CPoint(10129,-7129), CPoint(10504, -7692) });
	  m_lstMidPath.push_back({ CPoint(10504,-7692), CPoint(11071, -7387) });
	  m_lstMidPath.push_back({ CPoint(11071,-7387), CPoint(11112, -7714) });
	  m_lstMidPath.push_back({ CPoint(11112,-7714), CPoint(11679, -7409) });
	  m_lstMidPath.push_back({ CPoint(11679,-7409), CPoint(11720, -7736) });
	  m_lstMidPath.push_back({ CPoint(11720,-7736), CPoint(11429, -7681) });
	  m_lstMidPath.push_back({ CPoint(11429,-7681), CPoint(11118, -8007) });
	  m_lstMidPath.push_back({ CPoint(11118,-8007), CPoint(11413, -8056) });
	  m_lstMidPath.push_back({ CPoint(11413,-8056), CPoint(11980, -7751) });
	  m_lstMidPath.push_back({ CPoint(11980,-7751), CPoint(12275, -7800) });
	  m_lstMidPath.push_back({ CPoint(12275,-7800), CPoint(11964, -8126) });
	  m_lstMidPath.push_back({ CPoint(11964,-8126), CPoint(12031, -7783) });
	  m_lstMidPath.push_back({ CPoint(12031,-7783), CPoint(12098, -7440) });
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

void CMainDlg::StartGame()
{
#ifdef __SMC__
	byte Key[] = { 'W', 'e', 'L', 'c', '0','m', 'E', '!' };
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	DecTarget2(dwStart, dwEnd - dwStart, Key, 8);
__lbEncS:
#endif
	// 原始输入key
	// r9cOlg1ewH3S08XYu5l1O0W7xCg4Np
	TCHAR szBuf[101] = { 0 };
	std::string sIn;
	std::string sIn_x;
	// 使用自已加载的GetWindowTextW函数来获取字串
	int nLength = m_pGetWindowText(m_edit.m_hWnd, szBuf, 100);
	if (!nLength)
#ifdef __SMC__
		goto __lbEncE;
#else
		return;
#endif

	USES_CONVERSION;
	sIn = T2A(szBuf);

	//sIn = "r9cOlg1ewH3S08X";
	nLength >>= 1;
	if (! nLength)
#ifdef __SMC__
		goto __lbEncE;
#else
		return;
#endif
	// 取左边一半的字串用于验证
	m_strInputL = sIn.substr(0, nLength);	
	// 保留右边的一半字串
	m_strInputR = sIn.substr(nLength);
	sIn = m_strInputL;

	char b64[50] = { 0 };
	size_t sz64 = base64Encode(b64, sIn.c_str(), sIn.length());
	char b32[50] = { 0 };
	base32Encode(b32, b64, sz64);
	sIn_x = b32;
	int len = sIn_x.length();

	// 首尾环状异或
	for (int i = 1; i < len; i++)
		sIn_x[i] ^= sIn_x[i - 1];
	sIn_x[0] ^= sIn_x[len - 1];    // 第一元素等于尾元素与之异或

	// 此处生成长度的hash值
	// ATLTRACE("0x%X", std::hash<size_t>()(len));
	if (std::hash<size_t>()(len) == 0x511DD735)
	{
		//根据random_shuffle 第一次打乱索引取值,这里搞个100个元素，实际只用到32个！
		byte x[100] = { 
			0x0C,  0x01,  0x09,  0x18,  0x00,  0x1B,  0x19,  0x1F,  0x1D,  0x0F, 
			0x12,  0x1C,  0x0E,  0x0D,  0x17,  0x10,  0x1A,  0x03,  0x14,  0x11,
			0x08,  0x13,  0x16,  0x0A,  0x1E,  0x07,  0x06,  0x0B,  0x05,  0x04,
			0x02,  0x15,  0x20,  0x33,  0x57,  0x3B,  0x2F,  0x2B,  0x2D,  0x42,
			0x5B,  0x48,  0x3F,  0x5A,  0x35,  0x61,  0x5E,  0x41,  0x3A,  0x28, 
			0x49,  0x22,  0x30,  0x31,  0x38,  0x62,  0x4B,  0x58,  0x4D,  0x37, 
			0x40,  0x5C,  0x34,  0x2E,  0x23,  0x4E,  0x4A,  0x3D,  0x2A,  0x39,
			0x47,  0x63,  0x54,  0x51,  0x4C,  0x27,  0x25,  0x3C,  0x53,  0x26, 
			0x5D,  0x52,  0x29,  0x50,  0x43,  0x24,  0x3E,  0x21,  0x36,  0x5F, 
			0x44,  0x59,  0x60,  0x2C,  0x4F,  0x32,  0x56,  0x46,  0x55,  0x45, 
		};


		// 此处用于生成随机打乱字串数组的索引
		//for (int i = 0; i < 100; i++)
		//	x[i] = i;
		//random_shuffle(x, x + 32);
		//random_shuffle(x + 33, x + 100);
		//for (int i = 0; i < 100; i++)
		//ATLTRACE("0x%02X, ", x[i]);
		sIn.resize(len);
		for (int i = 0; i<len; i++)
			sIn[x[i]] = sIn_x[i];
	}
	
	// 此处加上长度判断，主要是防止字串下标越界
	if (len > (int)(sIn.length()))
#ifdef __SMC__
		goto __lbEncE;
#else
		return;
#endif
	// 进行尾首环状异或
	for (int i = len - 1; i > 0; i--)
		sIn[i - 1] ^= sIn[i];
	sIn[len - 1] ^= sIn[0];  // 最后一元素等于首元素与之异或
	{
		size_t sz16 = base16EncodeGetLength(len);
		std::shared_ptr<char> pOut16(new char[sz16], std::default_delete<char[]>());
		base16Encode(&*pOut16, sIn.c_str(), len);
		void (CMainDlg::*pFnMoveThere)(shared_ptr<char>) = &CMainDlg::MoveThere;
		
		// MoveThere的函数大小为 0xD29
#ifdef __SMC__
		DecTarget1(*(PDWORD)&pFnMoveThere, SZ_MoveThere, Key, 8);
#endif
		MoveThere(pOut16);
#ifdef __SMC__
		EncTarget1(*(PDWORD)&pFnMoveThere, SZ_MoveThere, Key, 8);
#endif
	}
#ifdef __SMC__
__lbEncE:
	EncTarget2(dwStart, dwEnd - dwStart, Key, 8);
#endif
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

void CMainDlg::Move(CPoint& curPos, int iDirect)
{
	Path pth;
	switch (iDirect)
	{
	case 0: //左
		pth.pt_fr = curPos;
		curPos.Offset(-0x127, 0x73);
		pth.pt_to = curPos;
		m_userPath.push_back(pth);
		break;
	case 1: //左上
		pth.pt_fr = curPos;
		curPos.Offset(-0x123, 0x37);
		pth.pt_to = curPos;
		m_userPath.push_back(pth);
		break;
	case 2: //上
		pth.pt_fr = curPos;
		curPos.Offset(0x43, 0x157);
		pth.pt_to = curPos;
		m_userPath.push_back(pth);
		break;
	case 3://右上
		pth.pt_fr = curPos;
		curPos.Offset(0x237, 0x131);
		pth.pt_to = curPos;
		m_userPath.push_back(pth);
		break;
	case 4://右
		pth.pt_fr = curPos;
		curPos.Offset(0x127, -0x31);
		pth.pt_to = curPos;
		m_userPath.push_back(pth);
		break;
	case 5://右下
		pth.pt_fr = curPos;
		curPos.Offset(0x177, -0x233);
		pth.pt_to = curPos;
		m_userPath.push_back(pth);
		break;
	case 6://下
		pth.pt_fr = curPos;
		curPos.Offset(0x29, -0x147);
		pth.pt_to = curPos;
		m_userPath.push_back(pth);
		break;
	case 7://左下
		pth.pt_fr = curPos;
		curPos.Offset(-0x137, -0x146);
		pth.pt_to = curPos;
		m_userPath.push_back(pth);
		break;
	}

}

void CMainDlg::MoveThere(std::shared_ptr<char> ptr)
{
__lbStart:
	CPoint ptStart(0x2017, -0x1989);
	CPoint ptCur;
	ptCur = ptStart;
	//342A0C31163EB4CBC80D36327BBCBE14C8C81B060572712E33C0BFC1CFC3B33C
	std::hash<char> myHash;

	//// 生成16个方向的hash值
	//for (auto i : { '0', '3', '2', '7', '4', 'A', '5', 'B', '6', 'C', '1', 'F', '8', 'E', '9', 'D' })
	//	ATLTRACE("0x%X, ", myHash(i));

	//16个字母对应的方向号的hash值（此处直接用生成的hash值，不调用函数）
	const size_t dir_code[] = 
	{
		 0x350CA8AF, 0x360CAA42, 0x370CABD5, 0x320CA3F6, 
		 0x310CA263, 0xC40BF6CC, 0x300CA0D0, 0xC70BFB85, 
		 0x330CA589, 0xC60BF9F2,  0x340CA71C, 0xC30BF539, 
		 0x3D0CB547, 0xC00BF080, 0x3C0CB3B4, 0xC10BF213
	};

	std::string str = &*ptr;
	str = str.substr(13, 31);
	// 此处结合下面的注释部分用于生成随机的运动加密轨迹
	//std::string str = "1234567890ABCDEF1234567890ABCDEF1234567890ABCDEF1234567890ABCDEF";
	//std::random_shuffle(str.begin(), str.end());
	//std::random_shuffle(str.begin(), str.end());
	//std::random_shuffle(str.begin(), str.end());
	m_userPath.clear();

	//右 	myHash('0'), myHash('3'),
	//上 	myHash('2'), myHash('7'),
	//右下	myHash('4'), myHash('A'),
	//左下	myHash('5'), myHash('B'),
	//右上	myHash('6'), myHash('C'),
	//左上	myHash('1'), myHash('F'),
	//下 	myHash('8'), myHash('E'),
	//左		myHash('9'), myHash('D')
	int nLen = (int)str.length();
	for (int i = 0; i < nLen; i++)
	{
		size_t szHash = myHash(str[i]);
		if (szHash == dir_code[0] || szHash == dir_code[1])
			Move(ptCur, 4);
		else if (szHash == dir_code[2] || szHash == dir_code[3])
			Move(ptCur, 2);
		else if (szHash == dir_code[4] || szHash == dir_code[5])
			Move(ptCur, 5);
		else if (szHash == dir_code[6] || szHash == dir_code[7])
			Move(ptCur, 7);
		else if (szHash == dir_code[8] || szHash == dir_code[9])
			Move(ptCur, 3);
		else if (szHash == dir_code[10] || szHash == dir_code[11])
			Move(ptCur, 1);
		else if (szHash == dir_code[12] || szHash == dir_code[13])
			Move(ptCur, 6);
		else if (szHash == dir_code[14] || szHash == dir_code[15])
			Move(ptCur, 0);
	}

	//////// 此处用于生成初始化的64位链表
	//auto itRB = begin(m_userPath);
	//auto itRE = end(m_userPath);
	//for (; itRB != itRE; itRB++)
	//	ATLTRACE("m_lstMidPath.push_back({CPoint(%d,%d), CPoint(%d, %d)});\r\n", 
	//		itRB->pt_fr.x, itRB->pt_fr.y, itRB->pt_to.x, itRB->pt_to.y);
	
	// 比较用户的64位链表与给定的链表是否等长，且每位相同
	if (m_lstMidPath.size() == m_userPath.size())
	{
		auto itLB = begin(m_lstMidPath);
		auto itLE = end(m_lstMidPath);
		auto itRB = begin(m_userPath);
		auto itRE = end(m_userPath);

		// 逐个比较68个点的运行轨迹是否一致。
		for (; itLB != itLE; itLB++) {
			if ((*itLB) == (*itRB))
				itRB++;
			else
				return;
		}
	__lbEnd:
#ifdef __SMC__
		DWORD dwStart, dwEnd;
		__asm 
		{
			mov dwStart, offset __lbStart
			mov dwEnd, offset __lbEnd
		}
		crc_32_self crc32;
		crc32.process_bytes((void*)dwStart, dwEnd - dwStart+1);
		DWORD dwCRC = crc32.checksum();
		__asm
		{
			mov dwStart, offset __lbEncS
			mov dwEnd, offset __lbEncE
		}
		DecTarget3(dwStart, dwEnd - dwStart, PBYTE(&dwCRC), sizeof(dwCRC));
#endif
		// 以下代码块需要先将上面函数改为EncTarget，用16进制编辑器复制出对应的机器码
		// 再修改为DecTarget，重新编译，然后将复制的机器码粘贴到相应地址才能成功。
	__lbEncS:
		// 以随机轨迹的终点作为迷宫移动的起点
		auto itmRB = m_lstPath.rbegin();
		m_mz.Reset(itmRB->pt_to);
		//9C371D9613BB21330A24
		str = str.substr(0, 13);

#ifdef __SMC__
		// 取CMaze::LetsGo函数指针
		bool (CMaze::*pfnLetsGo)(std::string, list<Path>&) = &CMaze::LetsGo;
		byte key_mg[] = { 0x20, 0x17, 0x18, 0x99 };
		// 此处同样需要手工先将修改后的机器码复制保存，然后改为DecTarget，再粘贴回原处。
		DecLetsGoWithCrcOfOID();
		// LetsGo的大小为 0x353
		DecTarget2(*(DWORD*)&pfnLetsGo, SZ_LetsGo, key_mg, 4);
#endif
		bool bMGOut = m_mz.LetsGo(str, m_lstPath);
#ifdef __SMC__
		EncTarget2(*(DWORD*)&pfnLetsGo, SZ_LetsGo, key_mg, 4);
		EncLetsGoWithCrcOfOID();
#endif
		if (bMGOut)
		{
			str = &*ptr;
			// str = 01B79C0ECBC3729DBB723ABC33B3C0BC74CD32363A723EC87F0733C6B82038731C3C
			bool (CMatrixMul::*pfnMMulEqu)(std::string) = &CMatrixMul::MMulEqual;
			byte key_mmul[] = { 'K', 'x', 'B', 'w', 'N', 's', };
#ifdef __SMC__		
			DecMMulEqualWithCrcOfInit();

			// MMulEqual的大小为 0x109A
			DecTarget3(*(DWORD*)&pfnMMulEqu, SZ_MMulEqual, key_mmul, 6);
#endif
			bool bMMul = m_mm.MMulEqual(str.substr(44));
#ifdef __SMC__
			EncTarget3(*(DWORD*)&pfnMMulEqu, SZ_MMulEqual, key_mmul, 6);
			EncMMulEqualWithCrcOfInit();
#endif
			if (bMMul)
			{
				// STL hash函数
				// ATLTRACE("stl::hash: 0x%X, crc32: 0x%X", hsh(m_strInput), crc32.checksum());		
				//if (0xF933063C == hsh(m_strInputL) && 0x5490B744 == crc32.checksum())
				{
					shared_ptr<CNineTowers> pNT = make_shared<CNineTowers>(m_lstPath);
#ifdef __SMC__
					bool (CNineTowers::*pfnPlay)(string, size_t, size_t, size_t) = &CNineTowers::Play;
					
					char key_rc6[25] = { 0 };

					/**************************/
					//此处的SHA1进行了常数修改：
					//// 修改后的SHA1各常数项
					//h_[0] = 0xB3016745;
					//h_[1] = 0xEFAD98BA;
					//h_[2] = 0xAB8ED8BA;
					//h_[3] = 0x103BDCFE;
					//h_[4] = 0xE1F0C3D7;
					//************************/
					boost::uuids::detail::sha1 SHA1;
					SHA1.process_bytes(m_strInputL.c_str(), m_strInputL.length());
					SHA1.get_digest(boost::uuids::detail::sha1::digest_type(key_rc6));
									
					hash<string> hsh;
					crc32.reset();
					crc32.process_bytes(m_strInputL.c_str(), m_strInputL.length());
					byte key_play[8] = { 0 };
					*(PDWORD)key_play = hsh(m_strInputL);
					DWORD dwCRC32 = crc32.checksum();
					*(PDWORD)&key_play[4] = dwCRC32;
					*(PDWORD)&key_rc6[20] = dwCRC32;
					
					RC6::rc6_key_setup((PBYTE)key_rc6, 24);
					int nBlocks = SZ_Play >> 4;
					for (int i = 0; i < nBlocks; i++)
						RC6::rc6_block_decrypt((unsigned int*)(*(PDWORD)&pfnPlay + (i << 4)), (unsigned int*)(*(PDWORD)&pfnPlay + (i << 4)));
					DecTarget1(*(DWORD*)&pfnPlay, SZ_Play, key_play, 8);
#endif

					bool bRet = pNT->Play(m_strInputR, SZ_InitGame, SZ_Walk, SZ_IsGetThrough);

#ifdef __SMC__
					EncTarget1(*(DWORD*)&pfnPlay, SZ_Play, key_play, 8);
					for (int i = 0; i < nBlocks; i++)
						RC6::rc6_block_encrypt((unsigned int*)(*(PDWORD)&pfnPlay + (i << 4)), (unsigned int*)(*(PDWORD)&pfnPlay + (i << 4)));

#endif
					if (bRet)
					{
						// 停止播放先前的背景音乐
						//uFMOD_StopSong();
						// 播放新的背景音乐庆祝！
						uFMOD_PlaySong((LPVOID)xm2, sizeof(xm2), XM_MEMORY);
						// 妖塔归来，九死一生。大神法力通玄，实至名归！\0
						TCHAR szMsg[] =
						{
							0x32fd, 0x333f, 0x3439, 0xc0e,  0x9467, 0x2536, 0x0010, 0x256b, 0x1e74, 0x5b69,
							0x324c, 0x1235, 0x07be, 0x39f0, 0xfb71, 0x18ef, 0x9467, 0x30f5, 0xea98, 0x3f66,
							0x3439, 0x946a, 0x6b6b
						};
						for (int i = 0; i < 23; i++)
							szMsg[i] ^= 0x6b6b;

						// 妖神归寂，人界安宁\0
						TCHAR szTitle[] =
						{
							0xf936, 0xd9fe, 0xfff2, 0xfb62, 
							0x5fac, 0xee1a, 0xd5ec, 0xfb29, 
							0xfb21, 0xa0a0
						};

						for (int i = 0; i < 10; i++)
							szTitle[i] ^= 0xa0a0;

						m_pMessageBox(m_hWnd, szMsg, szTitle, MB_ICONINFORMATION);
						//MessageBox(szMsg, szTitle, MB_ICONINFORMATION);
					}
				}
			}	
		}
#ifdef __SMC__
	__lbEncE:
		EncTarget3(dwStart, dwEnd - dwStart, PBYTE(&dwCRC), sizeof(dwCRC));
#endif
	}
}

LRESULT CMainDlg::OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	
	// 调用信号函数
	m_sig();
	return 0;
}

LRESULT CMainDlg::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
#ifdef __SMC__
	if (IsHardbreakDetected() || IsSoftbreakDetected())
	{
		KillTimer(50);
		// 当发现被调戏时，更换信号的插槽。这样按下Button时调用的函数自然会发生改变。
		if (1 < m_sig.num_slots())
		{
			m_sig.disconnect_all_slots();
			m_sig.connect(bind(&CMainDlg::Leave, this));
		}
	}
#endif
	return 0;
}
	

// 当发现被调试后，程序退出
void CMainDlg::Leave()
{
	// 这一段密文使用Python进行处理，比较方便，生成后
	// 复制出来，再替换掉 ' 即可。
	/*
	str = "Warning";
	e = []
	for i in range(len(str)):
	e.append(ord(str[i]) ^ 0x88)
	x =[]
	for i in range(len(e)):
	x.append("0x%02X" %e[i])

	print x
	*/

	// 加密串： 陷入弑仙裂神阵，您神形俱灭!
	WORD szMsg[] = 
	{
		0xd2ff, 0x15ed, 0x1b99, 0xa51, 0xcc4a, 0x3dd6, 0xd2bd, 
		0xbb84, 0x2420, 0x3dd6, 0x1bea, 0xb79, 0x34e5, 0xbb89, 0x4488
	};

	for (int i = 0; i < 15; i++)
		szMsg[i] ^= 0x4488;

	// 加密串: 殒仙榜
	WORD szTitle[] = { 0xe3d6, 0xc69d, 0xe1d8, 0x8844};
	for (int i = 0; i < 4; i++)
		szTitle[i] ^= 0x8844;
	MessageBoxTimeout(m_hWnd, (LPCWSTR)szMsg, (LPCWSTR)szTitle, MB_ICONWARNING, GetUserDefaultLangID(), 3000);

	// 结束当前主线程，也即是退出。
	PostMessage(WM_CLOSE);
}


// 解码以Timer代码的crc32为密钥加密的StartGame代码
void CMainDlg::DecStartGame()
{
	// 以OnTimer中的代码CRC32值作为密钥，解密StartGame函数
	BYTE Key[] = {0xD1, 0xE3, 0xC8, 0xAE};
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	DecTarget3(dwStart, dwEnd - dwStart, Key, 4);
__lbEncS:
	LRESULT(CMainDlg::*pFnOnTimer)(UINT, WPARAM, LPARAM, BOOL&) = &CMainDlg::OnTimer;
	void (CMainDlg::*pFnStartGame)() = &CMainDlg::StartGame;

	crc_32_self crc32;
	// OnTimer的长度为0x19A
	crc32.process_bytes((const void*)(PBYTE)*(PDWORD)&pFnOnTimer, SZ_OnTimer);
	DWORD dwCrc32 = crc32.checksum();

	// StartGame长度为0x1282
	DecTarget1(*(PDWORD)&pFnStartGame, SZ_StartGame, (PBYTE)&dwCrc32, sizeof(dwCrc32));
__lbEncE:
	EncTarget3(dwStart, dwEnd - dwStart, Key, 4);
}


// 使用后再次加密StartGame
void CMainDlg::EncStartGame()
{
	BYTE Key[] = { 0xCC, 0xAF, 0xD4, 0xE8 };
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	DecTarget3(dwStart, dwEnd - dwStart, Key, 4);
__lbEncS:
	// 以OnTimer中的代码CRC32值作为密钥，解密StartGame函数
	LRESULT(CMainDlg::*pFnOnTimer)(UINT, WPARAM, LPARAM, BOOL&) = &CMainDlg::OnTimer;
	void (CMainDlg::*pFnStartGame)() = &CMainDlg::StartGame;

	// 校验长度为0x19A
	crc_32_self crc32;
	crc32.process_bytes((const void*)(PBYTE)*(PDWORD)&pFnOnTimer, SZ_OnTimer);
	DWORD dwCrc32 = crc32.checksum();

	// StartGame长度为0x1282
	EncTarget1(*(PDWORD)&pFnStartGame, SZ_StartGame, (PBYTE)&dwCrc32, sizeof(dwCrc32));
__lbEncE:
	EncTarget3(dwStart, dwEnd - dwStart, Key, 4);
}

// 用OnitialDlg中的crc值解码CMaze::LetsGo
void CMainDlg::DecLetsGoWithCrcOfOID()
{
	byte Key[] = { 0x1F, 0x2E, 0x3D, 0x4C, 0x5B, 0x6A };
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	DecTarget2(dwStart, dwEnd - dwStart, Key, 6);
__lbEncS:
	
	bool (CMaze::*pFnLetsGo)(std::string, list<Path>&) = &CMaze::LetsGo;
	LRESULT (CMainDlg::*pFnOnInitDialog)(UINT, WPARAM, LPARAM, BOOL&) = &CMainDlg::OnInitDialog;
	crc_32_self crc32;
	// OnInitialize的大小
	crc32.process_bytes((void *)*(PDWORD)&pFnOnInitDialog, SZ_OnInitDialog);
	DWORD dwCrc32 = crc32.checksum();
	// 用OnInitialDlg处的CRC值解码LetsGo 大小为 0x483
	DecTarget3(*(PDWORD)&pFnLetsGo, SZ_LetsGo, (PBYTE)&dwCrc32, 4);

__lbEncE:
	EncTarget2(dwStart, dwEnd - dwStart, Key, 6);
}

// 用OnitialDlg中的crc值加密CMaze::LetsGo
void CMainDlg::EncLetsGoWithCrcOfOID()
{
	byte Key[] = { 0x6F, 0x5E, 0x4D, 0x3C, 0x2B, 0x1A };
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	DecTarget2(dwStart, dwEnd - dwStart, Key, 6);
__lbEncS:

	bool (CMaze::*pFnLetsGo)(std::string, list<Path>&) = &CMaze::LetsGo;
	LRESULT(CMainDlg::*pFnOnInitDialog)(UINT, WPARAM, LPARAM, BOOL&) = &CMainDlg::OnInitDialog;
	crc_32_self crc32;
	crc32.process_bytes((void *)*(PDWORD)&pFnOnInitDialog, SZ_OnInitDialog);
	DWORD dwCrc32 = crc32.checksum();
	// 用OnInitialDlg处的CRC值加密LetsGo
	EncTarget3(*(PDWORD)&pFnLetsGo, SZ_LetsGo, (PBYTE)&dwCrc32, 4);

__lbEncE:
	EncTarget2(dwStart, dwEnd - dwStart, Key, 6);
}

// 用Initialize中的crc值来解码CMatrixMul::MMulEqual
void CMainDlg::DecMMulEqualWithCrcOfInit()
{
	byte Key[] = { 0xCF, 0xBE, 0xAD, 0x9C, 0x8B, 0x7E };
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	DecTarget3(dwStart, dwEnd - dwStart, Key, 6);
__lbEncS:

	void (CMainDlg::*pFnInit)() = &CMainDlg::Initialize;

	crc_32_self crc32;
	// Initialize函数的大小为 0x669
	crc32.process_bytes((void *)*(PDWORD)&pFnInit, SZ_Initialize);
	DWORD dwCrc32 = crc32.checksum();

	bool (CMatrixMul::*pFnMMulEqual)(string) = &CMatrixMul::MMulEqual;
	// 用nitialize处的CRC值加密MMulEqual 大小为 0x10D2
	DecTarget2(*(PDWORD)&pFnMMulEqual, SZ_MMulEqual, (PBYTE)&dwCrc32, 4);

__lbEncE:
	EncTarget3(dwStart, dwEnd - dwStart, Key, 6);
}

// 用Initialize中的crc值来加密CMatrixMul::MMulEqual
void CMainDlg::EncMMulEqualWithCrcOfInit()
{
	byte Key[] = { 0x7F, 0x8E, 0x9D, 0xAC, 0xBB, 0xCE };
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	DecTarget3(dwStart, dwEnd - dwStart, Key, 6);
__lbEncS:
	void (CMainDlg::*pFnInit)() = &CMainDlg::Initialize;
	crc_32_self crc32;
	crc32.process_bytes((void *)*(PDWORD)&pFnInit, SZ_Initialize);
	DWORD dwCrc32 = crc32.checksum();

	bool (CMatrixMul::*pFnMMulEqual)(string) = &CMatrixMul::MMulEqual;
	// 用Initialize处的CRC值加密MMulEqual 大小: 0x10D2
	EncTarget2(*(PDWORD)&pFnMMulEqual, SZ_MMulEqual, (PBYTE)&dwCrc32, 4);

__lbEncE:
	EncTarget3(dwStart, dwEnd - dwStart, Key, 6);
}


void CMainDlg::Initialize()
{
	// 函数中的代码进行加密处理
#ifdef __SMC__
	byte Key[] = { 0xCC, 0xEB, 0xFA, 0x9D, 0x8E, 0x68, 0x3A, 0x7D };
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	DecTarget1(dwStart, dwEnd - dwStart, Key, 8);

__lbEncS:
	CSelfCheck csc;
	// 以代码段的CRC校验值对MoveThere进行解码
	DWORD dwCrc32 = csc.ComputeFileCodeCRC();
	void (CMainDlg::*pFnMoveThere)(std::shared_ptr<char>) = &CMainDlg::MoveThere;

	// 此处有重定位，若放到加密中，会导致定位的地址不正确，引起崩溃！
	// MoveThere的大小为 0xD29
	DecTarget3((DWORD)byteMoveThere, SZ_MoveThere, (PBYTE)&dwCrc32, 4);
	// MoveThere的大小为 0xD29
	memcpy_s((void*)*(PDWORD)&pFnMoveThere, SZ_MoveThere, byteMoveThere, SZ_MoveThere);

#endif
	//SetTimer
	byte ST[]{ 0x8D,0xBA,0x94,0xB5,0x8B,0x8E,0x81,0x97,0xE6 };
	for (int i = 0; i <0x09; i++)
		ST[i] ^= (0xDD + i + 1);

	// GetWindowTextW
	byte GWT[]{ 0xA8,0x95,0x85,0xA5,0x9A,0x9A,0x91,0x99,0x80,0xAC,0x9C,0x82,0x8F,0xAB,0xFD };
	for (int i = 0; i <0x0F; i++)
		GWT[i] ^= (0xEE + i + 1);

	// MessageBoxW
	byte MB[]{ 0xb3,0x98,0x8f,0x88,0x9b,0x9e,0x9d,0xb5,0x99,0x8d,0xa3,0xf3 };
	for (int i = 0; i < 0x0c; i++)
		MB[i] ^= (0xFF - (i + 1));

	m_pSetTimer = (fnSetTimer)m_cUser32.GetFunctionAddr(reinterpret_cast<LPCSTR>(ST));
	assert(m_pSetTimer);
	m_pGetWindowText = (fnGetWindowText)m_cUser32.GetFunctionAddr(reinterpret_cast<LPCSTR>(GWT));
	assert(m_pGetWindowText);
	m_pMessageBox = (fnMessageBox)m_cUser32.GetFunctionAddr(reinterpret_cast<LPCSTR>(MB));
	assert(m_pMessageBox);
	
	//TCHAR SysDir[100] = { 0 };
	//WORD key[] = { 0xAC12, 0xBD34, 0xCE56, 0xDF78, 0xBC90, 0xDEAB, 0xEBCA, 0xABCD, 0xBFEC, 0xDEFB, 0xFDEB };

	//// 对user32.dll的名称进行加密处理
	//TCHAR szUser32[] = { L'u' ^ 0xAC12, L's' ^ 0xBD34, L'e' ^ 0xCE56,
	//	L'r' ^ 0xDF78, L'3' ^ 0xBC90, L'2' ^ 0xDEAB, L'.' ^ 0xEBCA, L'd' ^ 0xABCD, L'l' ^ 0xBFEC, L'l' ^ 0xDEFB, 0xFDEB };
	//int cnt = 0;
	//std::for_each(std::begin(szUser32), std::end(szUser32), [&](TCHAR& c) {
	//	c ^= key[cnt++];
	//});
	//GetSystemWow64Directory(SysDir, 100);
	//if (ERROR_CALL_NOT_IMPLEMENTED == GetLastError())
	//	GetSystemDirectory(SysDir, 100);
	//PathCombine(SysDir, SysDir, szUser32);
	//TCHAR szOpen[] = { L'r' ^ 0xFC75, L'b' ^ 0xADE8 , 0x74EA };
	//key[0] = 0xFC75, key[1] = 0xADE8, key[2] = 0x74EA;
	//cnt = 0;
	//std::for_each(std::begin(szOpen), std::end(szOpen), [&](TCHAR& c) {
	//	c ^= key[cnt++];
	//});
	//FILE* fp = nullptr;
	//_wfopen_s(&fp, SysDir, szOpen);
	//if (fp)
	//{
	//	std::shared_ptr<char> pFile(new char[0x100000], std::default_delete<char[]>());
	//	if (pFile)
	//	{
	//		size_t sz = fread(&*pFile, 1, 0x100000, fp);
	//		fclose(fp);
	//		if (m_cUser32.MemLoadLibrary(&*pFile, sz))
	//		{
	//			m_pSetTimer = (fnSetTimer)m_cUser32.MemGetProcAddress("SetTimer");
	//			assert(m_pSetTimer);
	//			m_pGetWindowText = (fnGetWindowText)m_cUser32.MemGetProcAddress("GetWindowTextW");
	//			assert(m_pGetWindowText);
	//		}

	//	}
	//	fclose(fp);
	//}
#ifdef __SMC__
__lbEncE:
	EncTarget1(dwStart, dwEnd - dwStart, Key, 8);
#endif
}
