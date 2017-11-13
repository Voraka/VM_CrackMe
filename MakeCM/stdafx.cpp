// stdafx.cpp : 只包括标准包含文件的源文件
// MakeCM.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <cassert>
// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用

__forceinline void DecTarget1(DWORD dwFn, int nFnLen, byte* key, int nKeyLen)
{
	PBYTE pDec = (PBYTE)dwFn;
	for (int i = 0; i < nFnLen; ++i)
	{
		pDec[i] ^= 0x75;
		pDec[i] = (pDec[i] >> 2) | (pDec[i] << 6);
		pDec[i] = (pDec[i] - key[i%nKeyLen] + 0x100) % 0x100;
		pDec[i] ^= 0xE8;
		pDec[i] = (pDec[i] >> 1) | (pDec[i] << 7);
	}


}

__forceinline void EncTarget1(DWORD dwFn, int nFnLen, byte* key, int nKeyLen)
{
	PBYTE pEnc = (PBYTE)dwFn;
	for (int i = 0; i < nFnLen; ++i)
	{
		pEnc[i] = ((pEnc[i] << 1) | (pEnc[i] >> 7)) ^ 0xE8;
		pEnc[i] = (pEnc[i] + key[i%nKeyLen]) % 0x100;
		pEnc[i] = ((pEnc[i] << 2) | (pEnc[i] >> 6)) ^ 0x75;

	}
}

__forceinline void DecTarget2(DWORD dwFn, int nFnLen, byte* key, int nKeyLen)
{
	PBYTE pDec = (PBYTE)dwFn;
	for (int i = 0; i < nFnLen; ++i)
	{
		pDec[i] ^= 0x6D;
		pDec[i] = (pDec[i] >> 3) | (pDec[i] << 5);
		pDec[i] = (pDec[i] - key[i%nKeyLen] + 0x100) % 0x100;
		pDec[i] ^= 0x9B;
		pDec[i] = (pDec[i] >> 2) | (pDec[i] << 6);
	}


}

__forceinline void EncTarget2(DWORD dwFn, int nFnLen, byte* key, int nKeyLen)
{
	PBYTE pEnc = (PBYTE)dwFn;
	for (int i = 0; i < nFnLen; ++i)
	{
		pEnc[i] = ((pEnc[i] << 2) | (pEnc[i] >> 6)) ^ 0x9B;
		pEnc[i] = (pEnc[i] + key[i%nKeyLen]) % 0x100;
		pEnc[i] = ((pEnc[i] << 3) | (pEnc[i] >> 5)) ^ 0x6D;
	}
}

__forceinline void DecTarget3(DWORD dwFn, int nFnLen, byte* key, int nKeyLen)
{
	PBYTE pDec = (PBYTE)dwFn;
	for (int i = 0; i < nFnLen; ++i)
	{
		pDec[i] ^= 0xAD;
		pDec[i] = (pDec[i] >> 7) | (pDec[i] << 1);
		pDec[i] = (pDec[i] - key[i%nKeyLen] + 0x100) % 0x100;
		pDec[i] ^= 0x7B;
		pDec[i] = (pDec[i] >> 4) | (pDec[i] << 4);
	}


}

__forceinline void EncTarget3(DWORD dwFn, int nFnLen, byte* key, int nKeyLen)
{
	PBYTE pEnc = (PBYTE)dwFn;
	for (int i = 0; i < nFnLen; ++i)
	{
		pEnc[i] = ((pEnc[i] << 4) | (pEnc[i] >> 4)) ^ 0x7B;
		pEnc[i] = (pEnc[i] + key[i%nKeyLen]) % 0x100;
	 	pEnc[i] = (pEnc[i] << 7) | (pEnc[i] >> 1);
		pEnc[i] ^= 0xAD;
	}
}

/****************************** Base16 Encoding ******************************/
static const size_t BASE16_ENCODE_INPUT = 1;
static const size_t BASE16_ENCODE_OUTPUT = 2;
static const char BASE16_ENCODE_TABLE[] = { 'C', '1', '9', '2', '0', 'B', '7', '3', '4', 'F', '8', '5', 'E', 'D', 'A', '6' };

