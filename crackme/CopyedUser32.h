#pragma once
class CCopyedUser32
{
public:
	CCopyedUser32();
	~CCopyedUser32();
public:
	// 获取本进程已加载的user32.dll模块句柄
	HMODULE GetUser32Module();
	// 搜索输出表中对应函数的地址
	FARPROC GetFunctionAddr(LPCSTR lpProcName);
	// 初始化，分配内存等
	bool CopyAndInit();

private:
	LPVOID m_lpBase = nullptr;
	SIZE_T m_szImage = 0UL;
	HMODULE hUser32dll = NULL;
	
};

