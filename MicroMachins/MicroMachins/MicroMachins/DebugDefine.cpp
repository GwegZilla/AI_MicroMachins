#include"stdafx.h"

namespace sft
{
	HHOOK g_hHook;
	HHOOK g_hHookKeyboard;
	bool g_bShiftDown;
	HWND g_hChildWnd;		// MessageBox is a "child"

	static bool	s_bErrorSilent = false;
	static bool	s_bErrorExit = false;
	static bool	s_bErrorLog = true;
	static bool s_bErrorMessageBox = true;

	LogCallback UserLogCallback;
	CoreCallback UserCoreCallback;

	unsigned long DefaultUserCoreCB(CoreCallbackCallType eType, const char* sCond, const char* sFilename, unsigned long iLine, const char* sFunction, const char* sMsg)
	{
		switch (eType)
		{
		case E_ASSERT_CALLBACK:
		{
			printf("Assertion\n");
			printf("[DESCRIPTION]:    %s\n\n[EXPRESSION]:    %s\n[FUNCTION]:    %s\n[FILE]:    %s\n[LINE]:    %i\n\nPress Abort to exit program\nRetry to debug\nIgnore to continue\n\n\n", sMsg, sCond, sFunction, sFilename, iLine);

			// WARNING: THIS MUST BE INITIALIZED BEFORE THE FIRST ASSERT AND THIS IS NOT THE CASE AS IT IS ON OTHER PLATFORMS [DCA]
			//UserLogCallback( Core::E_ALWAYS, sFilename, sFunction, iLine, sMsg );

			return 0;
		}

		case E_ERROR_CALLBACK:
		{
			UserLogCallback(E_ERROR, 0, sFilename, sFunction, iLine, sMsg);
			return 0;
		}
		}
		return 0;
	}

	LRESULT CALLBACK MessageBoxBreakHook(INT iCode, WPARAM wParam, LPARAM lParam)
	{
		// Notification that a window is about to be activated (window handle is wParam)

		if (iCode == HCBT_ACTIVATE)
		{
			g_hChildWnd = (HWND)wParam;

			// Override the button caption

			UINT iResult;

			if (GetDlgItem(g_hChildWnd, IDYES) != NULL)		iResult = SetDlgItemTextA(g_hChildWnd, IDYES, "Ignore");
			if (GetDlgItem(g_hChildWnd, IDNO) != NULL)		iResult = SetDlgItemTextA(g_hChildWnd, IDNO, "Always");

			if (GetDlgItem(g_hChildWnd, IDCANCEL) != NULL)
			{
				if (IsDebuggerPresent())
				{
					iResult = SetDlgItemTextA(g_hChildWnd, IDCANCEL, "BREAK");
				}
				else
				{
					iResult = SetDlgItemTextA(g_hChildWnd, IDCANCEL, "Stop");
				}
			}

			UnhookWindowsHookEx(g_hHook);
		}
		else CallNextHookEx(g_hHook, iCode, wParam, lParam);

		return 0;
	}

	LRESULT CALLBACK MessageBoxBreakHookKeyboard(INT iCode, WPARAM wParam, LPARAM lParam)
	{
		bool bDown = GetAsyncKeyState(VK_SHIFT) != 0;
		if (bDown != g_bShiftDown)
		{
			g_bShiftDown = bDown;
			if (g_hChildWnd != NULL)
			{
				UINT iResult;
				if (GetDlgItem(g_hChildWnd, IDYES) != NULL)		iResult = SetDlgItemTextA(g_hChildWnd, IDYES, g_bShiftDown ? "Callstack" : "Ignore");
			}
		}

		return CallNextHookEx(g_hHook, iCode, wParam, lParam);
	}