__forceinline size_t base16EncodeGetLength(size_t size)
{
	/*
	* output 2 bytes for every 1 input:
	*                1
	* 1 = ----1111 = 1111----
	* 2 = ----1111 = ----1111
	*/

	return (((size + BASE16_ENCODE_INPUT - 1) / BASE16_ENCODE_INPUT) * BASE16_ENCODE_OUTPUT) + 1; /*plus terminator*/
}

__forceinline size_t base16Encode(char* dest, const void* src, size_t size)
{
	if (dest && src)
	{
		unsigned char* pSrc = (unsigned char*)src;
		size_t dwSrcSize = size;
		size_t dwDestSize = 0;
		unsigned char ch;

		while (dwSrcSize >= 1)
		{
			/* 1 input */
			ch = *pSrc++;
			dwSrcSize -= BASE16_ENCODE_INPUT;

			/* 2 outputs */
			*dest++ = BASE16_ENCODE_TABLE[(unsigned char)(ch & 0xf0) >> 4];
			*dest++ = BASE16_ENCODE_TABLE[(ch & 0x0f)];
			dwDestSize += BASE16_ENCODE_OUTPUT;
		}
		*dest++ = '\x0'; /*append terminator*/

		return dwDestSize;
	}
	else
		return 0; /*ERROR - null pointer*/
}

/****************************** Base32 Encoding ******************************/

static const size_t BASE32_ENCODE_INPUT = 5;
static const size_t BASE32_ENCODE_OUTPUT = 8;
static const char BASE32_ENCODE_TABLE[] = { 'M', 'B', 'J', 'Y', 'A', '3', 'Z', '7', '5', 'P', 'S', '4', '=', 'N', 'X', 'Q', '2', 'D', 'U', 'R', 'I', 'T', 'W', 'K', '6', 'H', 'G', 'L', 'F', 'E', 'C', 'V', 'O' };

__forceinline size_t base32EncodeGetLength(size_t size)
{
	/*
	* output 8 bytes for every 5 input:
	*                1        2        3        4        5
	* 1 = ---11111 = 11111---
	* 2 = ---111XX = -----111 XX------
	* 3 = ---11111 =          --11111-
	* 4 = ---1XXXX =          -------1 XXXX----
	* 5 = ---1111X =                   ----1111 X-------
	* 6 = ---11111 =                            -11111--
	* 7 = ---11XXX =                            ------11 XXX-----
	* 8 = ---11111 =                                     ---11111
	*/

	return (((size + BASE32_ENCODE_INPUT - 1) / BASE32_ENCODE_INPUT) * BASE32_ENCODE_OUTPUT) + 1; /*plus terminator*/
}

