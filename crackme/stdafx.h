// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0500

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#include <assert.h>
#include <atlbase.h>
#include <atltypes.h>
#include "SimpleVM.h"

extern SVM::CSimpleVM g_svm;

typedef struct _Path {
	CPoint pt_fr;  // 起点
	CPoint pt_to;  // 终点
	bool operator ==(_Path& oth)
	{
		return (pt_fr == oth.pt_fr && pt_to == oth.pt_to);
	}
}Path, *pPath;

// 对指定函数地址进行解密处理
extern void DecTarget1(DWORD dwFn, DWORD nFnLen, byte* key, DWORD nKeyLen);
// 对指定函数地址进行加密处理
extern void EncTarget1(DWORD dwFn, DWORD nFnLen, byte* key, DWORD nKeyLen);
// 对指定函数地址进行解密处理
extern void DecTarget2(DWORD dwFn, DWORD nFnLen, byte* key, DWORD nKeyLen);
// 对指定函数地址进行加密处理
extern void EncTarget2(DWORD dwFn, DWORD nFnLen, byte* key, DWORD nKeyLen);
// 对指定函数地址进行解密处理
extern void DecTarget3(DWORD dwFn, DWORD nFnLen, byte* key, DWORD nKeyLen);
// 对指定函数地址进行加密处理
extern void EncTarget3(DWORD dwFn, DWORD nFnLen, byte* key, DWORD nKeyLen);

extern	const size_t BASE16_ENCODE_INPUT; // = 1;
extern const size_t BASE16_ENCODE_OUTPUT; // = 2;
extern const char BASE16_ENCODE_TABLE[]; // = { 'C', '1', '9', '2', '0', 'B', '7', '3', '4', 'F', '8', '5', 'E', 'D', 'A', '6' };
extern size_t base16EncodeGetLength(size_t size);
extern size_t base16Encode(char* dest, const void* src, size_t size);

extern const size_t BASE32_ENCODE_INPUT; 
extern const size_t BASE32_ENCODE_OUTPUT;
extern const char BASE32_ENCODE_TABLE[];
extern size_t base32EncodeGetLength(size_t size);
extern size_t base32Encode(char* dest, const void* src, size_t size);

/****************************** Base64 Encoding ******************************/

extern const size_t BASE64_ENCODE_INPUT;
extern const size_t BASE64_ENCODE_OUTPUT;
extern const char BASE64_ENCODE_TABLE[];

extern size_t base64EncodeGetLength(size_t size);
extern size_t base64Encode(char* dest, const void* src, size_t size);

///****************************** Base16 Decoding ******************************/
//extern const size_t BASE16_DECODE_INPUT; 
//extern const size_t BASE16_DECODE_OUTPUT;
//extern const unsigned char BASE16_DECODE_MAX;
//extern const unsigned char BASE16_DECODE_TABLE[0x80];
//extern int base16Validate(const char* src, size_t size);
//extern size_t base16DecodeGetLength(size_t size);
//extern size_t base16Decode(void* dest, const char* src, size_t size);
///****************************** Base32 Decoding ******************************/
//
//extern const size_t BASE32_DECODE_INPUT;
//extern const size_t BASE32_DECODE_OUTPUT;
//extern const size_t BASE32_DECODE_MAX_PADDING;
//extern const unsigned char BASE32_DECODE_MAX;
//
////ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=
////MBJYA3Z75PS4=NXQ2DURITWK6HGLFECVO
//
//extern const unsigned char BASE32_DECODE_TABLE[0x80];
//extern int cyoBase32Validate(const char* src, size_t size);
//extern size_t base32DecodeGetLength(size_t size);
//extern size_t base32Decode(void* dest, const char* src, size_t size);
///****************************** Base64 Decoding ******************************/
//
//extern const size_t BASE64_DECODE_INPUT;
//extern const size_t BASE64_DECODE_OUTPUT;
//extern const size_t BASE64_DECODE_MAX_PADDING;
//extern const unsigned char BASE64_DECODE_MAX;
//extern const unsigned char BASE64_DECODE_TABLE[0x80];
//extern int base64Validate(const char* src, size_t size);
//extern size_t base64DecodeGetLength(size_t size);
//extern size_t base64Decode(void* dest, const char* src, size_t size);

extern unsigned char xm[29840];
extern unsigned char xm2[8166];