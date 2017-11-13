#pragma once
#include <memory>
//#pragma comment(linker, "/SECTION:BWNS520,ERW")
//#pragma code_seg("BWNS520")

/***********************************************/
// 将此类代码放到.text节段更隐蔽一点!!!
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