__forceinline size_t base32Encode(char* dest, const void* src, size_t size)
{
	if (dest && src)
	{
		unsigned char* pSrc = (unsigned char*)src;
		size_t dwSrcSize = size;
		size_t dwDestSize = 0;
		size_t dwBlockSize;
		unsigned char n1, n2, n3, n4, n5, n6, n7, n8;

		while (dwSrcSize >= 1)
		{
			/* Encode inputs */
			dwBlockSize = (dwSrcSize < BASE32_ENCODE_INPUT ? dwSrcSize : BASE32_ENCODE_INPUT);
			n1 = n2 = n3 = n4 = n5 = n6 = n7 = n8 = 0;
			switch (dwBlockSize)
			{
			case 5:
				n8 = (pSrc[4] & 0x1f);
				n7 = ((unsigned char)(pSrc[4] & 0xe0) >> 5);
			case 4:
				n7 |= ((unsigned char)(pSrc[3] & 0x03) << 3);
				n6 = ((unsigned char)(pSrc[3] & 0x7c) >> 2);
				n5 = ((unsigned char)(pSrc[3] & 0x80) >> 7);
			case 3:
				n5 |= ((unsigned char)(pSrc[2] & 0x0f) << 1);
				n4 = ((unsigned char)(pSrc[2] & 0xf0) >> 4);
			case 2:
				n4 |= ((unsigned char)(pSrc[1] & 0x01) << 4);
				n3 = ((unsigned char)(pSrc[1] & 0x3e) >> 1);
				n2 = ((unsigned char)(pSrc[1] & 0xc0) >> 6);
			case 1:
				n2 |= ((unsigned char)(pSrc[0] & 0x07) << 2);
				n1 = ((unsigned char)(pSrc[0] & 0xf8) >> 3);
				break;

			default:
				assert(0);
			}
			pSrc += dwBlockSize;
			dwSrcSize -= dwBlockSize;

			/* Validate */
			assert(n1 <= 31);
			assert(n2 <= 31);
			assert(n3 <= 31);
			assert(n4 <= 31);
			assert(n5 <= 31);
			assert(n6 <= 31);
			assert(n7 <= 31);
			assert(n8 <= 31);

			/* Padding */
			switch (dwBlockSize)
			{
			case 1: n3 = n4 = 32;
			case 2: n5 = 32;
			case 3: n6 = n7 = 32;
			case 4: n8 = 32;
			case 5:
				break;

			default:
				assert(0);
			}

			/* 8 outputs */
			*dest++ = BASE32_ENCODE_TABLE[n1];
			*dest++ = BASE32_ENCODE_TABLE[n2];
			*dest++ = BASE32_ENCODE_TABLE[n3];
			*dest++ = BASE32_ENCODE_TABLE[n4];
			*dest++ = BASE32_ENCODE_TABLE[n5];
			*dest++ = BASE32_ENCODE_TABLE[n6];
			*dest++ = BASE32_ENCODE_TABLE[n7];
			*dest++ = BASE32_ENCODE_TABLE[n8];
			dwDestSize += BASE32_ENCODE_OUTPUT;
		}
		*dest++ = '\x0'; /*append terminator*/

		return dwDestSize;
	}
	else
		return 0; /*ERROR - null pointer*/
}

/****************************** Base64 Encoding ******************************/

static const size_t BASE64_ENCODE_INPUT = 3;
static const size_t BASE64_ENCODE_OUTPUT = 4;
static const char BASE64_ENCODE_TABLE[] = { 'M', 'B', '4', 'x', 'A', 'b', 'k', 'f', 'd', 't', 'S', 'u', 'g', 'o', '+', 'z', 'a', '2', 'w', 'R', 'I', 'v', 'W', 'i', 'e', 'H', 'G', 'L', '=', 'E', 'C', '0', 'q', 'j', 'y', '6', 'Z', 'D', 'r', 'Q', 'm', 'l', 'O', 'N', 'Y', 'P', '1', 'T', 'U', 's', '/', 'h', 'V', 'c', 'p', '8', 'J', 'n', '7', '9', '5', '3', 'X', 'K', 'F' };

__forceinline size_t base64EncodeGetLength(size_t size)
{
	/*
	* output 4 bytes for every 3 input:
	*                1        2        3
	* 1 = --111111 = 111111--
	* 2 = --11XXXX = ------11 XXXX----
	* 3 = --1111XX =          ----1111 XX------
	* 4 = --111111 =                   --111111
	*/

	return (((size + BASE64_ENCODE_INPUT - 1) / BASE64_ENCODE_INPUT) * BASE64_ENCODE_OUTPUT) + 1; /*plus terminator*/
}

