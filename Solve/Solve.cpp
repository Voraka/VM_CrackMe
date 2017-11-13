// Solve.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "stdafx.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include <ctime>
#include <boost/crc.hpp>
#include <list>
#include <iomanip>
#include <algorithm>
using namespace std;

int main()
{
	///*
	//const size_t dir_code[] = {
	//myHash('0'),myHash('3'),
	//myHash('2'), myHash('7'),
	//myHash('4'), myHash('A'),
	//myHash('5'), myHash('B'),
	//myHash('6'), myHash('C'),
	//myHash('1'), myHash('F'),
	//myHash('8'), myHash('E'),
	//myHash('9'), myHash('D') };

	//*/
	//中间28字符
	//702EBB1 C799FB6 7ACD2FC AB89FBA


	//342A0C31163EB4C BC80D36327BBCBE14C8C81B060572 712E33C0BFC1CFC3B33C

	// 左13的base16编码
	string s1 = "342A0C31163EB";
	// 右20的base16编码
	string s2 = "712E33C0BFC1CFC3B33C";

	int xch[32] = { 0x0C,  0x01,  0x09,  0x18,  0x00,  0x1B,  0x19,  0x1F,  0x1D,  0x0F,
		0x12,  0x1C,  0x0E,  0x0D,  0x17,  0x10,  0x1A,  0x03,  0x14,  0x11,
		0x08,  0x13,  0x16,  0x0A,  0x1E,  0x07,  0x06,  0x0B,  0x05,  0x04,
		0x02,  0x15 };

	// base64解码
	byte dec_64[100] = { 0 };
	// base32解码
	byte dec_32[100] = { 0 };
	// base16解码
	byte dec_16[100] = { 0 };
	// 编码后字串
	char enc_str[100] = { 0 };
	// 待base32/base16解码部分
	byte to_dec[100] = { 0 };

	//UINT64 dwcnt = 0;
	DWORD dwKeys = 0;
	// 经常用到的数字
	int nblockLen = 32;

	hash<string> hsh;
	// 自定义CRC32函数
	typedef boost::crc_optimal<32, 0x04C27EB9, 0xFFFFFFFF, 0xFFFFFFFF, true, true>crc_32_self;
	crc_32_self crc32;

	cout << "Computing..." << endl;
	DWORD dws = clock();
	lstrcpyA(enc_str, s1.c_str());
	lstrcpyA(enc_str + 44, s2.c_str());

	DWORD dwCrc32 = 0;
	DWORD dwOtherKeys = 0;
	// 中间31位字符
	// 4C BC80D36 327BBCB E14C8C8 1B060572
	for (char i1 : {'4', 'A'})
	for (char i2 : {'6', 'C'})
	for (char i3 : {'5', 'B'})
	for (char i4 : {'6', 'C'})
	for (char i5 : {'8', 'E'})
	for (char i6 : {'0', '3'})
	for (char i7 : {'9', 'D'})
	for (char i8 : {'0', '3'})
	for (char i9 : {'6', 'C'})
	for (char i10 : {'0', '3'})
	for (char i11 : {'2', '7'})
	for (char i12 : {'2', '7'})
	for (char i13 : {'5', 'B'})
	for (char i14 : {'5', 'B'})
	for (char i15 : {'6', 'C'})
	for (char i16 : {'5', 'B'})
	for (char i17 : {'8', 'E'})
	for (char i18 : {'1', 'F'})
	for (char i19 : {'4', 'A'})
	for (char i20 : {'6', 'C'})
	for (char i21 : {'8', 'E'})
	for (char i22 : {'6', 'C'})
	for (char i23 : {'8', 'E'})
	for (char i24 : {'1', 'F'})
	for (char i25 : {'5', 'B'})
	for (char i26 : {'0', '3'})
	for (char i27 : {'6', 'C'})
	for (char i28 : {'0', '3'})
	for (char i29 : {'5', 'B'})
	for (char i30 : {'2', '7'})
	for (char i31 : {'2', '7'})
	{
		//// 循环计数加1
		//dwcnt++;
		// 此处直接赋值，比调用sprintf_s快多了！
		enc_str[13] = i1;
		enc_str[14] = i2;
		enc_str[15] = i3;
		enc_str[16] = i4;
		enc_str[17] = i5;
		enc_str[18] = i6;
		enc_str[19] = i7;
		enc_str[20] = i8;
		enc_str[21] = i9;
		enc_str[22] = i10;
		enc_str[23] = i11;
		enc_str[24] = i12;
		enc_str[25] = i13;
		enc_str[26] = i14;
		enc_str[27] = i15;
		enc_str[28] = i16;
		enc_str[29] = i17;
		enc_str[30] = i18;
		enc_str[31] = i19;
		enc_str[32] = i20;
		enc_str[33] = i21;
		enc_str[34] = i22;
		enc_str[35] = i23;
		enc_str[36] = i24;
		enc_str[37] = i25;
		enc_str[38] = i26;
		enc_str[39] = i27;
		enc_str[40] = i28;
		enc_str[41] = i29;
		enc_str[42] = i30;
		enc_str[43] = i31;
		//sprintf_s(enc_str, 100, "%s%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%s",
		//	s1.c_str(), i1, i2, i3, i4, i5, i6, i7, i8, i9, i10, i11, i12, i13, i14, i15,
		//	i16, i17, i18, i19, i20, i21, i22, i23, i24, i25, i26, i27, i28, i29, s2.c_str());
		// 直接进行64位解码(解码成32个字符）
		size_t sz16 = base16Decode(dec_16, enc_str, 64);
		if (!sz16) continue;
		// 组合成未编码前的字符串			
		dec_16[nblockLen - 1] ^= dec_16[0];
		for (size_t l = 0; l != nblockLen - 1; l++)
			dec_16[l] ^= dec_16[l + 1];

		for (int i = 0; i < nblockLen; i++)
			to_dec[i] = dec_16[xch[i]];

		to_dec[0] ^= to_dec[nblockLen - 1];
		for (size_t l = nblockLen - 1; l != 0; l--)
			to_dec[l] ^= to_dec[l - 1];
		to_dec[nblockLen] = 0;

		// 先进行base32解码（32字符解码成20字符）
		size_t sz32 = base32Decode(dec_32, (char*)to_dec, nblockLen);
		if (!sz32) continue;
		// 再进行base64解码（20字符解码成15字符）
		size_t sz64 = base64Decode(dec_64, (char*)dec_32, 20);
		if (!sz64) continue;
		if (0xF933063C == hsh((char*)dec_64))
		{
			// 自定义CRC32
			crc32.process_bytes((char*)dec_64, 15);
			dwCrc32 = crc32.checksum();
			crc32.reset();
			if (0x5490B744 == dwCrc32)
			{
				// 通过了两个hash的解
				dwKeys++;
				cout << (clock() - dws) / 1000 << " seconds in finding a to_dec=" << (char*)to_dec << ", key=" << (char*)dec_64 << " crc32=" << dwCrc32 << endl;
			}
			else
			{
				// 通过了std::hash，但未通过 crc32的解
				dwOtherKeys++;
				cout << "find a match: to_dec=" << (char*)to_dec << ", key=" << (char*)dec_64 << " ,but does not match crc32. crc32=" << dwCrc32 << endl;
			}
		}
		else
		{
			// 成功解码，但未通过hash函数的解
			cout << (clock() - dws) / 1000 << " seconds used decoding a to_dec=" << (char*)to_dec << ", key=" << (char*)dec_64 << " failed in passing hash func!" << endl;
		}
	}
	cout << dwKeys << " keys to origin input, " << dwOtherKeys << " keys passed std::hash but failed in passing crc32." << endl;
	cout << "Compute finished. Cost " << (clock() - dws) / 1000 << " seconds." << endl;

	getchar();
	return 0;
}
