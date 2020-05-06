#pragma once
#include "Reader.h"
#include "FAT32Structure.h"


class FAT32FileSystem
{
	Reader* reader;
	BootSector bootSector;
	UINT32* fatTable;
	UINT32 fatTableSize;
	UCHAR* rootDirectory;

	void parseBootSector(UCHAR* info);
	void parseFatTable(UCHAR* buffer, int size);
	int getStartSectorOfActiveFat();

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

};

