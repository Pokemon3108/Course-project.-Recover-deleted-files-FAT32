#include "pch.h"
#include "Reader.h"


HANDLE Reader::getHandle()
{
	return volumeHandle;
}

bool Reader::OpenDevice(wstring volumeName)
{
	if (volumeHandle != INVALID_HANDLE_VALUE) return false;
	bool returnValue = true;
	int len = volumeName.length();
	volumeName.at(len - 1) = 0;
	volumeHandle = CreateFileW(volumeName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE , 
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_DEVICE, NULL);
	if (volumeHandle == INVALID_HANDLE_VALUE) returnValue = false;
	return returnValue;
}

int Reader::ReadSector(UINT64 sector, int sectorSize,int bytesToRead, UCHAR* buffer)
{
	if (volumeHandle == INVALID_HANDLE_VALUE) return 0;
	int result = 0;
	int remainBytes = bytesToRead % sectorSize;
	int tempBytesToRead = bytesToRead - remainBytes;

	LARGE_INTEGER shiftInSectors;
	shiftInSectors.QuadPart = sector * sectorSize;
	SetFilePointerEx(volumeHandle, shiftInSectors, NULL, FILE_BEGIN);

	DWORD alreadyReadBytes=0;
	ReadFile(volumeHandle, buffer, tempBytesToRead, &alreadyReadBytes, NULL);
	result = alreadyReadBytes;

	if (remainBytes != 0) {
		UCHAR tempBuf[1024] = {};
		ReadFile(volumeHandle, tempBuf, sectorSize, &alreadyReadBytes, NULL);
		memcpy(buffer + tempBytesToRead, tempBuf, remainBytes);
		result += remainBytes;
	}

	return result;
	
}