__forceinline size_t base64Encode(char* dest, const void* src, size_t size)
{
	if (dest && src)
	{
		unsigned char* pSrc = (unsigned char*)src;
		size_t dwSrcSize = size;
		size_t dwDestSize = 0;
		size_t dwBlockSize = 0;
		unsigned char n1, n2, n3, n4;

		while (dwSrcSize >= 1)
		{
			/* Encode inputs */
			dwBlockSize = (dwSrcSize < BASE64_ENCODE_INPUT ? dwSrcSize : BASE64_ENCODE_INPUT);
			n1 = n2 = n3 = n4 = 0;
			switch (dwBlockSize)
			{
			case 3:
				n4 = (unsigned char)(pSrc[2] & 0x3f);
				n3 = ((unsigned char)(pSrc[2] & 0xc0) >> 6);
			case 2:
				n3 |= ((unsigned char)(pSrc[1] & 0x0f) << 2);
				n2 = ((unsigned char)(pSrc[1] & 0xf0) >> 4);
			case 1:
				n2 |= ((unsigned char)(pSrc[0] & 0x03) << 4);
				n1 = ((unsigned char)(pSrc[0] & 0xfc) >> 2);
				break;

			default:
				assert(0);
			}
			pSrc += dwBlockSize;
			dwSrcSize -= dwBlockSize;

			/* Validate */
			assert(n1 <= 63);
			assert(n2 <= 63);
			assert(n3 <= 63);
			assert(n4 <= 63);

			/* Padding */
			switch (dwBlockSize)
			{
			case 1: n3 = 64;
			case 2: n4 = 64;
			case 3:
				break;

			default:
				assert(0);
			}

			/* 4 outputs */
			*dest++ = BASE64_ENCODE_TABLE[n1];
			*dest++ = BASE64_ENCODE_TABLE[n2];
			*dest++ = BASE64_ENCODE_TABLE[n3];
			*dest++ = BASE64_ENCODE_TABLE[n4];
			dwDestSize += BASE64_ENCODE_OUTPUT;
		}
		*dest++ = '\x0'; /*append terminator*/

		return dwDestSize;
	}
	else
		return 0; /*ERROR - null pointer*/
}

/****************************** Base16 Decoding ******************************/

static const size_t BASE16_DECODE_INPUT = 2;
static const size_t BASE16_DECODE_OUTPUT = 1;
static const unsigned char BASE16_DECODE_MAX = 15;
static const unsigned char BASE16_DECODE_TABLE[0x80] = {
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 4, 1, 3, 7, 8, 11, 15, 6, 10, 2, 255,
	255, 255, 255, 255, 255, 255, 14, 5, 0, 13, 12, 9, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255
};

int base16Validate(const char* src, size_t size)
{
	/*
	* returns 0 if the source is a valid base16 encoding
	*/

	if (size % BASE16_DECODE_INPUT != 0)
		return -1; /*ERROR - size isn't a multiple of 2*/

	for (; size >= 1; --size)
	{
		unsigned char ch = *src++;
		if ((ch >= 0x80) || (BASE16_DECODE_TABLE[ch] > BASE16_DECODE_MAX))
			return -2; /*ERROR - invalid base16 character*/
	}

	return 0; /*OK*/
}

size_t base16DecodeGetLength(size_t size)
{
	/*
	* output 1 byte for every 2 input:
	*                1
	* 1 = ----1111 = 1111----
	* 2 = ----1111 = ----1111
	*/

	if (size % BASE16_DECODE_INPUT == 0)
	{
		return (((size + BASE16_DECODE_INPUT - 1) / BASE16_DECODE_INPUT) * BASE16_DECODE_OUTPUT) + 1; /*plus terminator*/
	}
	else
		return 0; /*ERROR - size isn't a multiple of 2*/
}

size_t base16Decode(void* dest, const char* src, size_t size)
{
	if (dest && src && (size % BASE16_DECODE_INPUT == 0))
	{
		unsigned char* pDest = (unsigned char*)dest;
		size_t dwSrcSize = size;
		size_t dwDestSize = 0;
		unsigned char in1, in2;

		while (dwSrcSize >= 1)
		{
			/* 2 inputs */
			in1 = *src++;
			in2 = *src++;
			dwSrcSize -= BASE16_DECODE_INPUT;

			/* Validate ascii */
			if (in1 >= 0x80 || in2 >= 0x80)
				return 0; /*ERROR - invalid base16 character*/

						  /* Convert ascii to base16 */
			in1 = BASE16_DECODE_TABLE[in1];
			in2 = BASE16_DECODE_TABLE[in2];

			/* Validate base16 */
			if (in1 > BASE16_DECODE_MAX || in2 > BASE16_DECODE_MAX)
				return 0; /*ERROR - invalid base16 character*/

						  /* 1 output */
			*pDest++ = ((in1 << 4) | in2);
			dwDestSize += BASE16_DECODE_OUTPUT;
		}
		*pDest++ = '\x0'; /*append terminator*/

		return dwDestSize;
	}
	else
		return 0; /*ERROR - null pointer, or size isn't a multiple of 2*/
}

