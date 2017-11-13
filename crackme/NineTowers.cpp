#include "stdafx.h"
#include "NineTowers.h"
#include <memory>
//#include <boost/algorithm/hex.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <mbedtls/aes.h>
#pragma comment(lib, "mbedtls.lib")

CNineTowers::CNineTowers(list<Path>& lstPath)
	: m_lstPath(lstPath)
{
	byte strLeft[] = { 0xE1, 0xD3, 0x87, 0xF5, 0x86, 0xDB, 0xF3, 0xB6 };
	for (int i = 0; i < 8; i++)
		strLeft[i] ^= 0xB6;
	
	m_strLeft = (char*)strLeft;
	byte strRight[] = { 0x27, 0x0E, 0x1F, 0x4C, 0x18, 0x2C, 0x04, 0x4A, 0x6B };
	for (int i = 0; i < 9; i++)
		strRight[i] ^= 0x6B;
	m_strRight = (char*)strRight;
	//FillPath();
}


CNineTowers::~CNineTowers()
{

}


//void CNineTowers::FillPath()
//{
//	m_lstPath.push_back({ CPoint(8215,-6537), CPoint(8510, -6586) });
//	m_lstPath.push_back({ CPoint(8510,-6586), CPoint(8885, -7149) });
//	m_lstPath.push_back({ CPoint(8885,-7149), CPoint(8952, -6806) });
//	m_lstPath.push_back({ CPoint(8952,-6806), CPoint(9327, -7369) });
//	m_lstPath.push_back({ CPoint(9327,-7369), CPoint(9622, -7418) });
//	m_lstPath.push_back({ CPoint(9622,-7418), CPoint(10189, -7113) });
//	m_lstPath.push_back({ CPoint(10189,-7113), CPoint(10484, -7162) });
//	m_lstPath.push_back({ CPoint(10484,-7162), CPoint(10193, -7107) });
//	m_lstPath.push_back({ CPoint(10193,-7107), CPoint(9902, -7052) });
//	m_lstPath.push_back({ CPoint(9902,-7052), CPoint(10469, -6747) });
//	m_lstPath.push_back({ CPoint(10469,-6747), CPoint(10764, -6796) });
//	m_lstPath.push_back({ CPoint(10764,-6796), CPoint(10805, -7123) });
//	m_lstPath.push_back({ CPoint(10805,-7123), CPoint(10494, -7449) });
//	m_lstPath.push_back({ CPoint(10494,-7449), CPoint(10869, -8012) });
//	m_lstPath.push_back({ CPoint(10869,-8012), CPoint(11436, -7707) });
//	m_lstPath.push_back({ CPoint(11436,-7707), CPoint(11125, -8033) });
//	m_lstPath.push_back({ CPoint(11125,-8033), CPoint(11692, -7728) });
//	m_lstPath.push_back({ CPoint(11692,-7728), CPoint(11733, -8055) });
//	m_lstPath.push_back({ CPoint(11733,-8055), CPoint(12028, -8104) });
//	m_lstPath.push_back({ CPoint(12028,-8104), CPoint(11733, -7989) });
//	m_lstPath.push_back({ CPoint(11733,-7989), CPoint(12028, -8038) });
//	m_lstPath.push_back({ CPoint(12028,-8038), CPoint(12595, -7733) });
//	m_lstPath.push_back({ CPoint(12595,-7733), CPoint(12890, -7782) });
//	m_lstPath.push_back({ CPoint(12890,-7782), CPoint(12957, -7439) });
//	m_lstPath.push_back({ CPoint(12957,-7439), CPoint(13024, -7096) });
//	m_lstPath.push_back({ CPoint(13024,-7096), CPoint(12713, -7422) });
//	m_lstPath.push_back({ CPoint(12713,-7422), CPoint(12402, -7748) });
//	m_lstPath.push_back({ CPoint(12402,-7748), CPoint(12969, -7443) });
//	m_lstPath.push_back({ CPoint(12969,-7443), CPoint(12658, -7769) });
//	m_lstPath.push_back({ CPoint(12658,-7769), CPoint(12699, -8096) });
//	m_lstPath.push_back({ CPoint(12699,-8096), CPoint(12408, -8041) });
//	m_lstPath.push_back({ CPoint(12408,-8041), CPoint(12783, -8604) });
//	m_lstPath.push_back({ CPoint(12783,-8604), CPoint(13350, -8299) });
//	m_lstPath.push_back({ CPoint(13350,-8299), CPoint(13391, -8626) });
//	m_lstPath.push_back({ CPoint(13391,-8626), CPoint(13958, -8321) });
//	m_lstPath.push_back({ CPoint(13958,-8321), CPoint(13999, -8648) });
//	m_lstPath.push_back({ CPoint(13999,-8648), CPoint(13708, -8593) });
//	m_lstPath.push_back({ CPoint(13708,-8593), CPoint(13397, -8919) });
//	m_lstPath.push_back({ CPoint(13397,-8919), CPoint(13692, -8968) });
//	m_lstPath.push_back({ CPoint(13692,-8968), CPoint(14259, -8663) });
//	m_lstPath.push_back({ CPoint(14259,-8663), CPoint(14554, -8712) });
//	m_lstPath.push_back({ CPoint(14554,-8712), CPoint(14243, -9038) });
//	m_lstPath.push_back({ CPoint(14243,-9038), CPoint(14310, -8695) });
//	m_lstPath.push_back({ CPoint(14310,-8695), CPoint(14377, -8352) });
//	m_lstPath.push_back({ CPoint(14377,-8352), CPoint(14444, -8009) });
//	m_lstPath.push_back({ CPoint(14444,-8009), CPoint(14153, -7954) });
//	m_lstPath.push_back({ CPoint(14153,-7954), CPoint(14220, -7611) });
//	m_lstPath.push_back({ CPoint(14220,-7611), CPoint(14261, -7938) });
//	m_lstPath.push_back({ CPoint(14261,-7938), CPoint(14556, -7987) });
//	m_lstPath.push_back({ CPoint(14556,-7987), CPoint(14851, -8036) });
//	m_lstPath.push_back({ CPoint(14851,-8036), CPoint(15418, -7731) });
//	m_lstPath.push_back({ CPoint(15418,-7731), CPoint(15713, -7780) });
//	m_lstPath.push_back({ CPoint(15713,-7780), CPoint(15402, -8106) });
//	m_lstPath.push_back({ CPoint(15402,-8106), CPoint(15111, -8051) });
//	m_lstPath.push_back({ CPoint(15111,-8051), CPoint(15678, -7746) });
//	m_lstPath.push_back({ CPoint(15678,-7746), CPoint(15387, -7691) });
//	m_lstPath.push_back({ CPoint(15387,-7691), CPoint(15954, -7386) });
//	m_lstPath.push_back({ CPoint(15954,-7386), CPoint(15663, -7331) });
//	m_lstPath.push_back({ CPoint(15663,-7331), CPoint(16230, -7026) });
//	m_lstPath.push_back({ CPoint(16230,-7026), CPoint(16525, -7075) });
//	m_lstPath.push_back({ CPoint(16525,-7075), CPoint(16214, -7401) });
//	m_lstPath.push_back({ CPoint(16214,-7401), CPoint(16509, -7450) });
//	m_lstPath.push_back({ CPoint(16509,-7450), CPoint(16804, -7499) });
//	m_lstPath.push_back({ CPoint(16804,-7499), CPoint(17371, -7194) });
//}

