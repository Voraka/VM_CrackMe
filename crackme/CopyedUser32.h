#pragma once
class CCopyedUser32
{
public:
	CCopyedUser32();
	~CCopyedUser32();
public:
	// ��ȡ�������Ѽ��ص�user32.dllģ����
	HMODULE GetUser32Module();
	// ����������ж�Ӧ�����ĵ�ַ
	FARPROC GetFunctionAddr(LPCSTR lpProcName);
	// ��ʼ���������ڴ��
	bool CopyAndInit();

private:
	LPVOID m_lpBase = nullptr;
	SIZE_T m_szImage = 0UL;
	HMODULE hUser32dll = NULL;
	
};

