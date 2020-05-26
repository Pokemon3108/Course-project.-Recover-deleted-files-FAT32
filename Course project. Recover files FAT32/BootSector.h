#pragma once
#include "pch.h"

struct BootSector {
	UINT16 bytesInSector;
	UCHAR sectorsInCluster;
	UINT16 reservedAreaSize;
	UINT16 fatCopiesNumber;
	UINT16 sectorsInFS16;
	UINT32 sectorsInFS32;
	UINT32 fatSize;
	UINT16 numberOfUpdateFat;
	UINT32 clusterOfRootDirectory;
	
};





