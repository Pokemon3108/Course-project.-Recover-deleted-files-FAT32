#include "pch.h"
#include "FAT32FileSystem.h"
#include "Conversion.h"

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

int FAT32FileSystem::getStartSectorOfActiveFat()
{
	int numberOfActiveFat = bootSector.fatCopiesNumber, startSector= bootSector.reservedAreaSize;
	if ((bootSector.numberOfUpdateFat & 64) == 0) {
		numberOfActiveFat = bootSector.numberOfUpdateFat;
		numberOfActiveFat &= 15;
	}
	if (numberOfActiveFat != bootSector.fatCopiesNumber) startSector *= numberOfActiveFat * bootSector.fatSize;
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

	numberOfFatRecords = (currentSector - startFatSector + 1)*bootSector.bytesInSector / 32;
	fatTable = new UINT32[numberOfFatRecords];
	reader->ReadSector(startFatSector, bootSector.bytesInSector, currentSector - startFatSector + 1,buffer);

	delete[] buffer;

}

void FAT32FileSystem::createBootSector()
{
	UCHAR* buffer = new UCHAR[48];
	reader->ReadSector(0, 512, 48, buffer);
	parseBootSector(buffer);
	delete[] buffer;
}
