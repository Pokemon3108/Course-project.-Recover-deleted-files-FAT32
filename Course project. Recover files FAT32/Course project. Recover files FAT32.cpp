#include "pch.h"
#include <iostream>
#include "Volume.h"
#include "Menu.h"
#include "ComputerInfo.h"
#include "Reader.h"
#include "FAT32FileSystem.h"

int main() {
	const int bufferSize = 100;
	using namespace std;
	wstring fat = L"FAT32";
	
	ComputerInfo info;
	info.searchVolumes();
	info.eraseNoNamedVolumes();

	
	Menu menu;	
	menu.printVolumesInformation(info);
	if (!info.fileSystemisInComputer(fat)) {
		cout << "There is no FAT32 in computer" << endl;
		getchar();
		return 0;
	}
	int numberOfVolume=menu.chooseVolume(info);

	//if (numberOfVolume == -1) return 0;
	//int numberOfVolume = 2;
	Volume volume = info.getVolume(numberOfVolume);

	//wstring path = volume.getGUIDPath();
	//path.at(path.length() - 1) = 0;
	//HANDLE hDisk = CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	Reader reader;

	reader.OpenDevice(volume.getGUIDPath());


	FAT32FileSystem fat32 = FAT32FileSystem(&reader);
	fat32.createBootSector();
	fat32.createFatTable();
	fat32.createRootDirectory();
	fat32.write();
	//fat32.recoverDeletedFiles();

/*
	const char* path = "G:/";
	DWORD d;
	STORAGE_PROPERTY_QUERY query = { StorageAccessAlignmentProperty,PropertyStandardQuery };
	STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR al = {};
	DeviceIoControl(reader.getHandle(), IOCTL_DISK_IS_WRITABLE,NULL, 0, NULL, 0, &d, NULL);*/

	//
	// 
	////UCHAR buf[2] = {'2', '3'};
	//DWORD w;
	//SetFilePointer(reader.getHandle(), 0 , NULL, FILE_BEGIN);
	//WriteFile(reader.getHandle(), buf, 2, &w, NULL);

	return 0;
}



