#include "stdafx.h"
#include "CopyedUser32.h"
#include <algorithm>
#include <Psapi.h>
#pragma comment(lib, "psapi.lib")

CCopyedUser32::CCopyedUser32()
{
	CopyAndInit();
}


CCopyedUser32::~CCopyedUser32()
{
	if (m_lpBase)
		VirtualFree(m_lpBase, m_szImage, MEM_FREE);
}


// 获取本进程已加载的user32.dll模块句柄
HMODULE CCopyedUser32::GetUser32Module()
{
	using namespace std;
	WORD key[]{ 0xAC12, 0xBD34, 0xCE56, 0xDF78, 0xBC90, 0xDEAB, 0xEBCA, 0xABCD, 0xBFEC, 0xDEFB, 0xFDEB };
	// 对user32.dll的名称进行加密处理
	TCHAR szUser32[]{ L'u' ^ 0xAC12, L's' ^ 0xBD34, L'e' ^ 0xCE56,
		L'r' ^ 0xDF78, L'3' ^ 0xBC90, L'2' ^ 0xDEAB, L'.' ^ 0xEBCA, L'd' ^ 0xABCD, L'l' ^ 0xBFEC, L'l' ^ 0xDEFB, 0xFDEB };
	int cnt = 0;
	for_each(begin(szUser32), end(szUser32), [&](auto& c) {
		c ^= key[cnt++];
	});

	return GetModuleHandle(szUser32);
}


// 搜索输出表中对应函数的地址
FARPROC CCopyedUser32::GetFunctionAddr(LPCSTR lpProcName)
{

	PIMAGE_DOS_HEADER pDOSHeader;
	PIMAGE_NT_HEADERS pNTHeader;
	PIMAGE_EXPORT_DIRECTORY pExportDir;
	LPCSTR *pFunctionName;
	LPCSTR pszFunName;
	LPDWORD pFunction;
	LPWORD pIndex;
	DWORD n;
	FARPROC ret = NULL;
	if ((INVALID_HANDLE_VALUE == hUser32dll) || (NULL == lpProcName))
		goto end;
	
	pDOSHeader = (PIMAGE_DOS_HEADER)hUser32dll;
	if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
		goto end;

	pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pDOSHeader + (DWORD)pDOSHeader->e_lfanew);
	if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
		goto end;
	
	pExportDir = (PIMAGE_EXPORT_DIRECTORY)((DWORD)pDOSHeader + \
		(DWORD)pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	if ((DWORD)pExportDir == (DWORD)pDOSHeader)
		goto end;

	pFunctionName = (LPCSTR *)((DWORD)pExportDir->AddressOfNames + (DWORD)pDOSHeader);
	pFunction = (LPDWORD)((DWORD)pExportDir->AddressOfFunctions + (DWORD)pDOSHeader);
	pIndex = (LPWORD)((DWORD)pExportDir->AddressOfNameOrdinals + (DWORD)pDOSHeader);
	n = pExportDir->NumberOfNames;
	while (n--)
	{
		pszFunName = (LPCSTR)((DWORD)*pFunctionName + (DWORD)pDOSHeader);
		if (strcmp(pszFunName, lpProcName) == 0)
		{
			ret = (FARPROC)(pFunction[*pIndex] + (DWORD)pDOSHeader);
			break;
		}
		pFunctionName++;
		pIndex++;
	}
end:
	return ret;
}


// 初始化，分配内存等
bool CCopyedUser32::CopyAndInit()
{
	MODULEINFO mi{ 0 };
	hUser32dll = GetUser32Module();
	GetModuleInformation(GetCurrentProcess(), hUser32dll, &mi, sizeof(mi));
	m_szImage = mi.SizeOfImage;
	m_lpBase = VirtualAlloc(NULL, mi.SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	
	memset(m_lpBase, 0, mi.SizeOfImage);
	PIMAGE_NT_HEADERS pNTH = (PIMAGE_NT_HEADERS)((DWORD)hUser32dll + ((PIMAGE_DOS_HEADER)hUser32dll)->e_lfanew);
	PIMAGE_SECTION_HEADER pSec = IMAGE_FIRST_SECTION(pNTH);
	WORD wSecs = pNTH->FileHeader.NumberOfSections;
	memcpy(m_lpBase, (const void*)hUser32dll, pNTH->OptionalHeader.SizeOfHeaders);

	for (WORD w = 0; w != wSecs; w++)
		memcpy((PBYTE)m_lpBase + pSec[w].VirtualAddress, (const void*)((DWORD)hUser32dll + pSec[w].VirtualAddress), pSec[w].SizeOfRawData);
	
	return (nullptr != m_lpBase);
}
