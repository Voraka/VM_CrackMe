#include "stdafx.h"
#include "FixSMC.h"
#include <fstream>
#include <iostream>
#include <cassert>
#include <boost/crc.hpp>
#include <atltrace.h>
#include <boost/uuid/sha1.hpp>
#include <mbedtls/aes.h>
#pragma comment(lib, "mbedtls.lib")

namespace RC6
{
#define w 32	/* word size in bits */
#define r 20	/* based on security estimates */

	//#define P32 0xB7E15163	/* Magic constants for key setup */
	//#define Q32 0x9E3779B9
#define P32 0xC85E63DF
#define Q32 0xAF6B57C3

	/* derived constants */
#define bytes   (w / 8)				/* bytes per word */
#define c_       ((b + bytes - 1) / bytes)	/* key in words, rounded up */
#define R24     (2 * r + 4)
#define lgw     5                       	/* log2(w) -- wussed out */

	/* Rotations */
#define ROTL(x,y) (((x)<<(y&(w-1))) | ((x)>>(w-(y&(w-1)))))
#define ROTR(x,y) (((x)>>(y&(w-1))) | ((x)<<(w-(y&(w-1)))))

	unsigned int S[R24 - 1];		/* Key schedule */

	void rc6_key_setup(unsigned char *K, int b)
	{
		int i, j, s, v;
		unsigned int L[(32 + bytes - 1) / bytes]; /* Big enough for max b */
		unsigned int A, B;

		L[c_ - 1] = 0;
		for (i = b - 1; i >= 0; i--)
			L[i / bytes] = (L[i / bytes] << 8) + K[i];

		S[0] = P32;
		for (i = 1; i <= 2 * r + 3; i++)
			S[i] = S[i - 1] + Q32;

		A = B = i = j = 0;
		v = R24;
		if (c_ > v) v = c_;
		v *= 3;

		for (s = 1; s <= v; s++)
		{
			A = S[i] = ROTL(S[i] + A + B, 3);
			B = L[j] = ROTL(L[j] + A + B, A + B);
			i = (i + 1) % R24;
			j = (j + 1) % c_;
		}
	}

	void rc6_block_encrypt(unsigned int *pt, unsigned int *ct)
	{
		unsigned int A, B, C, D, t, u, x;
		int i;

		A = pt[0];
		B = pt[1];
		C = pt[2];
		D = pt[3];
		B += S[0];
		D += S[1];
		for (i = 2; i <= 2 * r; i += 2)
		{
			t = ROTL(B * (2 * B + 1), lgw);
			u = ROTL(D * (2 * D + 1), lgw);
			A = ROTL(A ^ t, u) + S[i];
			C = ROTL(C ^ u, t) + S[i + 1];
			x = A;
			A = B;
			B = C;
			C = D;
			D = x;
		}
		A += S[2 * r + 2];
		C += S[2 * r + 3];
		ct[0] = A;
		ct[1] = B;
		ct[2] = C;
		ct[3] = D;
	}

	void rc6_block_decrypt(unsigned int *ct, unsigned int *pt)
	{
		unsigned int A, B, C, D, t, u, x;
		int i;

		A = ct[0];
		B = ct[1];
		C = ct[2];
		D = ct[3];
		C -= S[2 * r + 3];
		A -= S[2 * r + 2];
		for (i = 2 * r; i >= 2; i -= 2)
		{
			x = D;
			D = C;
			C = B;
			B = A;
			A = x;
			u = ROTL(D * (2 * D + 1), lgw);
			t = ROTL(B * (2 * B + 1), lgw);
			C = ROTR(C - S[i + 1], t) ^ u;
			A = ROTR(A - S[i], u) ^ t;
		}
		D -= S[1];
		B -= S[0];
		pt[0] = A;
		pt[1] = B;
		pt[2] = C;
		pt[3] = D;
	}
}


typedef boost::crc_optimal<32, 0x04C27EB9, 0xFFFFFFFF, 0xFFFFFFFF, true, true>crc_32_self1;
typedef boost::crc_optimal<32, 0x04D5A2B9, 0xFFFFFFFF, 0xFFFFFFFF, true, true>crc_32_self2;
CFixSMC::CFixSMC()
{
	m_pFile = LoadFile();
	assert(m_pFile);


	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)&*m_pFile;
	PIMAGE_NT_HEADERS pNTHdr = (PIMAGE_NT_HEADERS)((PBYTE)&*m_pFile + pDosHdr->e_lfanew);

	// 保存代码段表头指针
	m_pSecText = IMAGE_FIRST_SECTION(pNTHdr);
	// 保存数据段表头指针
	m_pSecData = m_pSecText + 1;

}


