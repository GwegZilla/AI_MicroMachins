#pragma once
#include "stdafx.h"

#define BLOCKING_ASSERTS
#define MY_ASSERT(a,b) DebugTools::_assert(a,b,__FUNCTION__,__LINE__)

class DebugTools
{
public:
	static const void _assert(const bool, const char*, const char*, const long);
private:
	DebugTools();
	~DebugTools();
};
