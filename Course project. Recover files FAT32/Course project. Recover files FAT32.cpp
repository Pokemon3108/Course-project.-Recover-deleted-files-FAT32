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

	while (true) {

		Menu menu;
		menu.printVolumesInformation(info);
		if (!info.fileSystemisInComputer(fat)) {
			cout << "There is no FAT32 in computer" << endl;
			break;
		}
		int numberOfVolume = menu.chooseVolume(info);
		if (numberOfVolume == -1) break;

		Volume volume = info.getVolume(numberOfVolume);


		Reader reader;

		reader.OpenDevice(volume.getGUIDPath());

		cout << "Parse file system..." << endl;
		FAT32FileSystem fat32 = FAT32FileSystem(&reader);
		fat32.createBootSector();
		fat32.createFatTable();
		fat32.createRootDirectory();
		cout << "Recover files..." << endl;


		wstring folderName = L"..\\RecoveredFiles\\";
		folderName.push_back(volume.getLetter()[0]);
		folderName += L"\\";
		CreateDirectoryW(folderName.c_str(), NULL);
		fat32.recoverDeletedFiles(folderName);
		cout << "Files were recovered" << endl;

	}
	system("pause");

	return 0;
}



