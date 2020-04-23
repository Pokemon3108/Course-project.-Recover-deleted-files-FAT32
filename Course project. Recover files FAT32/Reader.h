#pragma once
#include "pch.h"
using namespace std;
class Reader
{
	HANDLE volumeHandle;

public:
	Reader() {
		volumeHandle = INVALID_HANDLE_VALUE;
	}
	Reader(HANDLE volumeHandle) {
		this->volumeHandle = volumeHandle;
	}
	
	~Reader() {
		if (volumeHandle != INVALID_HANDLE_VALUE) 
			CloseHandle(volumeHandle);
	}

	bool OpenDevice(wstring volumeName);
	int ReadSector(int sector, int sectorSize,int bytesToRead, UCHAR* buffer);
};

