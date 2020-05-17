#include "pch.h"
#include "FAT32FileSystem.h"
#include "Conversion.h"
#include <iostream>
#include <algorithm>
#include "FileLfn.h"
#include <list>

void FAT32FileSystem::parseBootSector(UCHAR * info)
{
	Conversion conversion;
	bootSector.bytesInSector = conversion.converseToType(info, 11, 12);
	bootSector.sectorsInCluster = conversion.converseToType(info, 13, 13);
	bootSector.reservedAreaSize = conversion.converseToType(info, 14, 15);
	bootSector.fatCopiesNumber = conversion.converseToType(info, 16, 16);
	bootSector.sectorsInFS16 = conversion.converseToType(info, 19, 20);
	bootSector.sectorsInFS32 = conversion.converseToType(info, 32, 35);
	bootSector.fatSize = conversion.converseToType(info, 36, 39);
	bootSector.numberOfUpdateFat = conversion.converseToType(info, 40, 41);
	bootSector.clusterOfRootDirectory = conversion.converseToType(info, 44, 47);
}

void FAT32FileSystem::parseFatTable(UCHAR * buffer, int size)
{
	for (int i = 0; i < size; ++i) {
		UINT32 temp = 0;
		for (int j = 3; j >= 0; --j) {
			temp = temp << 8;
			UCHAR c = buffer[i * 4 + j];
			temp+=buffer[i * 4 + j];
			
		}
		
		fatTable[i] = temp;
	}
}

void FAT32FileSystem::getFileInfo(UINT32 offset, File& file)
{
	file.createFileName(rootDirectory + offset);
	file.createFirstCluster(rootDirectory+offset);
	file.createSize(rootDirectory + offset);
}


int FAT32FileSystem::getStartSectorOfActiveFat()
{
	int numberOfActiveFat = bootSector.fatCopiesNumber, startSector= bootSector.reservedAreaSize;
	if ((bootSector.numberOfUpdateFat & 128) == 0) {
		numberOfActiveFat = bootSector.numberOfUpdateFat;
		numberOfActiveFat &= 15;
	}
	if (numberOfActiveFat != bootSector.fatCopiesNumber) startSector += numberOfActiveFat * bootSector.fatSize;
	return startSector;
}

list<int> FAT32FileSystem::getFileClusters(const File & file)
{
	list<int> clusters;
	int clusterNumber = file.getSize() / (bootSector.sectorsInCluster*bootSector.bytesInSector) + 1;
	int startCluster = file.getFirstCluster();
	for (int i = 0; i < clusterNumber; i++) {
		int cluster = startCluster;
		clusters.push_back(cluster);
		while (fatTable[startCluster++]);
	}
	return clusters;
}


void FAT32FileSystem::createFatTable()
{
	int numberOfFatRecords=0;
	int startFatSector = getStartSectorOfActiveFat();
	int currentSector = startFatSector + bootSector.fatSize;

	bool recordsAreEmpty = true;
	UCHAR* buffer = new UCHAR[bootSector.bytesInSector];
	do {
		--currentSector;
		reader->ReadSector(currentSector, bootSector.bytesInSector, bootSector.bytesInSector, buffer);
		for (int i = 0; i < bootSector.bytesInSector; i++) {
			if (buffer[i] != 0) {
				recordsAreEmpty = false;
				break;
			}
		}

	} while (recordsAreEmpty && currentSector !=startFatSector);

	delete[] buffer;

	numberOfFatRecords = (currentSector - startFatSector + 1)*bootSector.bytesInSector / 4;
	buffer = new UCHAR[numberOfFatRecords * 4];

	fatTable = new UINT32[numberOfFatRecords*2];
	reader->ReadSector(startFatSector, bootSector.bytesInSector, 
		(currentSector - startFatSector + 1)*bootSector.bytesInSector,buffer);

	parseFatTable(buffer, numberOfFatRecords);
	delete[] buffer;
}


void FAT32FileSystem::createBootSector()
{
	UCHAR* buffer = new UCHAR[48];
	reader->ReadSector(0, 512, 48, buffer);
	parseBootSector(buffer);
	delete[] buffer;
}

