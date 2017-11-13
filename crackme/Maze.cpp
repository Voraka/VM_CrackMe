#include "StdAfx.h"
#include "Maze.h"
#include <iostream>
#include <boost/algorithm/hex.hpp>
#include <algorithm>
CMaze::CMaze(void) : m_vM(MAZE_SZ, vector<DWORD>(MAZE_SZ, 1)),
m_nLeftSteps(40)
{

	////////	// 此段代码用于生成混淆加密的迷宫数据。
	//m_lstPath.push_back({ CPoint(8215,-6537), CPoint(8282, -6194) });
	//m_lstPath.push_back({ CPoint(8282,-6194), CPoint(8349, -5851) });
	//m_lstPath.push_back({ CPoint(8349,-5851), CPoint(8390, -6178) });
	//m_lstPath.push_back({ CPoint(8390,-6178), CPoint(8765, -6741) });
	//m_lstPath.push_back({ CPoint(8765,-6741), CPoint(9140, -7304) });
	//m_lstPath.push_back({ CPoint(9140,-7304), CPoint(9707, -6999) });
	//m_lstPath.push_back({ CPoint(9707,-6999), CPoint(9416, -6944) });
	//m_lstPath.push_back({ CPoint(9416,-6944), CPoint(9105, -7270) });
	//m_lstPath.push_back({ CPoint(9105,-7270), CPoint(9480, -7833) });
	//m_lstPath.push_back({ CPoint(9480,-7833), CPoint(9185, -7718) });
	//m_lstPath.push_back({ CPoint(9185,-7718), CPoint(8890, -7603) });
	//m_lstPath.push_back({ CPoint(8890,-7603), CPoint(8957, -7260) });
	//m_lstPath.push_back({ CPoint(8957,-7260), CPoint(9524, -6955) });
	//m_lstPath.push_back({ CPoint(9524,-6955), CPoint(9229, -6840) });
	//m_lstPath.push_back({ CPoint(9229,-6840), CPoint(9270, -7167) });
	//m_lstPath.push_back({ CPoint(9270,-7167), CPoint(9837, -6862) });
	//m_lstPath.push_back({ CPoint(9837,-6862), CPoint(10212, -7425) });
	//m_lstPath.push_back({ CPoint(10212,-7425), CPoint(9921, -7370) });
	//m_lstPath.push_back({ CPoint(9921,-7370), CPoint(9630, -7315) });
	//m_lstPath.push_back({ CPoint(9630,-7315), CPoint(9925, -7364) });
	//m_lstPath.push_back({ CPoint(9925,-7364), CPoint(10220, -7413) });
	//m_lstPath.push_back({ CPoint(10220,-7413), CPoint(9929, -7358) });
	//m_lstPath.push_back({ CPoint(9929,-7358), CPoint(9634, -7243) });
	//m_lstPath.push_back({ CPoint(9634,-7243), CPoint(9675, -7570) });
	//m_lstPath.push_back({ CPoint(9675,-7570), CPoint(9384, -7515) });
	//m_lstPath.push_back({ CPoint(9384,-7515), CPoint(9451, -7172) });
	//m_lstPath.push_back({ CPoint(9451,-7172), CPoint(9140, -7498) });
	//m_lstPath.push_back({ CPoint(9140,-7498), CPoint(8829, -7824) });
	//m_lstPath.push_back({ CPoint(8829,-7824), CPoint(9396, -7519) });
	//m_lstPath.push_back({ CPoint(9396,-7519), CPoint(9963, -7214) });
	//m_lstPath.push_back({ CPoint(9963,-7214), CPoint(9652, -7540) });
	//m_lstPath.push_back({ CPoint(9652,-7540), CPoint(9947, -7589) });
	//m_lstPath.push_back({ CPoint(9947,-7589), CPoint(9988, -7916) });
	//m_lstPath.push_back({ CPoint(9988,-7916), CPoint(9693, -7801) });
	//m_lstPath.push_back({ CPoint(9693,-7801), CPoint(9988, -7850) });
	//m_lstPath.push_back({ CPoint(9988,-7850), CPoint(10363, -8413) });
	//m_lstPath.push_back({ CPoint(10363,-8413), CPoint(10930, -8108) });
	//m_lstPath.push_back({ CPoint(10930,-8108), CPoint(10619, -8434) });
	//m_lstPath.push_back({ CPoint(10619,-8434), CPoint(10324, -8319) });
	//m_lstPath.push_back({ CPoint(10324,-8319), CPoint(10365, -8646) });
	//m_lstPath.push_back({ CPoint(10365,-8646), CPoint(10740, -9209) });
	//m_lstPath.push_back({ CPoint(10740,-9209), CPoint(10429, -9535) });
	//m_lstPath.push_back({ CPoint(10429,-9535), CPoint(10470, -9862) });
	//m_lstPath.push_back({ CPoint(10470,-9862), CPoint(10511, -10189) });
	//m_lstPath.push_back({ CPoint(10511,-10189), CPoint(10806, -10238) });
	//m_lstPath.push_back({ CPoint(10806,-10238), CPoint(10873, -9895) });
	//m_lstPath.push_back({ CPoint(10873,-9895), CPoint(11168, -9944) });
	//m_lstPath.push_back({ CPoint(11168,-9944), CPoint(11543, -10507) });
	//m_lstPath.push_back({ CPoint(11543,-10507), CPoint(12110, -10202) });
	//m_lstPath.push_back({ CPoint(12110,-10202), CPoint(11819, -10147) });
	//m_lstPath.push_back({ CPoint(11819,-10147), CPoint(11528, -10092) });
	//m_lstPath.push_back({ CPoint(11528,-10092), CPoint(12095, -9787) });
	//m_lstPath.push_back({ CPoint(12095,-9787), CPoint(11784, -10113) });
	//m_lstPath.push_back({ CPoint(11784,-10113), CPoint(11493, -10058) });
	//m_lstPath.push_back({ CPoint(11493,-10058), CPoint(11560, -9715) });
	//m_lstPath.push_back({ CPoint(11560,-9715), CPoint(11601, -10042) });
	//m_lstPath.push_back({ CPoint(11601,-10042), CPoint(11976, -10605) });
	//m_lstPath.push_back({ CPoint(11976,-10605), CPoint(12043, -10262) });
	//m_lstPath.push_back({ CPoint(12043,-10262), CPoint(12110, -9919) });
	//m_lstPath.push_back({ CPoint(12110,-9919), CPoint(11799, -10245) });
	//m_lstPath.push_back({ CPoint(11799,-10245), CPoint(12094, -10294) });
	//m_lstPath.push_back({ CPoint(12094,-10294), CPoint(11799, -10179) });
	//m_lstPath.push_back({ CPoint(11799,-10179), CPoint(12094, -10228) });
	//m_lstPath.push_back({ CPoint(12094,-10228), CPoint(11799, -10113) });

	//vector<Path>::iterator itBeg = begin(m_lstPath);
	//for (; itBeg != end(m_lstPath); itBeg++)
	//{
	//	x.push_back(itBeg->pt_to.x);
	//	x.push_back(itBeg->pt_to.y);
	//}
	//m_vM[0][0] = 21561;
	//m_vM[0][1] = 23587;
	//m_vM[0][2] = 16079;
	//m_vM[0][3] = 40281;
	//m_vM[0][4] = 19711;
	//m_vM[0][5] = 32591;
	//m_vM[0][6] = 31495;
	//m_vM[0][7] = 35864;
	//m_vM[0][8] = 21414;

	//m_vM[1][0] = 11620;
	//m_vM[1][1] = 26852;
	//m_vM[1][2] = 31720;
	//m_vM[1][3] = 39743;
	//m_vM[1][4] = 31042;
	//m_vM[1][5] = 10129;
	//m_vM[1][6] = 36430;
	//m_vM[1][7] = 13575;
	//m_vM[1][8] = 155713;

	//m_vM[2][0] = 274387;
	//m_vM[2][1] = 32522;
	//m_vM[2][2] = 1081995;
	//m_vM[2][3] = 25543;
	//m_vM[2][4] = 32586;
	//m_vM[2][5] = 10188;
	//m_vM[2][6] = 16798480;
	//m_vM[2][7] = 24857;
	//m_vM[2][8] = 19702;

	//m_vM[3][0] = 14116;
	//m_vM[3][1] = 24625;
	//m_vM[3][2] = 39796;
	//m_vM[3][3] = 16114;
	//m_vM[3][4] = 11068;
	//m_vM[3][5] = 40426;
	//m_vM[3][6] = 11344;
	//m_vM[3][7] = 27039;
	//m_vM[3][8] = 25794;

	//m_vM[4][0] = 20757;
	//m_vM[4][1] = 19989;
	//m_vM[4][2] = 16283;
	//m_vM[4][3] = 26152;
	//m_vM[4][4] = 787;
	//m_vM[4][5] = 29903;
	//m_vM[4][6] = 55188;
	//m_vM[4][7] = 37647;
	//m_vM[4][8] = 12846;

	//m_vM[5][0] = 26959;
	//m_vM[5][1] = 10976;
	//m_vM[5][2] = 1082368;
	//m_vM[5][3] = 2123548;
	//m_vM[5][4] = 23170;
	//m_vM[5][5] = 12687;
	//m_vM[5][6] = 16794205;
	//m_vM[5][7] = 37126;
	//m_vM[5][8] = 67148030;

	//m_vM[6][0] = 32683;
	//m_vM[6][1] = 32917;
	//m_vM[6][2] = 36074;
	//m_vM[6][3] = 28883;
	//m_vM[6][4] = 18366;
	//m_vM[6][5] = 8834;
	//m_vM[6][6] = 31870;
	//m_vM[6][7] = 21828;
	//m_vM[6][8] = 26261;

	//m_vM[7][0] = 10918;
	//m_vM[7][1] = 20182;
	//m_vM[7][2] = 23885;
	//m_vM[7][3] = 19524;
	//m_vM[7][4] = 32248;
	//m_vM[7][5] = 9223;
	//m_vM[7][6] = 33671;
	//m_vM[7][7] = 87658;
	//m_vM[7][8] = 13896;

	//m_vM[8][0] = 280333;
	//m_vM[8][1] = 562643;
	//m_vM[8][2] = 1060790;
	//m_vM[8][3] = 29468;
	//m_vM[8][4] = 19114;
	//m_vM[8][5] = 40947;
	//m_vM[8][6] = 16812202;
	//m_vM[8][7] = 24445;
	//m_vM[8][8] = 34832;

	//for (int i = 0; i < 9; i++)
	//{
	//	for (int j = 0; j < 9; j++)
	//	{
	//		m_vM[i][j] ^= x[i * 9 + j];
	//		ATLTRACE("m_vM[%d][%d] = %u;\r\n", i, j, m_vM[i][j]);
	//		//cout << "m_vM[" << i << "][" << j << "] = " << m_vM[i][j] << ";"<<endl;
	//	}
	//	//cout << endl;
	//}
	////cout << endl;

	 m_vM[0][0] = 29795;
	 m_vM[0][1] = 4294949869;
	 m_vM[0][2] = 7762;
	 m_vM[0][3] = 4294931580;
	 m_vM[0][4] = 27705;
	 m_vM[0][5] = 4294940817;
	 m_vM[0][6] = 22842;
	 m_vM[0][7] = 4294928819;
	 m_vM[0][8] = 28690;
	 m_vM[1][0] = 4294954524;
	 m_vM[1][1] = 19727;
	 m_vM[1][2] = 4294942529;
	 m_vM[1][3] = 49143;
	 m_vM[1][4] = 4294942114;
	 m_vM[1][5] = 1024;
	 m_vM[1][6] = 4294929876;
	 m_vM[1][7] = 4111;
	 m_vM[1][8] = 4294803750;
	 m_vM[2][0] = 265266;
	 m_vM[2][1] = 4294942416;
	 m_vM[2][2] = 1089585;
	 m_vM[2][3] = 4294934922;
	 m_vM[2][4] = 23991;
	 m_vM[2][5] = 4294952040;
	 m_vM[2][6] = 16807460;
	 m_vM[2][7] = 4294936012;
	 m_vM[2][8] = 26875;
	 m_vM[3][0] = 4294955628;
	 m_vM[3][1] = 17415;
	 m_vM[3][2] = 4294934389;
	 m_vM[3][3] = 6303;
	 m_vM[3][4] = 4294954510;
	 m_vM[3][5] = 47630;
	 m_vM[3][6] = 4294954671;
	 m_vM[3][7] = 20318;
	 m_vM[3][8] = 4294936564;
	 m_vM[4][0] = 29835;
	 m_vM[4][1] = 4294946168;
	 m_vM[4][2] = 6494;
	 m_vM[4][3] = 4294935828;
	 m_vM[4][4] = 9471;
	 m_vM[4][5] = 4294940612;
	 m_vM[4][6] = 61789;
	 m_vM[4][7] = 4294930509;
	 m_vM[4][8] = 6028;
	 m_vM[5][0] = 4294937338;
	 m_vM[5][1] = 3883;
	 m_vM[5][2] = 4293879406;
	 m_vM[5][3] = 2114484;
	 m_vM[5][4] = 4294948903;
	 m_vM[5][5] = 5476;
	 m_vM[5][6] = 4278165921;
	 m_vM[5][7] = 45746;
	 m_vM[5][8] = 4227824200;
	 m_vM[6][0] = 24022;
	 m_vM[6][1] = 4294926821;
	 m_vM[6][2] = 43102;
	 m_vM[6][3] = 4294939250;
	 m_vM[6][4] = 24917;
	 m_vM[6][5] = 4294951248;
	 m_vM[6][6] = 22986;
	 m_vM[6][7] = 4294948808;
	 m_vM[6][8] = 16462;
	 m_vM[7][0] = 4294953213;
	 m_vM[7][1] = 27090;
	 m_vM[7][2] = 4294949977;
	 m_vM[7][3] = 27033;
	 m_vM[7][4] = 4294941823;
	 m_vM[7][5] = 771;
	 m_vM[7][6] = 4294927057;
	 m_vM[7][7] = 97809;
	 m_vM[7][8] = 4294961515;
	 m_vM[8][0] = 290239;
	 m_vM[8][1] = 4294407559;
	 m_vM[8][2] = 1050317;
	 m_vM[8][3] = 4294945810;
	 m_vM[8][4] = 25342;
	 m_vM[8][5] = 4294918258;
	 m_vM[8][6] = 16818391;
	 m_vM[8][7] = 4294934855;
	 m_vM[8][8] = 41444;

}

