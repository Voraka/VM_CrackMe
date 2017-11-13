#include "stdafx.h"
#include "Player.h"
#include <boost/dynamic_bitset.hpp>
#include "NineTowers.h"
#include <mbedtls/aes.h>
#pragma comment(lib, "mbedtls.lib")

CPlayer::CPlayer()
{
}


CPlayer::~CPlayer()
{
}


// 操纵人物移动，设定整个立体迷宫的正中心为坐标原点
// 则左、右、上、下各有4个单位的移动空间
// Z方向有九个单位的移动空间（Z轴设定向下）
bool CPlayer::Walk(size_t szDir, CNineTowers& nt)
{
#ifdef __SMC__
	//byte Key[] = { 'g', '0', 'o', 'D', '1', 'U', 'C', 'K' };
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	byte IV[16]{ 0 };
	byte aes_key[16]{ 'B', '9', 'W', '2', 'n', '0', 'S', '1', 'N', '7', '8', 'i', 'n', '1', '^', '!' };
	mbedtls_aes_context aes_ctx;
	mbedtls_aes_init(&aes_ctx);
	mbedtls_aes_setkey_enc(&aes_ctx, aes_key, 128);
	mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_DECRYPT, dwEnd - dwStart,
		IV, (PBYTE)dwStart, (PBYTE)dwStart);
	//DecTarget2(dwStart, dwEnd - dwStart, Key, 8);
__lbEncS:
#endif
	bool bRet = false;
	if (m_nSteps - 1 < 0)
#ifdef __SMC__
		goto __lbEncE;
#else
		return false;
#endif
	m_nSteps--;
	bool bZMove = false;
	switch (szDir)
	{
	case XLEFT:
		if ((m_d3Pos.x - STEP_X - O_X) / STEP_X < -4)  // 不能穿墙！
#ifdef __SMC__
			goto __lbEncE;
#else
			return false;
#endif
		else
			m_d3Pos.x -= STEP_X;
		break;
	case XRIGHT:
		if ((m_d3Pos.x + STEP_X - O_X) / STEP_X > 4)  // 不能穿墙！
#ifdef __SMC__
			goto __lbEncE;
#else
			return false;
#endif
		else
			m_d3Pos.x += STEP_X;
		break;
	case YUP:
		if ((m_d3Pos.y + STEP_Y - O_Y) / STEP_Y > 4)  // 不能穿墙！
#ifdef __SMC__
			goto __lbEncE;
#else
			return false;
#endif
		else
			m_d3Pos.y += STEP_Y;
		break;
	case YDOWN:
		if ((m_d3Pos.y - STEP_Y - O_Y) / STEP_Y < -4)  // 不能穿墙！
#ifdef __SMC__
			goto __lbEncE;
#else
			return false;
#endif
		else
			m_d3Pos.y -= STEP_Y;
		break;
	case ZDOWN:
		m_d3Pos.z += STEP_Z;
		bZMove = true;
		break;
	default:
		// 其他字符不处理，但增加减少步数。
#ifdef __SMC__
		bRet = true;
		goto __lbEncE;
#else
		return false;
#endif
	}
	{
		boost::dynamic_bitset<> bs;
		D3Pos ptMap = Pos2MapPos();
		auto itB = nt.m_lstPath.begin();
		advance(itB, (ptMap.z + 1) * 5 / 2 + ((ptMap.x * 9 + ptMap.y) >> 1));   // 搜索加密链表对应的起始偏移

		// 根据行、列表的奇偶性来选取对应的坐标值解码
		if ((ptMap.x * 9 + ptMap.y) & 1)
			bs.append(nt.m_nMap[ptMap.z][ptMap.x][ptMap.y] ^ itB->pt_to.x);
		else
			bs.append(nt.m_nMap[ptMap.z][ptMap.x][ptMap.y] ^ itB->pt_to.y);

		// 这里就是嗖嗖地掉血之地！！！
		// 测试地图的相应位是否有怪，0为无，1为有！
		if (bZMove)
		{
			if (ptMap.z < 9)
			{
				// 向下一层移动，会受到BOSS的阻挠，攻击为普通怪的1.1倍！
				m_fHealth -= 1.1 * nt.m_fAttackLevel[(ptMap.z - 1)];
				// 向下穿若遇怪，会受到前层怪1.3倍攻击加上后一层怪1.2倍攻击！
				m_fHealth -= bs.test(((ptMap.x + ptMap.y + 2) << (ptMap.z + 1)) % 32)*(1.3*nt.m_fAttackLevel[ptMap.z - 1] + 1.2*nt.m_fAttackLevel[ptMap.z]);
			}
			else
			{
				bs.clear();
				if ((ptMap.x * 9 + ptMap.y) & 1)
					bs.append(nt.m_nMap[8][ptMap.x][ptMap.y] ^ itB->pt_to.x);
				else
					bs.append(nt.m_nMap[8][ptMap.x][ptMap.y] ^ itB->pt_to.y);

				m_fHealth -= 1.1 * nt.m_fAttackLevel[(ptMap.z - 1)];
				// 最后一层若直接向下穿越，会受到1.5倍于当前攻击
				m_fHealth -= bs.test(((ptMap.x + ptMap.y + 2) << 9) % 32)*(1.5*nt.m_fAttackLevel[8]);
			}
		}
		else
		{
			// 遇到普通妖怪受到1.03倍于正常的攻击（妖塔有攻击加成作用！）
			m_fHealth -= 1.03 * bs.test(((ptMap.x + ptMap.y + 2) << (ptMap.z + 1)) % 32) * nt.m_fAttackLevel[ptMap.z];
		}
		bRet = true;
	}
#ifdef __SMC__
	__lbEncE :
	memset(IV, 0, 16);
	mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_ENCRYPT, dwEnd - dwStart,
		IV, (PBYTE)dwStart, (PBYTE)dwStart);
	mbedtls_aes_free(&aes_ctx);
	//EncTarget2(dwStart, dwEnd - dwStart, Key, 8);
#endif
	return bRet;
}

bool CPlayer::IsGetThrough()
{
#ifdef __SMC__
	//byte Key[] = { 'G', 'o', '0', 'd', 'l', 'u', 'c', 'k' };
	byte aes_key[24]{ '!', '#', '$', '%', '~', '^', '&', '*', '(', ')', '{', '}', '[', ']', '\\', '?', '<', '>', ':', '\'', '|', '`', ',', '.' };
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	byte IV[16]{ 0 };
	mbedtls_aes_context aes_ctx;
	mbedtls_aes_init(&aes_ctx);
	mbedtls_aes_setkey_enc(&aes_ctx, aes_key, 192);
	mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_DECRYPT, dwEnd - dwStart,
		IV, (PBYTE)dwStart, (PBYTE)dwStart);
	//DecTarget3(dwStart, dwEnd - dwStart, Key, 8);
__lbEncS:
#endif
	// 是否活着？
	bool bIsLive = (abs(m_fHealth - 1.0) < 1E-13);
	D3Pos pos(0x2480, 0xffffe299, 0x264a);
	// 是否到达指定坐标点
	bool IsTheSamePos = (pos == m_d3Pos);
	// 是否所有步数用完
	bool bStepsOver = (m_nSteps == 0);

#ifdef __SMC__
__lbEncE :
	memset(IV, 0, 16);
	mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_ENCRYPT, dwEnd - dwStart,
		IV, (PBYTE)dwStart, (PBYTE)dwStart);
	mbedtls_aes_free(&aes_ctx);
			 //EncTarget3(dwStart, dwEnd - dwStart, Key, 8);
#endif
	return bIsLive && IsTheSamePos && bStepsOver;
}
