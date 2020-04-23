#pragma once
#include "Reader.h"
#include "FAT32Structure.h"


class FAT32FileSystem
{
	Reader* reader;
	BootSector bootSector;
	UINT32* fatTable;

	void parseBootSector(UCHAR* info);
	int getStartSectorOfActiveFat();

public:
	FAT32FileSystem(Reader* reader) {
		this->reader = reader;
	};
	~FAT32FileSystem() {};

	void createFatTable();
	void createBootSector();
};