/****************************** Base32 Decoding ******************************/

static const size_t BASE32_DECODE_INPUT = 8;
static const size_t BASE32_DECODE_OUTPUT = 5;
static const size_t BASE32_DECODE_MAX_PADDING = 6;
static const unsigned char BASE32_DECODE_MAX = 31;

//ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=
//MBJYA3Z75PS4=NXQ2DURITWK6HGLFECVO

static const unsigned char BASE32_DECODE_TABLE[0x80] = {
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	16, 5, 11, 8, 24, 7, 255, 255, 255, 255, 255, 12,
	255, 255, 255, 4, 1, 30, 17, 29, 28, 26, 25, 20,
	2, 23, 27, 0, 13, 32, 9, 15, 19, 10, 21, 18, 31,
	22, 14, 3, 6, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255
};

int cyoBase32Validate(const char* src, size_t size)
{
	/*
	* returns 0 if the source is a valid base32 encoding
	*/

	if (size % BASE32_DECODE_INPUT != 0)
		return -1; /*ERROR - size isn't a multiple of 8*/

	for (; size >= 1; --size, ++src)
	{
		unsigned char ch = *src;
		if ((ch >= 0x80) || (BASE32_DECODE_TABLE[ch] > BASE32_DECODE_MAX))
			break;
	}
	for (; 1 <= size && size <= BASE32_DECODE_MAX_PADDING; --size, ++src)
	{
		unsigned char ch = *src;
		if ((ch >= 0x80) || (BASE32_DECODE_TABLE[ch] != BASE32_DECODE_MAX + 1))
			break;
	}
	if (size != 0)
		return -2; /*ERROR - invalid base32 character*/

	return 0; /*OK*/
}

size_t base32DecodeGetLength(size_t size)
{
	/*
	* output 5 bytes for every 8 input:
	*                1        2        3        4        5
	* 1 = ---11111 = 11111---
	* 2 = ---111XX = -----111 XX------
	* 3 = ---11111 =          --11111-
	* 4 = ---1XXXX =          -------1 XXXX----
	* 5 = ---1111X =                   ----1111 X-------
	* 6 = ---11111 =                            -11111--
	* 7 = ---11XXX =                            ------11 XXX-----
	* 8 = ---11111 =                                     ---11111
	*/

	if (size % BASE32_DECODE_INPUT == 0)
		return (((size + BASE32_DECODE_INPUT - 1) / BASE32_DECODE_INPUT) * BASE32_DECODE_OUTPUT) + 1; /*plus terminator*/
	else
		return 0; /*ERROR - size isn't a multiple of 8*/
}

