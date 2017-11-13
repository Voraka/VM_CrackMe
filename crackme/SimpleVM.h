#pragma once
#include <windows.h>
#include <memory>
namespace SVM
{
	using std::shared_ptr;
#define EAX Stack[EEAX]
#define EBX Stack[EEBX]
#define ECX Stack[EECX]
#define EDX Stack[EEDX]
#define ESP Stack[EESP]
#define ESI Stack[EESI]
#define EDI Stack[EEDI]
#define EIP Stack[EEIP]
#define EBP Stack[EEBP]
#define EZF Stack[EEZF]
#define ESF Stack[EESF]
#define OEP Stack[EOEP]

	class CSimpleVM
	{
	public:
		CSimpleVM();
		~CSimpleVM();
		enum E_DIR{E_ENC1, E_DEC1, E_ENC2, E_DEC2, E_ENC3, E_DEC3};
		enum E_REG
		{
			EEAX = 0xc1,
			EEBX = 0xc2,
			EECX = 0xc3,
			EEDX = 0xc4,
			EESP = 0xc5,
			EESI = 0xc6,
			EEDI = 0xc7,
			EEIP = 0xc8,
			EEBP = 0xc9,	
			EEZF = 0xca,
			EESF = 0xcb,
			EOEP = 0xcc 
		};

		enum E_OPCODE
		{
			EPSH_EAX = 0xbc,
			EPSH_ESI = 0xbd,
			EPSH_EDI = 0xbe,
			EPOP_EAX = 0xbf,
			EPOP_ESI = 0xc0,
			EPOP_EDI = 0xc1,
			EPSH_MEM = 0xc2,
			EINC_EBX = 0xc3,
			ECMP = 0xc4,
			EJGE = 0xc5,
			EXOR = 0xc6,  // ~AB+A~B
			EAND = 0xc7,  
			ENOT = 0xc8,
			EOR = 0xc9,
			EMOD = 0xca,
			EPSH_ECX = 0xcb,
			EPSH_EDX = 0xcc,
			EPOP_ECX = 0xcd,
			EPOP_EDX = 0xce,
			EPSH_EBX = 0xcf,
			EJMP = 0xd0,
			ELDSB = 0xd1,
			ELSHIFT = 0xd2,
			ERSHIFT = 0xd3,
			EPSH_MEM8=0xd4,
			EPOP_ESI_P=0xd5,
			ELDDSB = 0xd7,
			EADD = 0xd8,
			EPSH_MEM16=0xd9,
			ESUB = 0xda
		};

		void Execute();
		void InitSelf(DWORD dwESI, int dwECX, DWORD dwEDI, int dwEDX, E_DIR ed);
	private:
	
		//// 保存跳转表1（6个跳转点）
		//DWORD vJmpTable1[6] = { 0x11111111,0x22222222,0x33333333,0x44444444,0x55555555,0x66666666 };
		//// 保存跳转表2 (31个跳转点）
		//DWORD vJmpTable2[31] = { 
		//	0xaaaaaaaa,0xbbbbbbbb,0x11111111,0x22222222,0x33333333,0x44444444,0x55555555,0x66666666,
		//	0x11111111,0x22222222,0x33333333,0x44444444,0x55555555,0x66666666,0x77777777,0x88888888,
		//	0x11111111,0x22222222,0x33333333,0x44444444,0x55555555,0x66666666,0x77777777,0x88888888,
		//	0x11111111,0x22222222,0x33333333,0x44444444,0x55555555,0x66666666,0x77777777
		//};
		// 定义二重虚化的VMContext
		/*typedef struct _VMCONTEXT
		{
			DWORD _ebx;
			DWORD _edx;
			DWORD _ecx;
			DWORD _eax;
			DWORD _esp;
			DWORD _esi;
			DWORD _edi;
			DWORD _ebp;
			DWORD _eip;
			DWORD _ZF;
			DWORD _SF;
			DWORD _CF;
			DWORD _OF;
		}VMCONTEXT;
		byte VMStack[0x1000] = { 0 };
		VMCONTEXT VMContext = { 0 };
		DWORD VMBegin=0;
		DWORD VMTemp=0;*/

	protected:
		void Push_MEM();
		void Push_EAX();
		void Push_ESI();
		void Push_EDI();
		void Push_ECX();
		void Push_EDX();
		void Push_EBX();
		void Pop_ECX();
		void Pop_EDX();
		void Pop_EAX();
		void Pop_ESI();
		void Pop_EDI();
		void LdSB();
		void LSHIFT();
		void RSHIFT();
		void Push_MEM8();
		void Pop_ESI_P();
		void LdDSB();
		void Add();
		void Inc_EBX();
		void Sub();

		void Push_MEM16();
		void Xor();
		void And();
		void Not();
		void Or();
		void Cmp();
		void Mod();
		void JGE();
		void JMP();
	};


}
