#include "pch.h"
#include "Conversion.h"


UINT32 Conversion::converseToType(UCHAR* bytes, int startBorder, int endBorder)
{
	UINT32 number=0;
	for (int i = endBorder; i >= startBorder; --i) {
		number = number << 8;
		number += bytes[i];
	}
	return number;
}