size_t base32Decode(void* dest, const char* src, size_t size)
{
	if (dest && src && (size % BASE32_DECODE_INPUT == 0))
	{
		unsigned char* pDest = (unsigned char*)dest;
		size_t dwSrcSize = size;
		size_t dwDestSize = 0;
		unsigned char in1, in2, in3, in4, in5, in6, in7, in8;

		while (dwSrcSize >= 1)
		{
			/* 8 inputs */
			in1 = *src++;
			in2 = *src++;
			in3 = *src++;
			in4 = *src++;
			in5 = *src++;
			in6 = *src++;
			in7 = *src++;
			in8 = *src++;
			dwSrcSize -= BASE32_DECODE_INPUT;

			/* Validate ascii */
			if (in1 >= 0x80 || in2 >= 0x80 || in3 >= 0x80 || in4 >= 0x80
				|| in5 >= 0x80 || in6 >= 0x80 || in7 >= 0x80 || in8 >= 0x80)
				return 0; /*ERROR - invalid base32 character*/

						  /* Convert ascii to base16 */
			in1 = BASE32_DECODE_TABLE[in1];
			in2 = BASE32_DECODE_TABLE[in2];
			in3 = BASE32_DECODE_TABLE[in3];
			in4 = BASE32_DECODE_TABLE[in4];
			in5 = BASE32_DECODE_TABLE[in5];
			in6 = BASE32_DECODE_TABLE[in6];
			in7 = BASE32_DECODE_TABLE[in7];
			in8 = BASE32_DECODE_TABLE[in8];

			/* Validate base32 */
			if (in1 > BASE32_DECODE_MAX || in2 > BASE32_DECODE_MAX)
				return 0; /*ERROR - invalid base32 character*/
						  /*the following can be padding*/
			if ((int)in3 > (int)BASE32_DECODE_MAX + 1 || (int)in4 > (int)BASE32_DECODE_MAX + 1 || (int)in5 > (int)BASE32_DECODE_MAX + 1
				|| (int)in6 > (int)BASE32_DECODE_MAX + 1 || (int)in7 > (int)BASE32_DECODE_MAX + 1 || (int)in8 > (int)BASE32_DECODE_MAX + 1)
				return 0; /*ERROR - invalid base32 character*/

						  /* 5 outputs */
			*pDest++ = ((unsigned char)(in1 & 0x1f) << 3) | ((unsigned char)(in2 & 0x1c) >> 2);
			*pDest++ = ((unsigned char)(in2 & 0x03) << 6) | ((unsigned char)(in3 & 0x1f) << 1) | ((unsigned char)(in4 & 0x10) >> 4);
			*pDest++ = ((unsigned char)(in4 & 0x0f) << 4) | ((unsigned char)(in5 & 0x1e) >> 1);
			*pDest++ = ((unsigned char)(in5 & 0x01) << 7) | ((unsigned char)(in6 & 0x1f) << 2) | ((unsigned char)(in7 & 0x18) >> 3);
			*pDest++ = ((unsigned char)(in7 & 0x07) << 5) | (in8 & 0x1f);
			dwDestSize += BASE32_DECODE_OUTPUT;

			/* Padding */
			if (in8 == BASE32_DECODE_MAX + 1)
			{
				--dwDestSize;
				assert((in7 == BASE32_DECODE_MAX + 1 && in6 == BASE32_DECODE_MAX + 1) || (in7 != BASE32_DECODE_MAX + 1));
				if (in6 == BASE32_DECODE_MAX + 1)
				{
					--dwDestSize;
					if (in5 == BASE32_DECODE_MAX + 1)
					{
						--dwDestSize;
						assert((in4 == BASE32_DECODE_MAX + 1 && in3 == BASE32_DECODE_MAX + 1) || (in4 != BASE32_DECODE_MAX + 1));
						if (in3 == BASE32_DECODE_MAX + 1)
						{
							--dwDestSize;
						}
					}
				}
			}
		}
		*pDest++ = '\x0'; /*append terminator*/

		return dwDestSize;
	}
	else
		return 0; /*ERROR - null pointer, or size isn't a multiple of 8*/
}

/****************************** Base64 Decoding ******************************/

static const size_t BASE64_DECODE_INPUT = 4;
static const size_t BASE64_DECODE_OUTPUT = 3;
static const size_t BASE64_DECODE_MAX_PADDING = 2;
static const unsigned char BASE64_DECODE_MAX = 63;
static const unsigned char BASE64_DECODE_TABLE[0x80] = {
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 14,
	255, 255, 255, 50, 31, 46, 17, 61, 2, 60, 35, 58, 55,
	59, 255, 255, 255, 28, 255, 255, 255, 4, 1, 30, 37, 29,
	64, 26, 25, 20, 56, 63, 27, 0, 43, 42, 45, 39, 19, 10,
	47, 48, 52, 22, 62, 44, 36, 255, 255, 255, 255, 255, 255,
	16, 5, 53, 8, 24, 7, 12, 51, 23, 33, 6, 41, 40, 57, 13, 54,
	32, 38, 49, 9, 11, 21, 18, 3, 34, 15, 255, 255, 255, 255, 255
};