CMaze::~CMaze(void)
{
}

bool CMaze::LetsGo(string strInput, list<Path>& lstPath)
{
	// 取前3字符
	string sLeft = strInput.substr(0, 3);
	shared_ptr<char> pb64(new char[5], default_delete<char[]>());
	base64Encode(&*pb64, sLeft.c_str(), 3);

	// 取后10字符
	string sRight = strInput.substr(3, 10);
	shared_ptr<char> ptrb32(new char[21], default_delete<char[]>());
	size_t sz32 = base32Encode(&*ptrb32, sRight.c_str(), 10);
	// 连接后7字符，形成7+8==15字符
	StrCatA(&*ptrb32, &*pb64);
	int nLength = lstrlenA(&*ptrb32);
	size_t sz16 = base16EncodeGetLength(nLength);
	shared_ptr<char> ptrEnc(new char[sz16], default_delete<char[]>());
	sz16 = base16Encode(&*ptrEnc, &*ptrb32, nLength);

	// 乱序索引表，将方向串进行换位打乱
	int x_order[] = 
	{ 
		 0x0A,  0x11,  0x04,  0x1D,  0x1C,  0x23,  0x0B,  0x25,  0x13,  0x0C,
		 0x24,  0x22,  0x00,  0x02,  0x01,  0x1E,  0x09,  0x17,  0x06,  0x08,
		 0x0F,  0x10,  0x07,  0x0E,  0x19,  0x1A,  0x20,  0x18,  0x26,  0x14,
		 0x0D,  0x12,  0x16,  0x27,  0x1F,  0x21,  0x03,  0x15,  0x1B,  0x05
	};

 

	byte key[]=
	{ 
	 0x71,  0x46,  0x79,  0x40,  0x79,  0x72,  0x6A,  0x79,  0x59,  0x44,
	 0x63,  0x69,  0x68,  0x43,  0x73,  0x6E,  0x6B,  0x16,  0x4E,  0x75,
	 0x43,  0x46,  0x40,  0x5C,  0x77,  0x62,  0x62,  0x5D,  0x7D,  0x3A,
	 0x5C,  0x5B,  0x47,  0x5B,  0x72,  0x67,  0x73,  0x68,  0x45,  0x6E
	};

	//// 此处用于生成方向移动与用户输入相异或后进行再简单换位的方向key串
	//int nKey = 40;//strKey.length();
	//string str = "RVVzVVRVRRVVRRuuuzuzuuRRuuRRVVRVVzVVRVVz";
	//int k = 0;

	//for_each(begin(str), end(str), [&](char&c) {c ^= ((&*ptrEnc)[k] - k - 1); k++; });
	//shared_ptr<char> ptrKey(new char[nKey], default_delete<char[]>());
	//for (int i = 0; i < nKey; i++)
	//{
	//	(&*ptrKey)[x_order[i]] = str[i];
	//}
	//for (int i = 0; i < nKey; i++)
	//	ATLTRACE("0x%02X, ", (&*ptrKey)[i]);


	hash<char> hsh;
	//char* pKey = &*ptrKey;
	char* pEnc = &*ptrEnc;
	for (size_t s = 0; s != sz16; s++)
	{
		//char c = (pEnc[i] - (i + 1)) ^ pKey[s];
		size_t sz = hsh((pEnc[s]-(static_cast<char>(s)+1)) ^ key[x_order[s]]);
		switch (sz)
		{
		case UP:
			if (!walk(3, lstPath))
				return false;
			break;
		case DOWN:
			if (!walk(1, lstPath))
				return false;
			break;
		case LEFT:
			if (!walk(2, lstPath))
				return false;
			break;
		case RIGHT:
			if (!walk(0, lstPath))
				return false;
			break;
		default:
			return false;  // 未定义的方向
			break;
		}
	}
	return true;
}

