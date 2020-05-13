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

	void parseBootSector(UCHAR* info);
	void parseFatTable(UCHAR* buffer, int size);
	File getFileInfo(UINT32 offset);
	UINT32 getFirstCluster(UINT32 offset);

public:
	FAT32FileSystem(Reader* reader) {
		this->reader = reader;
	};
	~FAT32FileSystem() {
		delete[] fatTable;
		delete[] rootDirectory;
	};

	void createFatTable();
	void createBootSector();
	void createRootDirectory();

	int getStartSectorOfActiveFat();

	void recoverDeletedFiles();
	void recoverLFNFile(UINT32& offset);
	void recoverFile(UINT32 offset);

	vector<UINT32> getFileClusters(const File& file);
	bool isCorrectLFN(UINT32 offset);
	bool isFreeCluster(UINT32 offset, int clusterNumber);

};

