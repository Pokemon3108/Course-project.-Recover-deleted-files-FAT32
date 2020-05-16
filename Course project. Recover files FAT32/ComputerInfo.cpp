#include "pch.h"
#include "ComputerInfo.h"

void ComputerInfo::searchVolumes()
{
	const int bufferSize = 100;
	list<Volume> volumes;
	HANDLE h;
	LPWSTR volumeName = new WCHAR[bufferSize];
	h = FindFirstVolumeW(volumeName, bufferSize);

	do {

		Volume volume = Volume(volumeName);
		volumes.push_back(volume);

	} while (FindNextVolumeW(h, volumeName, bufferSize));

	delete[] volumeName;
	this->volumes = volumes;
}

void ComputerInfo::eraseNoNamedVolumes()
{
	Volume v;
	list<Volume>::iterator it;
	for (it = volumes.begin(); it != volumes.end(); ) {
		if (it->getLetter()[0] == '*')
			it = volumes.erase(it);
		else it++;
	}
}

bool ComputerInfo::fileSystemisInComputer(wstring fsName)
{
	list<Volume>::iterator it;
	bool flag=false;
	for (it = volumes.begin(); it != volumes.end(); it++) {
		if (it->getFileSystemName() == fsName) {
			flag = true;
			break;
		}
	}
	return flag;
}

Volume ComputerInfo::getVolume(int number)
{
	list<Volume>::iterator it=volumes.begin();
	advance(it, number);
	return *it;
}


list<Volume> ComputerInfo::getVolumes()
{
	return volumes;
}
