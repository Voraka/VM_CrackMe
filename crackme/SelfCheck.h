#pragma once
#include <memory>
//#pragma comment(linker, "/SECTION:BWNS520,ERW")
//#pragma code_seg("BWNS520")

/***********************************************/
// ���������ŵ�.text�ڶθ�����һ��!!!
/**********************************************/
using namespace std;

class CSelfCheck
{
public:
	CSelfCheck();
	~CSelfCheck();

	DWORD ComputeFileCodeCRC();
protected:
	shared_ptr<char> LoadSelf();

};
//#pragma code_seg()
