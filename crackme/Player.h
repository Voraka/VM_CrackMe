#pragma once

// 作者：不问年少
// 日期：2017-7-15
// 类名：CPlayer
// 作用：代表玩家对象及操作

class CNineTowers;
class CPlayer
{
public:

	CPlayer();
	~CPlayer();

protected:
	bool Walk(size_t szDir, CNineTowers& nt);
	
	typedef struct _D3Pos
	{
		int x, y, z;
		_D3Pos() { x = y = z = 0; }
		_D3Pos(int x, int y, int z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		void Set(int x, int y, int z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		bool operator == (_D3Pos& oth)
		{
			return (x == oth.x && y == oth.y && z == oth.z);
		}

	}D3Pos;
	friend class CNineTowers;
	// 上a 下V 左k 右D z轴下 z
	enum {YUP = 3826002220, YDOWN = 3540782697, XLEFT = 3993778410, XRIGHT = 3238785555, ZDOWN = 4278997933 };
	enum { STEP_X = 0x234, STEP_Y =0x158, STEP_Z = 0x169 };

	// 设定人物初始原点所代表的世界坐标点
	enum {O_X = 0x2018, O_Y=-0x2017, O_Z=0x1999};



private:
	double m_fHealth = 100.0;   // 人物初始生命值
	D3Pos m_d3Pos;				// 人物当前的坐标
	int m_nSteps = 0;

public:

	// 转换当前人物坐标至地图数组坐标
	D3Pos Pos2MapPos()
	{
		return D3Pos(4-(m_d3Pos.y - O_Y) / STEP_Y, 4+(m_d3Pos.x - O_X) / STEP_X,  (m_d3Pos.z - O_Z) / STEP_Z);
	}
	// 判断是否成功通过九层妖塔
	 bool IsGetThrough();

};

