#pragma once
#include "pch.h"
using namespace std;
class Reader
{
	HANDLE volumeHandle;
public:
	Reader() {
		volumeHandle = INVALID_HANDLE_VALUE;
	};
	~Reader() {
		if (volumeHandle != INVALID_HANDLE_VALUE) 
			CloseHandle(volumeHandle);
	};

	bool OpenDevice(wstring volumeName);
	int ReadSector(int sector, int bytesToRead, UCHAR* buffer);
};

