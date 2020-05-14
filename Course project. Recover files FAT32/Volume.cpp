#include "pch.h"
#include "Volume.h"

const int bufferSize = 100;
wstring Volume::getLetter()
{
	if (!letter.length()) setLetter();
	return letter;
}

void Volume::setLetter()
{
	int len = wcslen(GUIDPath);
	GUIDPath[len - 1] = 0;
	PCWSTR tempPath=new WCHAR[bufferSize];
	wcscpy((WCHAR*)tempPath, GUIDPath);
	
	LPWSTR volumeBuffer=new WCHAR[bufferSize];
	
	
	if (tempPath[0] == '\\' && tempPath[1]== '\\' && tempPath[3]== '\\'  && tempPath[2] == '?' 
		&& QueryDosDeviceW(GUIDPath+4, volumeBuffer, bufferSize)) {
		wcscpy((WCHAR*)tempPath, volumeBuffer);
	}

	LPWSTR let = new WCHAR[3];
	for (let[0] = 'A', let[1] = ':', let[2] = 0; let[0] <= L'Z'; let[0] += 1) {
		if (QueryDosDeviceW(let, volumeBuffer, bufferSize) && !_wcsicmp(volumeBuffer, tempPath)) {
			break;
		}
	}

	if (let[0] > 'Z') let[0] = '*';

	letter = let;

	GUIDPath[len - 1] = '\\';
	delete[] volumeBuffer;
	delete[] let;
	delete[] tempPath;
}

wstring Volume::getFileSystemName()
{
	if (fileSystemName.length()==0) setFileSystemName();
	return fileSystemName;
}


LPCWSTR Volume::getGUIDPath() const
{
	return GUIDPath;
}

ULARGE_INTEGER Volume::getTotalCapacity()
{
	if (totalCapacity.QuadPart==0) setTotalCapacity();
	return totalCapacity;
}

void Volume::setTotalCapacity()
{
	if (!letter.length()) setLetter();
	GetDiskFreeSpaceExW(letter.c_str(), NULL, &totalCapacity, NULL);
}


void Volume::setFileSystemName()
{
	LPWSTR fsName=new WCHAR[bufferSize];
	GetVolumeInformationW(GUIDPath, NULL, NULL, NULL, NULL, NULL, fsName, bufferSize);
	fileSystemName = fsName;
	delete[] fsName;
}
