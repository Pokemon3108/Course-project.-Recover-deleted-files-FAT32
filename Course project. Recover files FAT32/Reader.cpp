#include "pch.h"
#include "Reader.h"


bool Reader::OpenDevice(wstring volumeName)
{
	bool returnValue = true;
	int len = volumeName.length();
	volumeName.at(len - 1) = 0;
	volumeHandle = CreateFileW(volumeName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (volumeHandle == INVALID_HANDLE_VALUE) returnValue = false;
	return returnValue;
}

int Reader::ReadSector(int sector, int bytesToRead, UCHAR* buffer)
{
	if (volumeHandle == INVALID_HANDLE_VALUE) return 0;
	int result = 0;
	int remainBytes = bytesToRead % 512;
	int tempBytesToRead = bytesToRead - remainBytes;

	LARGE_INTEGER shiftInSectors;
	shiftInSectors.QuadPart = sector * 512;
	SetFilePointerEx(volumeHandle, shiftInSectors, NULL, FILE_BEGIN);

	DWORD alreadyReadBytes=0;
	ReadFile(volumeHandle, buffer, tempBytesToRead, &alreadyReadBytes, NULL);
	result = alreadyReadBytes;

	if (remainBytes!=0) {
		UCHAR tempBuf[512] = {};
		ReadFile(volumeHandle, tempBuf, 512, &alreadyReadBytes, NULL);
		memcpy(buffer + tempBytesToRead, tempBuf, remainBytes);
		result += remainBytes;
	}

	return result;
	
}
