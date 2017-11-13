// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO:  在此处引用程序需要的其他头文件

extern const size_t BASE64_ENCODE_INPUT;
extern const size_t BASE64_ENCODE_OUTPUT;
extern const char BASE64_ENCODE_TABLE[];

extern size_t base64EncodeGetLength(size_t size);
extern size_t base64Encode(char* dest, const void* src, size_t size);

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
