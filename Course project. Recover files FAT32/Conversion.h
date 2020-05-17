#pragma once
#include "pch.h"
class Conversion
{
public:
	Conversion() {};
	~Conversion() {};

	UINT32 converseToType(const UCHAR* bytes, int startBorder, int endBorder);
};

