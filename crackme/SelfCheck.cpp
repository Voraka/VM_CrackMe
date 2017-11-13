#include "stdafx.h"
#include "SelfCheck.h"
#include <boost/crc.hpp>
#include <algorithm>
typedef boost::crc_optimal<32, 0x04D5A2B9, 0xFFFFFFFF, 0xFFFFFFFF, true, true>crc_32_self;

//#pragma code_seg("BWNS520")
CSelfCheck::CSelfCheck()
{
}


CSelfCheck::~CSelfCheck()
{
}

DWORD CSelfCheck::ComputeFileCodeCRC()
{
	shared_ptr<char> pFile = LoadSelf();
	if (pFile)
	{
		PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)&*pFile;
		PIMAGE_NT_HEADERS pNTHdr = (PIMAGE_NT_HEADERS)((PBYTE)&*pFile + pDosHdr->e_lfanew);
		PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNTHdr);
		crc_32_self crc32;
		PBYTE pMemCode = (PBYTE)&*pFile + pSec->PointerToRawData;
		crc32.process_bytes(pMemCode, pSec->SizeOfRawData);
		return crc32.checksum();
	}

	return 0;
}

shared_ptr<char> CSelfCheck::LoadSelf()
{
	TCHAR szFile[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szFile, MAX_PATH);
	std::shared_ptr<char> pFile(new char[0x100000], std::default_delete<char[]>());
	FILE* fp = nullptr;
	TCHAR szOpen[] = { L'r' ^ 0xADE8, L'b' ^ 0x74EA , 0xFC75 };
	WORD  key[] = { 0xADE8, 0x74EA, 0xFC75 };
	int cnt = 0;
	std::for_each(std::begin(szOpen), std::end(szOpen), [&](TCHAR& c) {
		c ^= key[cnt++];
	});

	_wfopen_s(&fp, szFile, szOpen);
	if (fp)
	{
		std::shared_ptr<char> pFile(new char[0x100000], std::default_delete<char[]>());
		if (pFile)
		{
			size_t sz = fread(&*pFile, 1, 0x100000, fp);
			return pFile;
		}
	}

	return std::shared_ptr<char>();
}

//#pragma code_seg()