void CNineTowers::InitGame(string strInput)
{
#ifdef __SMC__
	byte Key[] = { 'G', '0', 'O', 'd', 'L', 'u', 'c', 'k' };
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	DecTarget2(dwStart, dwEnd - dwStart, Key, 8);
__lbEncS:
#endif

	// 扩展成为30个字符
	string strExt = m_strLeft + strInput + m_strRight;
	int nLen = (int)strExt.length();
	// 进行尾首环状异或
	for (int i = nLen - 1; i > 0; i--)
		strExt[i - 1] ^= strExt[i];
	strExt[nLen - 1] ^= strExt[0];  // 最后一元素等于首元素与之异或

	size_t sz32 = base32EncodeGetLength(nLen);
	shared_ptr<char> pb32(new char[sz32], default_delete<char[]>());
	sz32 = base32Encode(&*pb32, strExt.c_str(), nLen);
	hash<size_t> hsh;
	if (hsh(sz32) == 0x53e1c845)
	{
		int xchg[] =
		{
			0x10, 0x17, 0x02, 0x14, 0x03, 0x26, 0x2A, 0x00, 0x28, 0x0F, 0x05, 0x21, 0x23,
			0x16, 0x06, 0x07, 0x22, 0x2C, 0x1D, 0x29, 0x04, 0x1E, 0x20, 0x27, 0x18, 0x2D,
			0x25, 0x2E, 0x2F, 0x0A, 0x1B, 0x24, 0x11, 0x0C, 0x19, 0x1F, 0x2B, 0x09, 0x15,
			0x12, 0x0D, 0x01, 0x0E, 0x13, 0x08, 0x1C, 0x0B, 0x1A, 0x3F, 0x34, 0x42, 0x36,
			0x5B, 0x50, 0x5F, 0x57, 0x3D, 0x48, 0x3E, 0x5A, 0x40, 0x49, 0x53, 0x46, 0x4C,
			0x56, 0x59, 0x4B, 0x37, 0x33, 0x60, 0x4E, 0x47, 0x30, 0x3A, 0x4D, 0x4A, 0x5D,
			0x41, 0x61, 0x43, 0x35, 0x32, 0x39, 0x54, 0x38, 0x51, 0x3B, 0x4F, 0x62, 0x3C,
			0x58, 0x45, 0x55, 0x63, 0x31, 0x5E, 0x52, 0x5C, 0x44
		};
		strExt.resize(sz32);
		for (int i = 0; i < (int)sz32; i++)
			strExt[xchg[i]] = (&*pb32)[i];

	}


	size_t sz64 = base64EncodeGetLength(sz32);
	shared_ptr<char> pb64(new char[sz64], default_delete<char[]>());
	sz64 = base64Encode(&*pb64, strExt.c_str(), sz32);

	// 进行尾首环状异或
	for (int i = (int)sz64 - 1; i > 0; i--)
		(&*pb64)[i - 1] ^= (&*pb64)[i];
	(&*pb64)[(int)sz64 - 1] ^= (&*pb64)[0];  // 最后一元素等于首元素与之异或
	size_t sz16 = base16EncodeGetLength(sz64);
	shared_ptr<char> pb16(new char[sz16], default_delete<char[]>());

	// 保存最终编码后的字符大小
	m_cPlayer.m_nSteps = (int)base16Encode(&*pb16, &*pb64, sz64);

	// 保存最终编码后的字符
	m_pPlay = pb16;

	// 初始化玩家属性
	m_cPlayer.m_d3Pos.Set(0x2018 - (CPlayer::STEP_X << 1), -0x2017 + CPlayer::STEP_Y, 0x1999);
	m_cPlayer.m_fHealth = 100.0;
#ifdef __SMC__
	__lbEncE :
			 EncTarget2(dwStart, dwEnd - dwStart, Key, 8);
#endif

}


