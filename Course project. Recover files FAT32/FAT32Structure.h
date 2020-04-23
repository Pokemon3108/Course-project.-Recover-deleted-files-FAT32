#pragma once
#include "pch.h"

struct BootSector {
	UINT16 bytesInSector;
	BYTE sectorsInCluster;
	UINT16 reservedAreaSize;
	BYTE fatCopiesNumber;
	UINT16 sectorsInFS16;
	UINT32 sectorsInFS32;
	UINT32 fatSize;
	UINT16 numberOfUpdateFat;
	UINT32 clusterOfRootDirectory;
	
};


struct DirectoryRecord {
	BYTE name[11];
	BYTE attribute;
	UINT16 highBytesOfFirstCluster;
	UINT16 lowBytesOfFirstCluster;
	UINT32 fileSize;
};


enum FILE_TYPE {
	ONLY_FOR_READ = 0x01,
	HIDDEN_FILE=0x02,
	SYSTEM_FILE=0x04,
	VOLUME_LABEL=0x08,
	LONG_FILE_NAME=0x0f,
	DIRECTORY=0x10,
	ARCHIVAL_FILE=0x20
};

struct DirectoryRecordLFN {
	BYTE numberOfLFNRecord;
	WCHAR fileName1_5[5];
	BYTE attribyte;
	BYTE controlSum;
	WCHAR fileName6_11[5];
	WCHAR fileName12_13[2];
};



