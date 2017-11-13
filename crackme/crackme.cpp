// crackme.cpp : main source file for crackme.exe
//

#include "stdafx.h"

#include "resource.h"
#include "MainDlg.h"
#include "ufmod.h"
#include <boost/uuid/sha1.hpp>

void CALLBACK HandleWinEvent(
	HWINEVENTHOOK hook,
	DWORD event, HWND hwnd,
	LONG idObject, LONG idChild,
	DWORD dwEventThread, DWORD dwmsEventTime)
{
	LONG mStyle = GetWindowLongA(hwnd, GWL_STYLE);
	LONG ExtStyle = GetWindowLongA(hwnd, GWL_EXSTYLE);
	if (mStyle == 0x57c70000 && ExtStyle == 0x140)
		ExitProcess(-1);

	if (mStyle == 0x56CF0000 && ExtStyle == 0x140)
		ExitProcess(-1);
}

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);
#ifdef __SMC__
	void (CMainDlg::*pDecStartGame)() = &CMainDlg::DecStartGame;
	boost::uuids::detail::sha1 SHA1;
	void (CALLBACK *pHandleWinEvent)(
		HWINEVENTHOOK hook,
		DWORD event, HWND hwnd,
		LONG idObject, LONG idChild,
		DWORD dwEventThread, DWORD dwmsEventTime) = HandleWinEvent;
	SHA1.process_bytes((PBYTE)pHandleWinEvent, CMainDlg::SZ_HandleWinEvent);
	// 以HandleWinEvent的SHA值作为解密Initialize的Key.
	byte Key_HandleWinEvent[20] = { 0 };
	SHA1.get_digest((boost::uuids::detail::sha1::digest_type)Key_HandleWinEvent);
	DecTarget1(*(PDWORD)&pDecStartGame, CMainDlg::SZ_DecStartGame, Key_HandleWinEvent, 20);
#endif
	CMainDlg dlgMain;
	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);
#ifdef __SMC__
	void (CMainDlg::*pEncStartGame)() = &CMainDlg::EncStartGame;
	DecTarget2(*(PDWORD)&pEncStartGame, CMainDlg::SZ_EncStartGame, Key_HandleWinEvent, 20);
#endif
	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));
	uFMOD_PlaySong((LPVOID)xm, sizeof(xm), XM_MEMORY);
	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls
	
	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	SetWinEventHook(EVENT_MIN, EVENT_MAX, NULL, HandleWinEvent, 0, 0,
		WINEVENT_OUTOFCONTEXT | WINEVENT_SKIPOWNPROCESS);
	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