bool CNineTowers::Play(string strInput, size_t szInitGame, size_t szWalk, size_t szIsGetThrough)
{
#ifdef __SMC__
	byte Key[] = { 'G', 'O', '0', 'd', '1', 'u', 'c', 'k' };
	DWORD dwStart, dwEnd;
	__asm
	{
		mov dwStart, offset __lbEncS
		mov dwEnd, offset __lbEncE
	}
	DecTarget3(dwStart, dwEnd - dwStart, Key, 8);
	bool bAesInited = false;
__lbEncS:
	byte IV[16]{ 0 };
	byte aes_key[24]{ 'H', 'e', 'l', '1', 'O', ',', 'w', 'E', 'l', 'C', '0', 'm', 'e', '2', 'N', 't','B','Y','b','w','n','s',':',')' };
	mbedtls_aes_context aes_ctx{ 0 };
	mbedtls_aes_init(&aes_ctx);
	mbedtls_aes_setkey_enc(&aes_ctx, aes_key, 192);
	bAesInited = true;
	void (CNineTowers::*pfnInitGame)(string) = &CNineTowers::InitGame;
	mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_DECRYPT, szInitGame, IV,
		(PBYTE)*(PDWORD)&pfnInitGame, (PBYTE)*(PDWORD)&pfnInitGame);

	//DecTarget2(*(DWORD*)&pfnInitGame, szInitGame, Key, 8);
#endif
	// Yu5l1O0W7xCg4Np
	InitGame(strInput);
#ifdef __SMC__
	memset(IV, 0, 16);
	mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_ENCRYPT, szInitGame, IV,
		(PBYTE)*(PDWORD)&pfnInitGame, (PBYTE)*(PDWORD)&pfnInitGame);
	//EncTarget2(*(DWORD*)&pfnInitGame, szInitGame, Key, 8);