void FAT32FileSystem::createRootDirectory()
{
	UCHAR* buffer = new UCHAR[bootSector.bytesInSector];

	bool recordIsEmpty = true;
	int currentSector = bootSector.reservedAreaSize + bootSector.fatCopiesNumber*bootSector.fatSize;
	currentSector += (bootSector.sectorsInCluster-1)*bootSector.bytesInSector;

	int noZeroSectors = bootSector.sectorsInCluster;


	while (noZeroSectors && recordIsEmpty) {
		reader->ReadSector(currentSector, bootSector.bytesInSector, bootSector.bytesInSector, buffer);
		for (int i = 0; i < bootSector.bytesInSector; ++i) {
			if (buffer[i] != 0) {
				recordIsEmpty = false;
				break;
			}
		}
		if (!recordIsEmpty) break;
		currentSector -= bootSector.bytesInSector;
		--noZeroSectors;
	}

	delete[] buffer;

	rootDirectorySize = (noZeroSectors)*bootSector.bytesInSector;
	rootDirectory = new UCHAR[rootDirectorySize];
	

	int rootDirectorySector = bootSector.reservedAreaSize + bootSector.fatCopiesNumber*bootSector.fatSize;
	reader->ReadSector(rootDirectorySector, bootSector.bytesInSector, rootDirectorySize, rootDirectory);

}

void FAT32FileSystem::recoverDeletedFiles()
{
	UINT32 offset = 0;

	while (offset < rootDirectorySize) {
		if (rootDirectory[offset] != 0xe5) {
			offset += 32;
			continue;
		}

		else {
			if (rootDirectory[offset + 11] == 0x0f) {
				if (!isCorrectLFN(offset)) {
					while (rootDirectory[offset + 11] == 0x0f)  offset += 32;
					offset += 32;
					continue;
				}
							
				FileLfn fileLfn;
				recoverFile(offset, fileLfn);
				while (rootDirectory[offset + 11] == 0x0f) offset += 32;
			}
				

			else if (rootDirectory[offset + 11] == 0x20) {
				File file;
				recoverFile(offset, file);
			}
			offset += 32;
		}
	}
}




void FAT32FileSystem::recoverFile(UINT32 offset, File& fileRecord)
{
	getFileInfo(offset, fileRecord);

	while (rootDirectory[offset + 11] == 0x0f) offset += 32;
	offset += 32;
	if (!isFreeCluster(offset, fileRecord.getFirstCluster())) return;

	list<int> clusters=getFileClusters(fileRecord);
	UCHAR* fileContent = new UCHAR[fileRecord.getSize()];

	
	UINT64 startSector = bootSector.reservedAreaSize + bootSector.fatCopiesNumber*bootSector.fatSize +
		(fileRecord.getFirstCluster() - 2)*bootSector.sectorsInCluster;
	reader->ReadSector(startSector, bootSector.bytesInSector, fileRecord.getSize(), fileContent);

	wstring filePath = L"..\\RecoveredFiles\\" + fileRecord.getFileName();
	HANDLE hFile = CreateFileW(filePath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD written;
	WriteFile(hFile, fileContent, fileRecord.getSize(), &written, NULL);

	delete [] fileContent;
}



bool FAT32FileSystem::isCorrectLFN(UINT32 offset)
{
	bool isCorrect = true;
	while (rootDirectory[offset + 11] == 0x0f) offset += 32;
	FileLfn fileLfn;
	getFileInfo(offset, fileLfn);
	if (fileLfn.getFirstCluster() == 0) isCorrect = false;
	return isCorrect;
}

bool FAT32FileSystem::isFreeCluster(UINT32 offset, int clusterNumber)
{
	File file;
	bool isFree = true;
	offset += 32;
	while (offset < rootDirectorySize) {
		while (rootDirectory[offset + 11] == 0x0f) offset += 32;
		
		if (clusterNumber == file.createFirstCluster(rootDirectory+offset)) {
			isFree = false;
			break;
		}

		offset += 32; 
	}
	return isFree;
}


