#pragma once
#include <memory>
#include <string>
using namespace std;

typedef struct _fix_addr
{
	DWORD dwStart;
	DWORD dwEnd;
} fix_addr;

typedef struct _fix_addr_size
{
	DWORD dwStart;
	DWORD dwSize;
}fix_addr_size;

typedef struct _st_fix
{
	struct
	{
		DWORD dwOID;			// OnInitialDialog����ͷ��ַ
		fix_addr oid;			// OnInitialDialog������ֹ
		DWORD dwDDS;		    // ��Ӧ�ļ��ܻ������������ݶεĵ�ַ
	}st_oid;
	struct
	{
		DWORD dwSG;			// StartGame����ʼλ��
		fix_addr smc_in;	// StartGame��Ƕ��smc����ֹλ��
		DWORD dwTM;			// timer����ֹλ��
	}st_sg;
	
	struct
	{
		fix_addr crc;
		fix_addr mvt2;			// MoveThere
		DWORD dwMvt;			// MoveThereͷ����ַ
		DWORD dwDDS;			// ��Ӧ�ļ��ܻ������������ݶεĵ�ַ
	}st_mvt;
	
	DWORD dwLG;		// LetsGo	
	DWORD dwMME;	// MMulEqual
	
	struct
	{
		fix_addr Init; // Init����Ҫ���ܵĲ���
		DWORD dwInit;	// Init�����׵�ַ
	}st_init;
	

	// �ڴ����������LetsGo��MMulEqual
	fix_addr st_DecLetsGo;
	fix_addr st_EncLetsGo;
	fix_addr st_DecMMul;
	fix_addr st_EncMMul;
	fix_addr st_OnIdle;
	struct {
		fix_addr lbDecStartGame;
		DWORD dwDecStartGame;
	}st_DecStartGame;
	struct {
		fix_addr lbEncStartGame;
		DWORD dwEncStartGame;
	}st_EncStartGame;	
	struct {
		fix_addr InitGame;	//InitGame�ڲ�SMC��ֹ
		DWORD dwInitGame;	//InitGame��ʼλ��
	}st_InitGame;
	struct {
		fix_addr Play;	//Play�ڲ�SMC��ֹ
		DWORD dwPlay;	//Play��ʼλ��
	}st_Play;
	struct {
		fix_addr Walk;	//Walk�ڲ�SMC��ֹ
		DWORD dwWalk;	//Walk��ʼλ��
	}st_Walk;
	struct {
		fix_addr IsGetThrough;	//InitGame�ڲ�SMC��ֹ
		DWORD dwIsGetThrough;	//InitGame��ʼλ��
	}st_IsGetThrough;

	fix_addr_size st_HandleWinEvent;		// ��¼HandleWinEvent�ĵ�ַ����С
	fix_addr_size st_DecStartGameOuter;		// DecStartGame��ʼ��ַ����С
	fix_addr_size st_EncStartGameOuter;		// EncStartGame��ʼ��ַ����С
}st_fix;


class CFixSMC
{
public:
	// ��Ҫ���м��ܵĸ�������С���������Ҫ�ֶ�����
	// �ӽ���ʱ��Ҫʹ�õĸ��������С
	enum E_SIZE_FUNC : DWORD{
		SZ_OnInitDialog = 0x4D4,
		SZ_StartGame = 0xDC2,
		SZ_OnTimer = 0x19D,
		SZ_LetsGo = 0x68B,
		SZ_MMulEqual = 0x10F2,
		SZ_MoveThere = 0x1109,
		SZ_Initialize = 0x298,
		SZ_DecStartGame = 0xF7,
		SZ_EncStartGame = 0xF7,
		SZ_InitGame = 0xDE7,
		SZ_Play = 0x6E7,
		SZ_Walk = 0x571,
		SZ_IsGetThrough = 0x1A1,
		SZ_HandleWinEvent = 0x4A
	};

	CFixSMC();
	~CFixSMC();
	int GetFixCount() const { return m_cntFixs; }
	void Fix(st_fix fix);
protected:
	void Fix_MoveThere();
	void Fix_LetsGo();
	void Fix_MMulEqual();
	void Fix_Last();
	void Fix_StartGame();
	void Fix_OnIdle();
	void Fix_Initialize();
	void Fix_DecEncStartGame();
	shared_ptr<char> LoadFile();
	DWORD CalcCodeCRC();
	void SaveFile();

private:
	string m_sFile = "E:\\new_crackme\\crackme\\Release\\crackme.exe";
	string m_sSaveFile = "E:\\new_crackme\\crackme\\Release\\fix_crackme.exe";
	st_fix m_fix;
	shared_ptr<char> m_pFile;
	DWORD m_dwFileLen;
	int m_cntFixs = 0;

public:
	void Fix_DecEncLetsGoMMul();
	void Fix_InitGame();
	void Fix_Play();
	void Fix_Walk();
	void Fix_IsGetThrough();

	// �������Ρ����ݶα�ָ��
	PIMAGE_SECTION_HEADER m_pSecText, m_pSecData;

	DWORD RVA2FileOffset(DWORD dwRVA)
	{
		// ת�����ݶ�ƫ�Ƶ��ļ�ƫ��
		if (dwRVA >= m_pSecData->VirtualAddress)
			return  dwRVA - m_pSecData->VirtualAddress + m_pSecData->PointerToRawData;
		else
			return dwRVA - m_pSecText->VirtualAddress + m_pSecText->PointerToRawData;
	}
};