#endif
	byte key[] = {
		0x9B, 0x67, 0x40, 0x9D, 0x95, 0x9F, 0x43, 0x45, 0x95, 0xA9, 0x6E, 0x8B, 0x47,
		0xDA, 0x59, 0x9F, 0x8F, 0x6C, 0xA1, 0xBC, 0xA7, 0xB0, 0xB8, 0x4D, 0xAE, 0x5A,
		0xA6, 0xD2, 0xD1, 0x7C, 0x76, 0x8B, 0x88, 0x73, 0xBB, 0x97, 0xA3, 0xF6, 0x81,
		0x40, 0xA4, 0xAB, 0xB1, 0x83, 0x6C, 0x98, 0x53, 0x6D, 0x96, 0x74, 0x9D, 0x56,
		0x82, 0x5A, 0xA2, 0x79, 0x62, 0x94, 0x88, 0xD4, 0x67, 0xBB, 0x72, 0xB6, 0xB4,
		0x42, 0x4A, 0xE9, 0x77, 0x5B, 0xB8, 0x5F, 0x91, 0xCC, 0xB0, 0xEB, 0xB5, 0xAB,
		0xBE, 0x4C,	0x47, 0x73, 0x97, 0x85, 0xDE, 0x8A, 0xB7, 0x95, 0x72, 0xB9, 0xF1,
		0x8C, 0x58, 0xB5, 0x5E, 0x54, 0x48, 0xD5, 0x9E, 0x49, 0x8E, 0x76, 0xA2, 0x79,
		0x9D, 0x4E, 0x64, 0x60, 0x52, 0x7B, 0xA4, 0x8A, 0xBD, 0xAC, 0xF0, 0x83, 0x9D,
		0xB8, 0xDF, 0x49, 0xB1, 0xD2, 0x84, 0x75, 0xBE, 0x56, 0x63, 0x88
	};



	//   ////此处用于生成正确的Key
	//// 上a 下V 左k 右D z轴下 z
	//string str = "akkVVDVDVDDz"
	//	"aDaakkaaDaDDVz"
	//	"VVDVDVVkVVkz"
	//	"aakakakkVkVVDDaz"
	//	"DVVkkz"
	//	"akakaaDDVDDaDDVDVVkz"
	//	"akkVkz"
	//	"kkakaaDaDaaaDDVVVVkVkz"
	//	"VDDVVDDaDDaakkkaaDaz";
	//shared_ptr<char> ptrKey(new char[m_cPlayer.m_nSteps], default_delete<char[]>());
	//for (int i = 0; i < m_cPlayer.m_nSteps; i++)
	//{
	//	str[i] ^= ((&*m_pPlay)[i] - (i + 9));
	//	(&*ptrKey)[m_xchg[i]] = str[i];
	//}
	//
	//for (int i = 0; i < m_cPlayer.m_nSteps; i++)
	//{
	//	ATLTRACE("%s", (boost::format("0x%02X, ") % (0xff & ((&*ptrKey)[i]))).str().c_str());
	//}

	hash<char> hsh;
	int nSteps = m_cPlayer.m_nSteps;
#ifdef __SMC__
	bool (CPlayer::*pfnWalk)(size_t, CNineTowers&) = &CPlayer::Walk;
#endif
	bool bRet = false;
	// 加上此句话的目的是防止后面非法访问内存。
	if (nSteps > (int)sizeof(m_xchg))
#ifdef __SMC__
		goto __lbEncE;
#else
		return false;
#endif
	for (int i = 0; i < nSteps; i++)
	{
#ifdef __SMC__
		memset(IV, 0, 16);
		mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_DECRYPT, szWalk, IV,
			(PBYTE)*(PDWORD)&pfnWalk, (PBYTE)*(PDWORD)&pfnWalk);
		
		//DecTarget3(*(PDWORD)&pfnWalk, szWalk, Key, 8);
#endif
		if (!m_cPlayer.Walk(hsh(((&*m_pPlay)[i] - (static_cast<char>(i) + 9)) ^ key[m_xchg[i]]), *this))
		{
#ifdef __SMC__
			//EncTarget3(*(PDWORD)&pfnWalk, szWalk, Key, 8);
			memset(IV, 0, 16);
			mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_ENCRYPT, szWalk, IV,
				(PBYTE)*(PDWORD)&pfnWalk, (PBYTE)*(PDWORD)&pfnWalk);

#endif
			break;
		}
#ifdef __SMC__
		memset(IV, 0, 16);
		mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_ENCRYPT, szWalk, IV,
			(PBYTE)*(PDWORD)&pfnWalk, (PBYTE)*(PDWORD)&pfnWalk);
		//EncTarget3(*(PDWORD)&pfnWalk, szWalk, Key, 8);
#endif
	}

	//cout << boolalpha<< m_cPlayer.IsGetThrough() << endl;
#ifdef __SMC__
	bool (CPlayer::*pfnIsGetThrough)() = &CPlayer::IsGetThrough;
	memset(IV, 0, 16);
	mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_DECRYPT, szIsGetThrough, IV,
		(PBYTE)*(PDWORD)&pfnIsGetThrough, (PBYTE)*(PDWORD)&pfnIsGetThrough);

	//DecTarget2(*(PDWORD)&pfnIsGetThrough, szIsGetThrough, Key, 8);
#endif
	bRet = m_cPlayer.IsGetThrough();
#ifdef __SMC__
	//EncTarget2(*(PDWORD)&pfnIsGetThrough, szIsGetThrough, Key, 8);
	memset(IV, 0, 16);
	mbedtls_aes_crypt_cfb8(&aes_ctx, MBEDTLS_AES_ENCRYPT, szIsGetThrough, IV,
		(PBYTE)*(PDWORD)&pfnIsGetThrough, (PBYTE)*(PDWORD)&pfnIsGetThrough);
__lbEncE :
		if (bAesInited)
			mbedtls_aes_free(&aes_ctx);
		EncTarget3(dwStart, dwEnd - dwStart, Key, 8);
#endif
	return bRet;
}