	MessageBoxResult	MessageBoxBreak(LPCTSTR sMessage, LPCTSTR sTitle)
	{
		MessageBoxResult eResult = E_MB_RESULT_BREAK;
		if (IsDebuggerPresent())
		{
			//PlayAll::Core::FreezeAllThreadsExceptThis();
		}

		g_bShiftDown = false;
		g_hChildWnd = NULL;
		g_hHook = SetWindowsHookEx(WH_CBT, &MessageBoxBreakHook, 0, GetCurrentThreadId());

		g_hHookKeyboard = SetWindowsHookEx(WH_KEYBOARD, &MessageBoxBreakHookKeyboard, 0, GetCurrentThreadId());
		int iWin32Result = MessageBox(NULL, sMessage, sTitle, MB_YESNOCANCEL | MB_ICONWARNING | MB_TOPMOST);
		UnhookWindowsHookEx(g_hHookKeyboard);

		if (iWin32Result == IDCANCEL)
		{
			if (IsDebuggerPresent() == false)
			{
				TerminateProcess(GetCurrentProcess(), 1);
			}
			eResult = E_MB_RESULT_BREAK;			// Break Program
		}
		else if (iWin32Result == IDYES)
		{
			if (g_bShiftDown)
			{
				StackWatcher::CallStackFrame oFrame;
				//StackWatcher::Get(oFrame);

				std::string sStack = StackWatcher::GetReadableCallStackFromFrame(oFrame);
				//Remove 5 first lines (Kernel Debug functions)
				//PlayAll::Array<std::string> vLines;
				//Tokenize(sStack, "\n", vLines);
				sStack.clear();
				//for ( int iLine = 5; iLine < (int)vLines.size(); ++iLine)
				//{
				//	sStack += vLines[iLine];
				//	sStack += "\n";
				//}

				OpenClipboard(0);
				EmptyClipboard();

				HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, sStack.size());
				if (hg != NULL)
				{
					memcpy(GlobalLock(hg), sStack.c_str(), sStack.size());

					GlobalUnlock(hg);
					SetClipboardData(CF_TEXT, hg);
					CloseClipboard();
					GlobalFree(hg);
				}

				MessageBox(NULL, (LPTSTR)sStack.c_str(), (LPTSTR)"Callstack", MB_OK | MB_TOPMOST);
			}
			eResult = E_MB_RESULT_IGNOREONCE;			// Ignore Once
		}
		else
		{
			eResult = E_MB_RESULT_IGNOREALWAYS;			// Ignore Always
		}
		return eResult;
	}

	void PaPrint(const char* sMessage, ...)
	{
		va_list oArgs;
		va_start(oArgs, sMessage);

		char sBuffer[2048];
		vsprintf_s(sBuffer, sizeof(sBuffer) - 1, sMessage, oArgs);
		::OutputDebugStringA(sBuffer);

		va_end(oArgs);
	}

	void CopyMessageToClipboard(const char* sMessage)
	{
		const size_t iMessageLen = strlen(sMessage) + 1;
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, iMessageLen);
		memcpy(GlobalLock(hMem), sMessage, iMessageLen);
		GlobalUnlock(hMem);

		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hMem);
		CloseClipboard();
	}