CFixSMC::~CFixSMC()
{
}

shared_ptr<char> CFixSMC::LoadFile()
{
	ifstream ifs(m_sFile, ios::binary);
	ifs.seekg(0, ios::end);
	m_dwFileLen = (DWORD)ifs.tellg();
	ifs.seekg(0, ios::beg);
	shared_ptr<char> pFile(new char[m_dwFileLen], default_delete<char[]>());
	ifs.read(&*pFile, m_dwFileLen);
	
	return pFile;
}

DWORD CFixSMC::CalcCodeCRC()
{
	PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)&*m_pFile;
	PIMAGE_NT_HEADERS pNTHdr = (PIMAGE_NT_HEADERS)((PBYTE)&*m_pFile + pDosHdr->e_lfanew);
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNTHdr);
	crc_32_self2 crc32;
	PBYTE pMemCode = (PBYTE)&*m_pFile + pSec->PointerToRawData;
	crc32.process_bytes(pMemCode, pSec->SizeOfRawData);
	return crc32.checksum();
}

// 修复 OnInitialDlg、MoveThere的代码段文件校验部分
void CFixSMC::Fix_Last()
{
	DWORD dwSize = m_fix.st_oid.oid.dwEnd - m_fix.st_oid.oid.dwStart;

	shared_ptr<char> pOriginData(new char[dwSize], default_delete<char[]>());
	memcpy_s(&*pOriginData, dwSize, &*m_pFile + RVA2FileOffset(m_fix.st_oid.oid.dwStart), dwSize);

	// 修改OnInitialDialog中部分代码，置为0xCC
	DWORD dwTmpOffset = RVA2FileOffset(m_fix.st_oid.oid.dwStart);
	for (DWORD dwS = 0; dwS != dwSize; dwS++)
		(&*m_pFile)[dwTmpOffset +dwS] = 0xCC;
	
	m_cntFixs++;
	dwTmpOffset = RVA2FileOffset(m_fix.st_mvt.dwMvt);
	shared_ptr<char> ptrMoveThere(new char[SZ_MoveThere], default_delete<char[]>());
	memcpy_s(&*ptrMoveThere, SZ_MoveThere, &*m_pFile + dwTmpOffset, SZ_MoveThere);

	// 修改MoveThere中的代码，置为0xcc
	for (DWORD i=0; i!= SZ_MoveThere; i++)
		(&*m_pFile)[dwTmpOffset + i] = 0xCC;
	m_cntFixs++;

	// 用crc(OnInitDialog)作为key对LetsGo进行二次加密
	crc_32_self1 crc32;
	crc32.process_bytes(&*m_pFile + RVA2FileOffset(m_fix.st_oid.dwOID), SZ_OnInitDialog);
	DWORD dwCRC32_OID = crc32.checksum();
	EncTarget3((DWORD)&*m_pFile + RVA2FileOffset(m_fix.dwLG), SZ_LetsGo, (PBYTE)&dwCRC32_OID, 4);
	m_cntFixs++;

	// 用crc(Initialize)作为key对MMulEqual进行二次加密 
	crc32.reset();
	crc32.process_bytes(&*m_pFile + RVA2FileOffset(m_fix.st_init.dwInit), SZ_Initialize);
	DWORD dwCRC32_INIT = crc32.checksum();
	EncTarget2((DWORD)&*m_pFile + RVA2FileOffset(m_fix.dwMME), SZ_MMulEqual, (PBYTE)&dwCRC32_INIT, 4);
	m_cntFixs++;

	// crc32(code)作为OnInitDialog的加密key
	DWORD dwCRC = CalcCodeCRC();
	EncTarget1((DWORD)&*pOriginData, dwSize, (PBYTE)&dwCRC, sizeof(dwCRC));

	// 将加密后的数据保存到对应的数据段中
	memcpy_s(&*m_pFile + RVA2FileOffset(m_fix.st_oid.dwDDS), dwSize, (void *)&*pOriginData, dwSize);
	m_cntFixs++;
	
	// crc32(code)作为MoveThere的加密key
	EncTarget3((DWORD)&*ptrMoveThere, SZ_MoveThere, (PBYTE)&dwCRC, sizeof(dwCRC));
	// 将加密后的数据保存到对应的数据段中
	memcpy_s(&*m_pFile + RVA2FileOffset(m_fix.st_mvt.dwDDS), SZ_MoveThere, (void *)&*ptrMoveThere, SZ_MoveThere);
	m_cntFixs++;
}

