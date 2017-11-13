// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
//#include "MemLoadDll.h"
#include "CopyedUser32.h"
#include "Maze.h"
#include "MatrixMul.h"

#include <list>
#include <atltypes.h>
#include <memory>
#include <boost/signals2/signal.hpp>
#include <boost/uuid/sha1.hpp>



extern "C"
{
	int WINAPI MessageBoxTimeoutA(IN HWND hWnd, IN LPCSTR lpText, IN LPCSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);
	int WINAPI MessageBoxTimeoutW(IN HWND hWnd, IN LPCWSTR lpText, IN LPCWSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);
};

#ifdef UNICODE
#define MessageBoxTimeout MessageBoxTimeoutW
#else
#define MessageBoxTimeout 
#endif

using namespace boost::signals2;
class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	/*
	// OnTimer的长度为0x19A
	crc32.process_bytes((const void*)(PBYTE)*(PDWORD)&pFnOnTimer, 0x19A);
	DWORD dwCrc32 = crc32.checksum();

	// StartGame长度为0x1282
	
	*/
	// 加解密时需要使用的各函数体大小
	enum E_SIZE_FUNC:DWORD {
		SZ_OnInitDialog = 0x4D4,
		SZ_StartGame = 0xDC2,
		SZ_OnTimer = 0x19D,
		SZ_LetsGo = 0x68B,
		SZ_MMulEqual = 0x10F2,
		SZ_MoveThere = 0x1109,
		SZ_Initialize = 0x298,
		SZ_DecStartGame = 0xF7,
		SZ_EncStartGame = 0xF7,
		SZ_InitGame = 0xDE7,
		SZ_Play = 0x6E7,
		SZ_Walk = 0x571,
		SZ_IsGetThrough = 0x1A1,
		SZ_HandleWinEvent = 0x4A
	};
	
	enum { IDD = IDD_MAINDLG };
	CMainDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_HANDLER(IDC_BUTTON1, BN_CLICKED, OnBnClickedButton1)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void CloseDialog(int nVal);

protected:
	__forceinline bool IsHardbreakDetected()
	{
		CONTEXT context;
		HANDLE hThread = GetCurrentThread();
		context.ContextFlags = CONTEXT_DEBUG_REGISTERS;
		GetThreadContext(hThread, &context);
		return (context.Dr0 || context.Dr1 || context.Dr2 || context.Dr3);
	}

	__forceinline bool IsSoftbreakDetected()
	{
		// 只检测GetWindowText、GetDlgItemText、SendMessage这三个函数的ANSI+UNICODE版本
		int (WINAPI *pFnGetWindowTextW)(HWND, LPWSTR, int) = ::GetWindowTextW;
		int (WINAPI *pFnGetWindowTextA)(HWND, LPSTR, int) = ::GetWindowTextA;
		UINT_PTR(WINAPI *pFnSetTimer)(HWND, UINT_PTR, UINT, TIMERPROC) = ::SetTimer;
		BOOL(WINAPI *pFnPostMessageW)(HWND, UINT, WPARAM, LPARAM) = ::PostMessageW;
		void (WINAPI *pFnExitProcess)(UINT) = ExitProcess;
		void (WINAPI *pFnPostQuitMessage)(int) = PostQuitMessage;
		HANDLE(WINAPI *pFnCreateFileW)(LPCWSTR, DWORD, DWORD,
			LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE) = CreateFile;

		PBYTE pFn[] =
		{
			(PBYTE)pFnGetWindowTextW,
			(PBYTE)pFnGetWindowTextA,
			(PBYTE)pFnSetTimer,
			(PBYTE)pFnPostMessageW,
			(PBYTE)pFnExitProcess,
			(PBYTE)pFnPostQuitMessage,
			(PBYTE)pFnCreateFileW
		};


		for (int nFn = 0; nFn < 7; nFn++)
		{
			PBYTE pFunc = &*(pFn[nFn]);
			// 只检测前20个字节
			for (int i = 0; i < 20; i++)
			{
				// 若发现断点，直接返回true
				if (((*(pFunc + i)) ^ 0x44) == 0x88)
				{
					return true;
				}
			}
		}
		return false;
	}


	void FillPath();
	void StartGame();
	void Leave();
	void CMainDlg::Move(CPoint& curPos, int iDirect);
	
	void MoveThere(std::shared_ptr<char> ptr);
	void DecLetsGoWithCrcOfOID();
	void EncLetsGoWithCrcOfOID();
	void DecMMulEqualWithCrcOfInit();
	void EncMMulEqualWithCrcOfInit();
	void Initialize();
public:
	// 解码以Timer代码的crc32为密钥加密的StartGame代码
	void DecStartGame();
	// 使用后再次加密StartGame
	void EncStartGame();
private:
	/*自己从内存中加载user32.dll从而使一般的断点失去作用。*/
	CCopyedUser32 m_cUser32;		// 内存拷贝user32.dll对象(此方式比从文件加载更隐蔽)
	typedef int (WINAPI *fnGetWindowText)(HWND, LPWSTR, int);
	fnGetWindowText m_pGetWindowText = nullptr;
	
	typedef UINT_PTR (WINAPI *fnSetTimer)(HWND,UINT_PTR,UINT,TIMERPROC);
  	fnSetTimer m_pSetTimer = nullptr;

	typedef int (WINAPI *fnMessageBox)(HWND, LPCWSTR, LPCWSTR,UINT);
	fnMessageBox m_pMessageBox = nullptr;

	CWindow m_edit;
	std::list<Path> m_lstPath;
	signal<void()> m_sig;
	std::string m_strInputL;
	std::string m_strInputR;
	//std::string m_strEnc16;
	std::list<Path> m_userPath;
	CMaze m_mz;		// 迷宫类对象
	// 记录中间31字符的移动轨迹
	std::list<Path> m_lstMidPath;
	CMatrixMul m_mm; // 矩阵相乘类对象
};
