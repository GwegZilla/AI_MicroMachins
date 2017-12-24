#pragma once
#include "stdafx.h"
#ifndef _DEBUG_DEFINE_H_
#define _DEBUG_DEFINE_H_

namespace sft
{
	static	int g_iPaAssertCount = 0;
	static	int g_iPaErrorCount = 0;

#define SYMOPT_LOAD_LINES                0x00000010
#define SYMOPT_DEFERRED_LOADS            0x00000004

#define PA_CONCAT(a, b)		PA__CONCAT(a, b)
#define PA_CONCAT3(a, b, c)	PA__CONCAT(a, PA__CONCAT(b, c))
#define PA_STRINGIFY(s)		PA__STRINGIFY(s)

#define PA__CONCAT(a, b)	a ## b
#define PA__STRINGIFY(s)	# s

#define PA_TODO(sMessage)				__pragma(message(__FILE__ "(" PA_STRINGIFY(__LINE__) ") : TODO: " sMessage))
#define PA_ASSERT_PUSH_WARNING			__pragma(warning(push))\__pragma(warning(disable: 4127))
#define PA_ASSERT_POP_WARNING			__pragma(warning(pop))
#define PA_BREAK_CPU()					__debugbreak()
#define PA_KERNEL_CALLSTACK_MAX_DEPTH	32

#define PA_DELETE( PARAMPTR )\
{\
	delete ( PARAMPTR );\
	PARAMPTR = NULL;\
}

#define PA_DELETE_ARRAY( PARAMPTR )\
{\
	delete [] ( PARAMPTR );\
	PARAMPTR = NULL;\
}

	// assuming {} block in PA_DELETE
#define PA_SAFE_DELETE( PARAMPTR )\
{\
	if ( PARAMPTR ) PA_DELETE( PARAMPTR )\
}

#define PA_SAFE_DELETE_ARRAY( PARAMPTR )\
{\
	if ( PARAMPTR ) PA_DELETE_ARRAY( PARAMPTR )\
}

#define PA_T( x )  x

	enum MessageBoxResult
	{
		E_MB_RESULT_BREAK = 0,
		E_MB_RESULT_IGNOREONCE = 1,
		E_MB_RESULT_IGNOREALWAYS = 2,
	};

	enum CoreCallbackCallType	// TO RENAME!
	{
		E_ERROR_CALLBACK = 0,
		E_ASSERT_CALLBACK = 1,
	};

	enum LogLevel
	{
		E_INFO,
		E_WARN,
		E_ERROR,
	};

	typedef unsigned long(*CoreCallback)(CoreCallbackCallType, const char*, const char*, unsigned long, const char*, const char*);
	extern unsigned long DefaultUserCoreCB(CoreCallbackCallType eType, const char* sCond, const char* sFilename, unsigned long iLine, const char* sFunction, const char* sMsg);

	typedef void(*LogCallback)(unsigned short iMessageId, unsigned short iGroudId, const char* sSourceFile, const char* sFunction, int iSourceLine, const char* sMessage);
	extern LogCallback UserLogCallback;
	extern CoreCallback UserCoreCallback;

	unsigned long DefaultUserCoreCB(CoreCallbackCallType eType, const char* sCond, const char* sFilename, unsigned long iLine, const char* sFunction, const char* sMsg);

	//void WriteLogEntry(unsigned short iMessageId, unsigned short iGroupId, const char* szSourceFile, const char* szFunction, int iSourceLine, const char* szMessage);

	namespace StackWatcher
	{
		void Init();

		struct CallStackFrame
		{
			unsigned short m_iStackSize;
			size_t m_iStackAddr[PA_KERNEL_CALLSTACK_MAX_DEPTH];
		};

		//void					Get( CallStackFrame& oFrame, int iSkip = 0, void* pContext = NULL );
		const char*				GetReadableCallStackFromFrame(CallStackFrame& oFrame);
	}

	LRESULT CALLBACK MessageBoxBreakHook(INT iCode, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK MessageBoxBreakHookKeyboard(INT iCode, WPARAM wParam, LPARAM lParam);
	MessageBoxResult	MessageBoxBreak(LPCTSTR sMessage, LPCTSTR sTitle);

	void PaPrint(const char* sMessage, ...);
	void CopyMessageToClipboard(const char* sMessage);
	MessageBoxResult PaErrorHandler(CoreCallbackCallType eError, const char* sExpression, const char* sFile, const int iLine, const char* sFunction, const char* sMessage);
	int PaAssert(const char* sExpression, const char* sFile, const int iLine, const char* sFunction, const char* sMessage, ...);

#define PA_ASSERT(bExpression,...)\
{\
	if(!(bExpression))\
	{\
		static bool __bAssertAlwaysIgnore = false;\
		if (!__bAssertAlwaysIgnore)\
		{\
			switch (PaAssert(#bExpression, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__, ""))\
			{\
				case 0 :	PA_BREAK_CPU(); break;\
				case 1 :	break;\
				case 2 :	__bAssertAlwaysIgnore = true; break;\
				default:	break;\
			};\
		}\
	}\
}

#define PA_ERROR(bExpression, ...)\
{\
	static bool __bAssertAlwaysIgnore = false;\
	if (!__bAssertAlwaysIgnore)\
	{\
		switch (PaAssert(#bExpression, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__, ""))\
		{\
			case 0 :	PA_BREAK_CPU(); break;\
			case 1 :	break;\
			case 2 :	__bAssertAlwaysIgnore = true; break;\
			default:	break;\
		};\
	}\
}

#define PA_ASSERT_FALSE(...)			PA_ASSERT(0, __VA_ARGS__)
#define PA_ASSERT_PTR(pPointer)			PA_ASSERT(pPointer != NULL, "pointer is NULL")
#define PA_VERIFY						PA_ASSERT
}

#endif // _DEBUG_DEFINE_H_