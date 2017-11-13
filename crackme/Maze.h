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
	// �Թ���XY�����޶�
	enum BOUND{X_MIN=0, Y_MIN=0, X_MAX=8, Y_MAX=8, MAZE_SZ = 9};
	// �������ҵ�hashֵ ��u ��V ��z ��R
	enum HASH_DIR {UP = 4027333648, DOWN = 3540782697, LEFT = 4278997933, RIGHT= 3607893173};
	// �ƶ��Ĳ���
	enum MOVE_STEP {STEP = 0x2018};
	// �����Թ����������ͷ��ʵ�
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
	// ������:walk
	// ���ܣ������������Թ��е��ƶ�
	// ����:
	// int nDirection: 0���� 1���� 2���� 3���� ����ֵ:���ƶ�
	// ����ֵ:�ƶ��ɹ�����true,���򷵻�false
	***************************************************/
	bool walk(int nDirection, list<Path>& lstPath);
	bool canMoveTo(CPoint ptTarg, list<Path>& lstPath)
	{
		// ת����ǰ����㵽������������
		CPoint ptTarget(1 - (ptTarg.y - m_ptOrigin.y) / STEP, (ptTarg.x - m_ptOrigin.x) / STEP);

		// Խ�磡����
		if (ptTarget.x < X_MIN || ptTarget.x > X_MAX ||
			ptTarget.y <Y_MIN || ptTarget.y > Y_MAX)
			return false;
		
		// ��ǰ�����ھ�������������Թ��������ʵλ��0��ʾ����ͨ�У�1��ʾ��ֹ)
		boost::dynamic_bitset<DWORD> bs;

		// ȡ�Թ����Թ켣���б�
		auto itB = lstPath.begin();
		advance(itB, (ptTarget.x*MAZE_SZ + ptTarget.y) >> 1);
		
		if ((ptTarget.x*MAZE_SZ + ptTarget.y) & 1)
			bs.append(m_vM[ptTarget.x][ptTarget.y]^ (itB->pt_to.y));
		else
			bs.append(m_vM[ptTarget.x][ptTarget.y] ^ (itB->pt_to.x));
		return ! bs.test((ptTarget.x % 3) * 9 + ptTarget.y);
	}

	// ��Ϊ����������[1,0]��Ϊ�ƶ���ԭ������������ģ������յ���������[8,7]ת��Ϊλ������x�����Ӧ7��y���귽���෴�������ƶ�һ��
	// return static_cast<bool>(m_ptPos == CPoint(STEP * 7 + m_ptOrigin.x, m_ptOrigin.y- 7 * STEP)
	// �˴�ֱ��д�볣�������ñ�¶�Թ��������㷨��
	bool IsGetOut() { 
		//ATLTRACE("%X, %X", STEP * 7 + m_ptOrigin.x, m_ptOrigin.y - 7 * STEP);
		return (m_ptPos.x == 0x00010EBF && m_ptPos.y == 0xFFFEF7D7); }

private:
	vector<vector<DWORD>> m_vM; // ���� 9*9�ľ����Թ�
	CPoint m_ptPos;				// ��ǰ�������ڵ�����
	int m_nLeftSteps;			// ��ǰ����ʣ����ƶ��Ĳ���
	vector<CPoint> m_vPtVis;	// �Ѿ����ʹ�������㣬��ֹ�ظ���
	
	/*��ʱ�������������ɼ��ܵ��Թ�����*/
	//vector<Path> m_lstPath;
	//vector<int> x;
	CPoint m_ptOrigin;			// �����ƶ�����ʼ����Ϊԭ��
};