void CFixSMC::Fix_OnIdle()
{
	byte Key[] = { 0xA3, 0x2D, 0x5C, 0x9D, 0x4E, 0xF7, 0x8B, 0xCF };
	DWORD szSMC = m_fix.st_OnIdle.dwEnd - m_fix.st_OnIdle.dwStart;
	EncTarget2((DWORD)(&*m_pFile + RVA2FileOffset(m_fix.st_OnIdle.dwStart)), szSMC, Key, 8);
	m_cntFixs++;
}

void CFixSMC::Fix_StartGame()
{
	//int smc_in_s = 0x85, smc_in_e = 0x97a;
	DWORD dwSMCIn = m_fix.st_sg.smc_in.dwEnd - m_fix.st_sg.smc_in.dwStart;

	byte Key[] = { 'W', 'e', 'L', 'c', '0','m', 'E', '!' };
	// 加密嵌套SMC
	EncTarget2((DWORD)(&*m_pFile + RVA2FileOffset(m_fix.st_sg.smc_in.dwStart)), dwSMCIn, Key, 8);
	m_cntFixs++;

	crc_32_self1 crc32;
	// OnTimer 的crc值作为密钥
	crc32.process_bytes(&*m_pFile + RVA2FileOffset(m_fix.st_sg.dwTM), SZ_OnTimer);
	DWORD dwKey = crc32.checksum();

	// 再次加密StartGame
	EncTarget1((DWORD)(&*m_pFile + RVA2FileOffset(m_fix.st_sg.dwSG)), SZ_StartGame, (PBYTE)&dwKey, 4);
	m_cntFixs++;
}

void CFixSMC::Fix_Initialize()
{
	byte Key[] = { 0xCC, 0xEB, 0xFA, 0x9D, 0x8E, 0x68, 0x3A, 0x7D };
	DWORD dwSize = m_fix.st_init.Init.dwEnd - m_fix.st_init.Init.dwStart;
	EncTarget1((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_init.Init.dwStart), dwSize, Key, 8);
	m_cntFixs++;

}


void CFixSMC::Fix_MoveThere()
{
	crc_32_self1 crc32;
	DWORD dwSize = m_fix.st_mvt.crc.dwEnd - m_fix.st_mvt.crc.dwStart+1;
	crc32.process_bytes(&*m_pFile + RVA2FileOffset(m_fix.st_mvt.crc.dwStart), dwSize);
	DWORD dwCrc32 = crc32.checksum();
	
	// 修复内部的CRC校验加密
	dwSize = m_fix.st_mvt.mvt2.dwEnd - m_fix.st_mvt.mvt2.dwStart;
	EncTarget3((DWORD)(&*m_pFile + RVA2FileOffset(m_fix.st_mvt.mvt2.dwStart)), dwSize, (PBYTE)&dwCrc32, 4);
	m_cntFixs++;

	byte Key[] = { 'W', 'e', 'L', 'c', '0','m', 'E', '!' };
	EncTarget1((DWORD)(&*m_pFile + RVA2FileOffset(m_fix.st_mvt.dwMvt)), SZ_MoveThere, Key, 8);
	m_cntFixs++;
}


void CFixSMC::Fix_LetsGo()
{
	// 对LetsGo进行第一次加密，后面还需再用crc32(OnInitDialog)值进行二次加密 
	// size == 0x45C
	byte key_mg[] = { 0x20, 0x17, 0x18, 0x99 };
	EncTarget2((DWORD)&*m_pFile + RVA2FileOffset(m_fix.dwLG), SZ_LetsGo, key_mg, 4);
	m_cntFixs++;
}

void  CFixSMC::Fix(st_fix fix)
{
	m_fix = fix;
	Fix_OnIdle();
	Fix_DecEncLetsGoMMul();
	Fix_StartGame();
	Fix_MoveThere();
	Fix_LetsGo();
	Fix_MMulEqual();
	Fix_DecEncStartGame();
	Fix_Initialize();
	Fix_InitGame();
	Fix_Play();
	Fix_Walk();
	Fix_IsGetThrough();
	Fix_Last();
	
	SaveFile();
}

