#include "pch.h"
#include "FAT32FileSystem.h"
#include "Conversion.h"
#include <iostream>

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

File FAT32FileSystem::getFileInfo(UINT32 offset)
{
	Conversion conversion;
	File fileRecord;

	UINT16 highBytesOfFirstCluster = conversion.converseToType(rootDirectory + offset, 20, 21);
	UINT16 lowBytesOfFirstCluster = conversion.converseToType(rootDirectory + offset, 26, 27);
	UINT32 firstCluster;
	firstCluster = highBytesOfFirstCluster << 16;
	firstCluster += lowBytesOfFirstCluster;
	fileRecord.setFirstCluster(firstCluster);

	UINT32 fileSize = conversion.converseToType(rootDirectory + offset, 28, 31);
	fileRecord.setSize(fileSize);

	return fileRecord;
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
		for (int i = 0; i < bootSector.bytesInSector; ++i) {
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
	UINT32 i = 0;

	while (i < rootDirectorySize) {
		if (rootDirectory[i] != 0xe5) {
			i += 32;
			continue;
		}

		else {
			if (rootDirectory[i + 11] == 0x0f)  recoverLFNFile(i);
				
			if (rootDirectory[i + 11] == 0x20) {
				recoverFile(i);
				i += 32;
			}
		}
	}
}

void FAT32FileSystem::recoverLFNFile(UINT32 & offset)
{

}

void FAT32FileSystem::recoverFile(UINT32 offset)
{
	File fileRecord = getFileInfo(offset);
	int clusterNumber = fileRecord.getSize() / (bootSector.sectorsInCluster*bootSector.bytesInSector) + 1;

}

void FAT32FileSystem::write()
{
	DWORD d;
	STORAGE_PROPERTY_QUERY query = { StorageAccessAlignmentProperty,PropertyStandardQuery };
	STORAGE_ACCESS_ALIGNMENT_DESCRIPTOR al = {};
	DeviceIoControl(reader->getHandle(), IOCTL_STORAGE_QUERY_PROPERTY, &query, sizeof query, &al, sizeof al, &d, NULL);

	DWORD f;
	rootDirectory[225] = 'a';
	LONG shift= (bootSector.reservedAreaSize + bootSector.fatCopiesNumber*bootSector.fatSize)*bootSector.bytesInSector;
	 
	DWORD b=SetFilePointer(reader->getHandle(), shift, NULL, FILE_BEGIN);

	//UCHAR* buffer = new UCHAR[512];
	
	//reader->ReadSector(0, 512, 512, buffer);
	UCHAR buf[65536] = {};
	bool flag=WriteFile(reader->getHandle(), buf, 65536, &f, NULL);
	if (flag == 0) std::cout << "Error" << std::endl;
	std::cout << GetLastError();


	
}
