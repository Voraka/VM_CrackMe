#pragma once
#include <vector>
#include <Windows.h>
#include <atlbase.h>
#include <atltypes.h>
#include <list>
#include <boost/dynamic_bitset.hpp>

using namespace std;

class CMaze
{
public:
	CMaze(void);
	~CMaze(void);
	// 迷宫的XY方向限定
	enum BOUND{X_MIN=0, Y_MIN=0, X_MAX=8, Y_MAX=8, MAZE_SZ = 9};
	// 上下左右的hash值 上u 下V 左z 右R
	enum HASH_DIR {UP = 4027333648, DOWN = 3540782697, LEFT = 4278997933, RIGHT= 3607893173};
	// 移动的步长
	enum MOVE_STEP {STEP = 0x2018};
	// 重置迷宫人物的坐标和访问点
	void Reset(CPoint ptStart) 
	{
		m_ptOrigin = m_ptPos = ptStart;
		m_vPtVis.clear();
		m_vPtVis.push_back(m_ptPos);
		m_nLeftSteps = 40;
	};
	//typedef struct _Path
	//{
	//	CPoint pt_fr;
	//	CPoint pt_to;
	//}Path;
public:
	bool LetsGo(string strInput, list<Path>& lstPath);

protected:
	/**************************************************
	// 函数名:walk
	// 功能：操作人物在迷宫中的移动
	// 参数:
	// int nDirection: 0：右 1：下 2：左 3：上 其他值:不移动
	// 返回值:移动成功返回true,否则返回false
	***************************************************/
	bool walk(int nDirection, list<Path>& lstPath);
	bool canMoveTo(CPoint ptTarg, list<Path>& lstPath)
	{
		// 转换当前坐标点到数组索引坐标
		CPoint ptTarget(1 - (ptTarg.y - m_ptOrigin.y) / STEP, (ptTarg.x - m_ptOrigin.x) / STEP);

		// 越界！！！
		if (ptTarget.x < X_MIN || ptTarget.x > X_MAX ||
			ptTarget.y <Y_MIN || ptTarget.y > Y_MAX)
			return false;
		
		// 当前点所在矩阵的行数决定迷宫矩阵的真实位（0表示可以通行，1表示禁止)
		boost::dynamic_bitset<DWORD> bs;

		// 取迷宫加迷轨迹点列表
		auto itB = lstPath.begin();
		advance(itB, (ptTarget.x*MAZE_SZ + ptTarget.y) >> 1);
		
		if ((ptTarget.x*MAZE_SZ + ptTarget.y) & 1)
			bs.append(m_vM[ptTarget.x][ptTarget.y]^ (itB->pt_to.y));
		else
			bs.append(m_vM[ptTarget.x][ptTarget.y] ^ (itB->pt_to.x));
		return ! bs.test((ptTarget.x % 3) * 9 + ptTarget.y);
	}

	// 因为数组坐标是[1,0]作为移动的原点坐标来计算的，所以终点数组坐标[8,7]转化为位置坐标x方向对应7，y坐标方向相反，且少移动一步
	// return static_cast<bool>(m_ptPos == CPoint(STEP * 7 + m_ptOrigin.x, m_ptOrigin.y- 7 * STEP)
	// 此处直接写入常数，不用暴露迷宫的坐标算法。
	bool IsGetOut() { 
		//ATLTRACE("%X, %X", STEP * 7 + m_ptOrigin.x, m_ptOrigin.y - 7 * STEP);
		return (m_ptPos.x == 0x00010EBF && m_ptPos.y == 0xFFFEF7D7); }

private:
	vector<vector<DWORD>> m_vM; // 定义 9*9的矩阵迷宫
	CPoint m_ptPos;				// 当前人物所在的坐标
	int m_nLeftSteps;			// 当前人物剩余的移动的步数
	vector<CPoint> m_vPtVis;	// 已经访问过的坐标点，防止重复走
	
	/*临时变量，用于生成加密的迷宫数据*/
	//vector<Path> m_lstPath;
	//vector<int> x;
	CPoint m_ptOrigin;			// 保存移动的起始点作为原点
};