void CFixSMC::Fix_MMulEqual()
{
	// 对MMulEqual进行一次加密，后面还会用crc32(Initialize)进行二次加密
	byte key_mmul[] = { 'K', 'x', 'B', 'w', 'N', 's', };
	
	// size == 0x10D2
	EncTarget3((DWORD)&*m_pFile + RVA2FileOffset(m_fix.dwMME), SZ_MMulEqual, key_mmul, 6);
	m_cntFixs++;
}


void CFixSMC::SaveFile()
{
	ofstream ofs(m_sSaveFile, ios::binary);
	ofs.write(&*m_pFile, m_dwFileLen);
	ofs.close();
}

void CFixSMC::Fix_DecEncStartGame()
{
	BYTE Key_Dec[] = { 0xD1, 0xE3, 0xC8, 0xAE };
	BYTE Key_Enc[] = { 0xCC, 0xAF, 0xD4, 0xE8 };

	DWORD sz_Dec = m_fix.st_DecStartGame.lbDecStartGame.dwEnd - m_fix.st_DecStartGame.lbDecStartGame.dwStart;
	DWORD sz_Enc = m_fix.st_EncStartGame.lbEncStartGame.dwEnd - m_fix.st_EncStartGame.lbEncStartGame.dwStart;
	
	EncTarget3((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_DecStartGame.lbDecStartGame.dwStart), sz_Dec, Key_Dec, 4);
	m_cntFixs++;

	EncTarget3((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_EncStartGame.lbEncStartGame.dwStart), sz_Enc, Key_Enc, 4);
	m_cntFixs++;

	boost::uuids::detail::sha1 SHA1;
	SHA1.process_bytes((void *)((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_HandleWinEvent.dwStart)), SZ_HandleWinEvent);
	// 以HandleWinEvent的SHA值作为解密DecStartGame、EncStartGame的Key.
	byte Key_HandleWinEvent[20] = { 0 };
	SHA1.get_digest((boost::uuids::detail::sha1::digest_type)Key_HandleWinEvent);
	EncTarget1((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_DecStartGame.dwDecStartGame), SZ_DecStartGame, Key_HandleWinEvent, 20);
	EncTarget2((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_EncStartGame.dwEncStartGame), SZ_EncStartGame, Key_HandleWinEvent, 20);
}

void CFixSMC::Fix_DecEncLetsGoMMul()
{ 
	byte Key_Dec_LG[] = { 0x1F, 0x2E, 0x3D, 0x4C, 0x5B, 0x6A };
	byte Key_Enc_LG[] = { 0x6F, 0x5E, 0x4D, 0x3C, 0x2B, 0x1A };
	byte Key_Dec_MM[] = { 0xCF, 0xBE, 0xAD, 0x9C, 0x8B, 0x7E };
	byte Key_Enc_MM[] = { 0x7F, 0x8E, 0x9D, 0xAC, 0xBB, 0xCE };

	DWORD Sz_Dec_LG = m_fix.st_DecLetsGo.dwEnd - m_fix.st_DecLetsGo.dwStart;
	DWORD Sz_Enc_LG = m_fix.st_EncLetsGo.dwEnd - m_fix.st_EncLetsGo.dwStart;
	DWORD Sz_Dec_MM = m_fix.st_DecMMul.dwEnd - m_fix.st_DecMMul.dwStart;
	DWORD Sz_Enc_MM = m_fix.st_EncMMul.dwEnd - m_fix.st_EncMMul.dwStart;


	EncTarget2((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_DecLetsGo.dwStart), Sz_Dec_LG, Key_Dec_LG, 6);
	m_cntFixs++;

	EncTarget2((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_EncLetsGo.dwStart), Sz_Enc_LG, Key_Enc_LG, 6);
	m_cntFixs++;

	EncTarget3((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_DecMMul.dwStart), Sz_Dec_MM, Key_Dec_MM, 6);
	m_cntFixs++;

	EncTarget3((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_EncMMul.dwStart), Sz_Enc_MM, Key_Enc_MM, 6);
	m_cntFixs++;
}


