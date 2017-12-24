#include "stdafx.h"
#include "DebugTools.h"

const void DebugTools::_assert(const bool l_iExpression, const char * l_arrcMessage, const char * l_arrcFile, const long l_luLine)
{
	if (l_iExpression)
	{
		printf("Assertion failed : %s at l %d . %s\n", l_arrcFile, l_luLine, l_arrcMessage);
#ifdef BLOCKING_ASSERTS
		printf("Abort... [BLOCKING_ASSERTS to avoid aborting]");
		abort();
#endif
	}
	return void();
}

DebugTools::DebugTools()
{
}

DebugTools::~DebugTools()
{
}