/**************************************************
// 函数名:walk
// 功能：操作人物在迷宫中的移动
// 参数:
// int nDirection: 0：右 1：下 2：左 3：上 其他值:不移动
// 返回值:移动成功返回true,否则返回false
***************************************************/
bool CMaze::walk(int nDirection, list<Path>& lp)
{
	CPoint ptCur = m_ptPos;
	switch (nDirection)
	{
	case 0: //右走，数组 Y 索引加大
		ptCur.Offset(STEP, 0);  
		break;
	case 1: //下走，数组 X 索引加大
		ptCur.Offset(0, -STEP);
		break;
	case 2://左走, 数组 Y 索引减小
		ptCur.Offset(-STEP, 0);
		break;
	case 3://上走， 数组 X 索引减小
		ptCur.Offset(0, STEP);
		break;

	}
	if (! canMoveTo(ptCur, lp))	// 撞墙了，失败！
		return false;
	
	m_ptPos = ptCur;
	
	// 当前坐标已经访问过了，再次访问就会挂掉，道路已毁，只能通行一次！
	vector<CPoint>::iterator itp = find(m_vPtVis.begin(), m_vPtVis.end(), m_ptPos);
	if (itp != m_vPtVis.end())
		return false;
	// 若未访问过当前点，则记录之
	m_vPtVis.push_back(m_ptPos);

	m_nLeftSteps--;			// 总步数减1
	if (m_nLeftSteps < 0)  // 步数用完了，仍未到达终点，失败！
		return false;
	else if (m_nLeftSteps == 0)  // 若总数已经用完，判断是否已经到达出口。
		return IsGetOut();
	
	// 剩余步数大于0，可以继续移动
	return true;
	
}