void CFixSMC::Fix_InitGame()
{
	byte Key_SMC[] = { 'G', '0', 'O', 'd', 'L', 'u', 'c', 'k' };
	EncTarget2((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_InitGame.InitGame.dwStart), m_fix.st_InitGame.InitGame.dwEnd- m_fix.st_InitGame.InitGame.dwStart, Key_SMC, 8);
	m_cntFixs++;

	//byte Key_Out[] = { 'G', 'O', '0', 'd', '1', 'u', 'c', 'k' };
	//EncTarget2((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_InitGame.dwInitGame), SZ_InitGame, Key_Out, 8);
	//m_cntFixs++;
	byte IV[16]{ 0 };
	byte aes_key[24]{ 'H', 'e', 'l', '1', 'O', ',', 'w', 'E', 'l', 'C', '0', 'm', 'e', '2', 'N', 't','B','Y','b','w','n','s',':',')' };
	mbedtls_aes_context aes_ctx{ 0 };
	mbedtls_aes_init(&aes_ctx);
	mbedtls_aes_setkey_enc(&aes_ctx, aes_key, 192);
	mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_ENCRYPT, SZ_InitGame, IV,
		(PBYTE)&*m_pFile + RVA2FileOffset(m_fix.st_InitGame.dwInitGame), (PBYTE)&*m_pFile + RVA2FileOffset(m_fix.st_InitGame.dwInitGame));

	mbedtls_aes_free(&aes_ctx);	
	m_cntFixs++;

}


void CFixSMC::Fix_Play()
{
	byte Key_SMC[] = { 'G', 'O', '0', 'd', '1', 'u', 'c', 'k' };
	EncTarget3((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_Play.Play.dwStart), m_fix.st_Play.Play.dwEnd - m_fix.st_Play.Play.dwStart, Key_SMC, 8);
	m_cntFixs++;


	char key_rc6[25] = { 0 };
	string m_strInputL = "r9cOlg1ewH3S08X";
	boost::uuids::detail::sha1 SHA1;
	/**************************/
	//此处的SHA1进行了常数修改：
	//// 修改后的SHA1各常数项
	//h_[0] = 0xB3016745;
	//h_[1] = 0xEFAD98BA;
	//h_[2] = 0xAB8ED8BA;
	//h_[3] = 0x103BDCFE;
	//h_[4] = 0xE1F0C3D7;
	//************************/
	/*************************/
	SHA1.process_bytes(m_strInputL.c_str(), m_strInputL.length());
	SHA1.get_digest(boost::uuids::detail::sha1::digest_type(key_rc6));

	hash<string> hsh;
	crc_32_self1 crc32;

	crc32.process_bytes(m_strInputL.c_str(), m_strInputL.length());
	byte key_play[8] = { 0 };
	*(PDWORD)key_play = hsh(m_strInputL);
	DWORD dwCRC32 = crc32.checksum();
	*(PDWORD)&key_play[4] = dwCRC32;
	*(PDWORD)&key_rc6[20] = dwCRC32;
	
	EncTarget1((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_Play.dwPlay),SZ_Play, key_play, 8);

	RC6::rc6_key_setup((PBYTE)key_rc6, 24);
	int nBlocks = SZ_Play >> 4;
	for (int i = 0; i < nBlocks; i++)
		RC6::rc6_block_encrypt((unsigned int*)((PBYTE)&*m_pFile + RVA2FileOffset(m_fix.st_Play.dwPlay) + (i << 4)), (unsigned int*)((PBYTE)&*m_pFile + RVA2FileOffset(m_fix.st_Play.dwPlay) + (i << 4)));

	m_cntFixs++;
}