#pragma warning(push)
#pragma warning( disable : 4311)
#pragma warning( disable : 4302)
	MessageBoxResult PaErrorHandler(CoreCallbackCallType eError, const char* sExpression, const char* sFile, const int iLine, const char* sFunction, const char* sMessage)
	{
		if (s_bErrorSilent)
		{
			return E_MB_RESULT_IGNOREALWAYS;
		}

		LPCSTR sError = eError == E_ERROR_CALLBACK ? "ERROR" : "ASSERT";
		if (s_bErrorLog)
		{
			if (UserLogCallback)
			{
				UserLogCallback(E_ERROR, 0, sFile, sFunction, iLine, sMessage);
			}
		}

		PaPrint(
			"################ ERROR %d ################\n"
			"[TYPE]:         %s (%d)\n"
			"[DESCRIPTION]:  %s\n"
			"[EXPRESSION]:   %s\n"
			"[LOCATION]:\n%s(%d) : in function <%s>\n\n"	// DONT CHANGE THE FORMAT OF THIS LINE!
			, g_iPaErrorCount, sMessage, (int)eError, sError, sExpression, sFile, iLine, sFunction);

		if (s_bErrorExit)
		{
			TerminateProcess(GetCurrentProcess(), 1);

			return E_MB_RESULT_BREAK;
		}

		if (s_bErrorMessageBox)
		{
			char sMessageBoxText[2048];
			sprintf_s(sMessageBoxText, 2040,
				"[DESCRIPTION]\n%s\n\n"
				"[EXPRESSION]\n%s\n\n"
				"[LOCATION]\n%s(%d)\n\n"
				"[FUNCTION]\n%s\n\n"
				"[THREAD HANDLE:%08x ID:%08x]\n\n"
				"\nIGNORE THE %s? ALWAYS? OR BREAK THE PROGRAM?    \n\n",
				sMessage, sExpression, sFile, iLine, sFunction, (DWORD)GetCurrentThread(), (DWORD)GetCurrentThreadId(), sError);

			if (IsDebuggerPresent() == false)
			{
				CopyMessageToClipboard(sMessageBoxText);
				strcat_s(sMessageBoxText, "\nTHIS MESSAGE IS AUTOMATICALLY COPIED TO THE CLIPBOARD\n\n");
			}

			return MessageBoxBreak((LPCTSTR)sMessageBoxText, eError == E_ERROR_CALLBACK ? (LPTSTR)"ERROR" : (LPTSTR)"ASSERT");
		}

		return E_MB_RESULT_IGNOREONCE;
	}