int base64Validate(const char* src, size_t size)
{
	/*
	* returns 0 if the source is a valid base64 encoding
	*/

	if (size % BASE64_DECODE_INPUT != 0)
		return -1; /*ERROR - size isn't a multiple of 4*/

	for (; size >= 1; --size, ++src)
	{
		unsigned char ch = *src;
		if ((ch >= 0x80) || (BASE64_DECODE_TABLE[ch] > BASE64_DECODE_MAX))
			break;
	}
	for (; 1 <= size && size <= BASE64_DECODE_MAX_PADDING; --size, ++src)
	{
		unsigned char ch = *src;
		if ((ch >= 0x80) || (BASE64_DECODE_TABLE[ch] != BASE64_DECODE_MAX + 1))
			break;
	}
	if (size != 0)
		return -2; /*ERROR - invalid base64 character*/

	return 0; /*OK*/
}

size_t base64DecodeGetLength(size_t size)
{
	/*
	* output 3 bytes for every 4 input:
	*                1        2        3
	* 1 = --111111 = 111111--
	* 2 = --11XXXX = ------11 XXXX----
	* 3 = --1111XX =          ----1111 XX------
	* 4 = --111111 =                   --111111
	*/

	if (size % BASE64_DECODE_INPUT == 0)
		return (((size + BASE64_DECODE_INPUT - 1) / BASE64_DECODE_INPUT) * BASE64_DECODE_OUTPUT) + 1; /*plus terminator*/
	else
		return 0; /*ERROR - size isn't a multiple of 4*/
}

size_t base64Decode(void* dest, const char* src, size_t size)
{
	if (dest && src && (size % BASE64_DECODE_INPUT == 0))
	{
		unsigned char* pDest = (unsigned char*)dest;
		size_t dwSrcSize = size;
		size_t dwDestSize = 0;
		unsigned char in1, in2, in3, in4;

		while (dwSrcSize >= 1)
		{
			/* 4 inputs */
			in1 = *src++;
			in2 = *src++;
			in3 = *src++;
			in4 = *src++;
			dwSrcSize -= BASE64_DECODE_INPUT;

			/* Validate ascii */
			if (in1 >= 0x80 || in2 >= 0x80 || in3 >= 0x80 || in4 >= 0x80)
				return 0; /*ERROR - invalid base64 character*/

						  /* Convert ascii to base64 */
			in1 = BASE64_DECODE_TABLE[in1];
			in2 = BASE64_DECODE_TABLE[in2];
			in3 = BASE64_DECODE_TABLE[in3];
			in4 = BASE64_DECODE_TABLE[in4];

			/* Validate base64 */
			if (in1 > BASE64_DECODE_MAX || in2 > BASE64_DECODE_MAX)
				return 0; /*ERROR - invalid base64 character*/
						  /*the following can be padding*/
			if ((int)in3 > (int)BASE64_DECODE_MAX + 1 || (int)in4 > (int)BASE64_DECODE_MAX + 1)
				return 0; /*ERROR - invalid base64 character*/

						  /* 3 outputs */
			*pDest++ = ((unsigned char)(in1 & 0x3f) << 2) | ((unsigned char)(in2 & 0x30) >> 4);
			*pDest++ = ((unsigned char)(in2 & 0x0f) << 4) | ((unsigned char)(in3 & 0x3c) >> 2);
			*pDest++ = ((unsigned char)(in3 & 0x03) << 6) | (in4 & 0x3f);
			dwDestSize += BASE64_DECODE_OUTPUT;

			/* Padding */
			if (in4 == BASE64_DECODE_MAX + 1)
			{
				--dwDestSize;
				if (in3 == BASE64_DECODE_MAX + 1)
				{
					--dwDestSize;
				}
			}
		}
		*pDest++ = '\x0'; /*append terminator*/

		return dwDestSize;
	}
	else
		return 0; /*ERROR - null pointer, or size isn't a multiple of 4*/
}