void CFixSMC::Fix_Walk()
{
	byte IV1[16]{ 0 };
	byte aes_key1[16]{ 'B', '9', 'W', '2', 'n', '0', 'S', '1', 'N', '7', '8', 'i', 'n', '1', '^', '!' };
	mbedtls_aes_context aes_ctx1;
	mbedtls_aes_init(&aes_ctx1);
	mbedtls_aes_setkey_enc(&aes_ctx1, aes_key1, 128);
	mbedtls_aes_crypt_cfb8(&aes_ctx1, MBEDTLS_AES_ENCRYPT, m_fix.st_Walk.Walk.dwEnd - m_fix.st_Walk.Walk.dwStart,
		IV1, (PBYTE)&*m_pFile + RVA2FileOffset(m_fix.st_Walk.Walk.dwStart), (PBYTE)&*m_pFile + RVA2FileOffset(m_fix.st_Walk.Walk.dwStart));
	mbedtls_aes_free(&aes_ctx1);

	//byte Key_SMC[] = { 'g', '0', 'o', 'D', '1', 'U', 'C', 'K' };
	//EncTarget2((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_Walk.Walk.dwStart),m_fix.st_Walk.Walk.dwEnd - m_fix.st_Walk.Walk.dwStart,	Key_SMC, 8);
	m_cntFixs++;

	byte IV[16]{ 0 };
	byte aes_key[24]{ 'H', 'e', 'l', '1', 'O', ',', 'w', 'E', 'l', 'C', '0', 'm', 'e', '2', 'N', 't','B','Y','b','w','n','s',':',')' };
	mbedtls_aes_context aes_ctx{ 0 };
	mbedtls_aes_init(&aes_ctx);
	mbedtls_aes_setkey_enc(&aes_ctx, aes_key, 192);
	mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_ENCRYPT, SZ_Walk, IV,
		(PBYTE)&*m_pFile + RVA2FileOffset(m_fix.st_Walk.dwWalk), (PBYTE)&*m_pFile + RVA2FileOffset(m_fix.st_Walk.dwWalk));

	mbedtls_aes_free(&aes_ctx);



	//byte key_Out[] = { 'G', 'O', '0', 'd', '1', 'u', 'c', 'k' }; 
	//EncTarget3((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_Walk.dwWalk),	SZ_Walk, key_Out, 8);
	m_cntFixs++;
}


void CFixSMC::Fix_IsGetThrough()
{
	byte aes_key1[24]{ '!', '#', '$', '%', '~', '^', '&', '*', '(', ')', '{', '}', '[', ']', '\\', '?', '<', '>', ':', '\'', '|', '`', ',', '.' };
	byte IV1[16]{ 0 };
	mbedtls_aes_context aes_ctx1;
	mbedtls_aes_init(&aes_ctx1);
	mbedtls_aes_setkey_enc(&aes_ctx1, aes_key1, 192);
	mbedtls_aes_crypt_cfb8(&aes_ctx1, MBEDTLS_AES_ENCRYPT, m_fix.st_IsGetThrough.IsGetThrough.dwEnd - m_fix.st_IsGetThrough.IsGetThrough.dwStart,
		IV1, (PBYTE)&*m_pFile + RVA2FileOffset(m_fix.st_IsGetThrough.IsGetThrough.dwStart), (PBYTE)&*m_pFile + RVA2FileOffset(m_fix.st_IsGetThrough.IsGetThrough.dwStart));
	mbedtls_aes_free(&aes_ctx1);

	//byte Key_SMC[] = { 'G', 'o', '0', 'd', 'l', 'u', 'c', 'k' };
	//EncTarget3((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_IsGetThrough.IsGetThrough.dwStart),m_fix.st_IsGetThrough.IsGetThrough.dwEnd - m_fix.st_IsGetThrough.IsGetThrough.dwStart,	Key_SMC, 8);
	m_cntFixs++;

	byte IV[16]{ 0 };
	byte aes_key[24]{ 'H', 'e', 'l', '1', 'O', ',', 'w', 'E', 'l', 'C', '0', 'm', 'e', '2', 'N', 't','B','Y','b','w','n','s',':',')' };
	mbedtls_aes_context aes_ctx{ 0 };
	mbedtls_aes_init(&aes_ctx);
	mbedtls_aes_setkey_enc(&aes_ctx, aes_key, 192);
	mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_ENCRYPT, SZ_IsGetThrough, IV,
		(PBYTE)&*m_pFile + RVA2FileOffset(m_fix.st_IsGetThrough.dwIsGetThrough), (PBYTE)&*m_pFile + RVA2FileOffset(m_fix.st_IsGetThrough.dwIsGetThrough));

	mbedtls_aes_free(&aes_ctx);

	//byte key_Out[] = { 'G', 'O', '0', 'd', '1', 'u', 'c', 'k' };
	//EncTarget2((DWORD)&*m_pFile + RVA2FileOffset(m_fix.st_IsGetThrough.dwIsGetThrough),	SZ_IsGetThrough, key_Out, 8);
	m_cntFixs++;
}
