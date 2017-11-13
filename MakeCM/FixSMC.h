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
		DWORD dwOID;			// OnInitialDialog函数头地址
		fix_addr oid;			// OnInitialDialog加密起止
		DWORD dwDDS;		    // 对应的加密机器码所在数据段的地址
	}st_oid;
	struct
	{
		DWORD dwSG;			// StartGame的起始位置
		fix_addr smc_in;	// StartGame中嵌套smc的起止位置
		DWORD dwTM;			// timer的起止位置
	}st_sg;
	
	struct
	{
		fix_addr crc;
		fix_addr mvt2;			// MoveThere
		DWORD dwMvt;			// MoveThere头部地址
		DWORD dwDDS;			// 对应的加密机器码所在数据段的地址
	}st_mvt;
	
	DWORD dwLG;		// LetsGo	
	DWORD dwMME;	// MMulEqual
	
	struct
	{
		fix_addr Init; // Init中需要加密的部分
		DWORD dwInit;	// Init函数首地址
	}st_init;
	

	// 内存核验解码加密LetsGo、MMulEqual
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
		fix_addr InitGame;	//InitGame内部SMC起止
		DWORD dwInitGame;	//InitGame起始位置
	}st_InitGame;
	struct {
		fix_addr Play;	//Play内部SMC起止
		DWORD dwPlay;	//Play起始位置
	}st_Play;
	struct {
		fix_addr Walk;	//Walk内部SMC起止
		DWORD dwWalk;	//Walk起始位置
	}st_Walk;
	struct {
		fix_addr IsGetThrough;	//InitGame内部SMC起止
		DWORD dwIsGetThrough;	//InitGame起始位置
	}st_IsGetThrough;

	fix_addr_size st_HandleWinEvent;		// 记录HandleWinEvent的地址及大小
	fix_addr_size st_DecStartGameOuter;		// DecStartGame起始地址及大小
	fix_addr_size st_EncStartGameOuter;		// EncStartGame起始地址及大小
}st_fix;


class CFixSMC
{
public:
	// 需要进行加密的各函数大小，编译后需要手动更新
	// 加解密时需要使用的各函数体大小
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

	// 保存代码段、数据段表指针
	PIMAGE_SECTION_HEADER m_pSecText, m_pSecData;

	DWORD RVA2FileOffset(DWORD dwRVA)
	{
		// 转换数据段偏移到文件偏移
		if (dwRVA >= m_pSecData->VirtualAddress)
			return  dwRVA - m_pSecData->VirtualAddress + m_pSecData->PointerToRawData;
		else
			return dwRVA - m_pSecText->VirtualAddress + m_pSecText->PointerToRawData;
	}
};

