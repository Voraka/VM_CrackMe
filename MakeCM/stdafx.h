// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <windows.h>


// TODO:  在此处引用程序需要的其他头文件
// 对指定函数地址进行解密处理
// 对指定函数地址进行解密处理
extern void DecTarget1(DWORD dwFn, int nFnLen, byte* key, int nKeyLen);
// 对指定函数地址进行加密处理
extern void EncTarget1(DWORD dwFn, int nFnLen, byte* key, int nKeyLen);
// 对指定函数地址进行解密处理
extern void DecTarget2(DWORD dwFn, int nFnLen, byte* key, int nKeyLen);
// 对指定函数地址进行加密处理
extern void EncTarget2(DWORD dwFn, int nFnLen, byte* key, int nKeyLen);
// 对指定函数地址进行解密处理
extern void DecTarget3(DWORD dwFn, int nFnLen, byte* key, int nKeyLen);
// 对指定函数地址进行加密处理
extern void EncTarget3(DWORD dwFn, int nFnLen, byte* key, int nKeyLen);

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

/****************************** Base16 Decoding ******************************/
extern const size_t BASE16_DECODE_INPUT; 
extern const size_t BASE16_DECODE_OUTPUT;
extern const unsigned char BASE16_DECODE_MAX;
extern const unsigned char BASE16_DECODE_TABLE[0x80];
extern int base16Validate(const char* src, size_t size);
extern size_t base16DecodeGetLength(size_t size);
extern size_t base16Decode(void* dest, const char* src, size_t size);
/****************************** Base32 Decoding ******************************/

extern const size_t BASE32_DECODE_INPUT;
extern const size_t BASE32_DECODE_OUTPUT;
extern const size_t BASE32_DECODE_MAX_PADDING;
extern const unsigned char BASE32_DECODE_MAX;

//ABCDEFGHIJKLMNOPQRSTUVWXYZ234567=
//MBJYA3Z75PS4=NXQ2DURITWK6HGLFECVO

extern const unsigned char BASE32_DECODE_TABLE[0x80];
extern int cyoBase32Validate(const char* src, size_t size);
extern size_t base32DecodeGetLength(size_t size);
extern size_t base32Decode(void* dest, const char* src, size_t size);
/****************************** Base64 Decoding ******************************/

extern const size_t BASE64_DECODE_INPUT;
extern const size_t BASE64_DECODE_OUTPUT;
extern const size_t BASE64_DECODE_MAX_PADDING;
extern const unsigned char BASE64_DECODE_MAX;
extern const unsigned char BASE64_DECODE_TABLE[0x80];
extern int base64Validate(const char* src, size_t size);
extern size_t base64DecodeGetLength(size_t size);
extern size_t base64Decode(void* dest, const char* src, size_t size);
