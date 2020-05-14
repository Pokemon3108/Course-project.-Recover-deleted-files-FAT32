#pragma once
#include "Reader.h"
#include "FAT32Structure.h"
#include "File.h"

class FAT32FileSystem
{
	Reader* reader;
	BootSector bootSector;
	UINT32* fatTable;
	UINT32 fatTableSize;
	UCHAR* rootDirectory;
	UINT32 rootDirectorySize;

	
public:
	FAT32FileSystem(Reader* reader) {
		this->reader = reader;
	};
	~FAT32FileSystem() {
		delete[] fatTable;
		delete[] rootDirectory;
	};

	void parseBootSector(UCHAR* info);
	void parseFatTable(UCHAR* buffer, int size);

	void createFatTable();
	void createBootSector();
	void createRootDirectory();

	int getStartSectorOfActiveFat();

	void recoverDeletedFiles();
	void recoverFile(UINT32 offset, File& fileRecord);

	vector<UINT32> getFileClusters(const File& file);
	bool isCorrectLFN(UINT32 offset);
	bool isFreeCluster(UINT32 offset, int clusterNumber);

	void getFileInfo(UINT32 offset, File& file);

};