#pragma warning( pop )

	int PaAssert(const char* sExpression, const char* sFile, const int iLine, const char* sFunction, const char* sMessage, ...)
	{
		// THIS FUNCTION WILL BE MERGED WITH PaError
		va_list oArgs;
		va_start(oArgs, sMessage);
		char sBuffer[1024];
		vsnprintf_s(sBuffer, sizeof(sBuffer), _TRUNCATE, sMessage, oArgs); sBuffer[sizeof(sBuffer) - 1] = 0;

		va_end(oArgs);

		//no message...
		if (sBuffer[0] == '\0')
		{
			sprintf_s(sBuffer, sizeof(sBuffer), "ASSERT: %s at %s(%d)", sExpression, sFile, iLine);
		}

		int retCallback = PaErrorHandler(E_ASSERT_CALLBACK, sExpression, sFile, iLine, sFunction, sBuffer);

		return retCallback;
	}

	/*void WriteLogEntry( unsigned short iMessageId, unsigned short iGroupId, const char* sSourceFile, const char* sFunction, int iSourceLine, const char* sMessage )
	{
	InfoTrackingAPI* pInfoAPI = PlayAll::Core::InfoTrackingAPI::GetInstance();
	if( pInfoAPI != NULL )
	{
	pInfoAPI->AddInfoTrackingString( iMessageId, iGroupId, sMessage );
	}

	if ( !Logger::IsNull() )
	{
	Logger::GetInstance()->Log( (Core::LogLevel)iMessageId, sSourceFile, sFunction, iSourceLine, sMessage );
	}
	}*/

	namespace StackWatcher
	{
		const unsigned short c_iReturnBufferSize = 16384;
		static char s_pReturnBuffer[c_iReturnBufferSize];
		const unsigned short c_iStructNameSize = 512;
		const unsigned short c_iTmpSize = 4096;
		const unsigned short c_iSymbolSize = 1024;
		const unsigned short c_iMessageBoxMaxChar = 2048;

		static bool s_bInit = false;
		void Init()
		{
			if (!s_bInit)
			{
				s_bInit = true;
				DWORD symOptions = SymGetOptions();
				symOptions |= SYMOPT_LOAD_LINES;
				symOptions |= SYMOPT_DEFERRED_LOADS;
				SymSetOptions(symOptions);

				if ((SymInitialize(GetCurrentProcess(), 0, true)) == false)
				{
					printf("SymInitialize returned error : %X\n", GetLastError());
				}
			}
		}
		struct SYMBOL_INFOW_NAME : SYMBOL_INFOW
		{
			char _name[c_iStructNameSize];
		};

		struct IMAGEHLP_LINE_NAME : IMAGEHLP_LINE
		{
			char _name[c_iStructNameSize];
		};

		/*void Get( CallStackFrame& oFrame, int iSkip, void* pContext )
		{
		int index = -iSkip; // Ignore first stacks entries

		oFrame.m_iStackSize = 0;
		memset(&oFrame.m_iStackAddr, 0, sizeof(unsigned long) * PA_KERNEL_CALLSTACK_MAX_DEPTH);

		size_t iPrev;
		size_t iAddr;
		if (pContext != NULL)
		{
		iPrev = ((CONTEXT*)pContext)->Ebp;
		}
		else
		{
		__asm { mov iPrev, ebp }
		}

		while(index < PA_KERNEL_CALLSTACK_MAX_DEPTH)
		{
		iAddr = ((unsigned long *)iPrev)[1];
		if (iAddr == 0)
		{
		break;
		}
		if (index >= 0)
		{
		oFrame.m_iStackAddr[index] = iAddr;
		}
		++index;
		iPrev = ((unsigned long *)iPrev)[0];
		}

		oFrame.m_iStackSize = (unsigned short)index;
		}*/

		const char* GetReadableCallStackFromFrame(CallStackFrame& oFrame)
		{
			Init();

			strcpy_s(s_pReturnBuffer, c_iReturnBufferSize, "");

			char sTmp[c_iTmpSize];

			//const uint16 c_i

			char sSymbolName[c_iSymbolSize];
			char sModuleName[c_iSymbolSize];

			DWORD displacement = 0;
			DWORD64 displacement64 = 0;

			HANDLE process = GetCurrentProcess();

			for (int i = 0; i < oFrame.m_iStackSize && oFrame.m_iStackAddr[i] != 0; ++i)
			{
				size_t addr = oFrame.m_iStackAddr[i];

				SYMBOL_INFOW_NAME symbol;
				symbol.MaxNameLen = c_iStructNameSize;
				symbol.SizeOfStruct = sizeof(SYMBOL_INFOW);
				if (SymFromAddrW(process, addr, &displacement64, (PSYMBOL_INFOW)&symbol))
				{
					size_t iReturnSize;
					wcstombs_s(&iReturnSize, sSymbolName, c_iSymbolSize, symbol.Name, c_iStructNameSize);
				}
				else {
					strcpy_s(sSymbolName, c_iSymbolSize, "?");
				}

				IMAGEHLP_MODULE64 moduleInfo;
				moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);
				if (SymGetModuleInfo64(process, addr, &moduleInfo))
				{
					//wcstombs( sModuleName, moduleInfo.ModuleName, 1024 );
					strcpy_s(sModuleName, c_iSymbolSize, moduleInfo.ModuleName);
				}
				else {
					strcpy_s(sModuleName, c_iSymbolSize, "?");
				}

				IMAGEHLP_LINE_NAME line;
				line.SizeOfStruct = sizeof(IMAGEHLP_LINE_NAME);
				if (SymGetLineFromAddr(process, addr, &displacement, &line))
				{
					sprintf_s(sTmp, c_iTmpSize, "%s(%i) : (0x%08x) %s!%s\n", line.FileName, line.LineNumber, (DWORD32)addr, sModuleName, sSymbolName);
				}
				else {
					sprintf_s(sTmp, c_iTmpSize, "(0x%08x) %s!%s\n", (DWORD32)addr, sModuleName, sSymbolName);
				}

				strcat_s(s_pReturnBuffer, c_iReturnBufferSize, sTmp);
			}
			strcat_s(s_pReturnBuffer, c_iReturnBufferSize, "\n");
			return s_pReturnBuffer;
		}
	}
}