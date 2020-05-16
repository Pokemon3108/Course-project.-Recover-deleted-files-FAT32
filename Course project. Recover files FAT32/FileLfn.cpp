#include "pch.h"
#include "FileLfn.h"


std::wstring FileLfn::createFileName(UCHAR * record)
{
	int offset=0;
	wstring name;
	
	while (record[offset + 11] == 0x0f) {
		name += getPartName(record, 1, 10);
		name += getPartName(record, 14, 25);
		name += getPartName(record, 28, 31);

		int banSymbolNumber=std::count(name.begin(), name.end(), 0xffff);
		if (banSymbolNumber != 0) name.clear();

		offset += 32;

	}
	return name;
}

std::wstring FileLfn::getPartName(UCHAR * record, int offsetStart, int offsetEnd)
{
	UCHAR* buffer = new UCHAR[10];
	WCHAR* namePart = new WCHAR[5];

	std::copy(record + offsetStart, record + offsetEnd-offsetStart+1, buffer);
	mbstowcs(namePart, (const char*)buffer, offsetEnd - offsetStart + 1);
	std::wstring part(namePart);

	delete[] buffer;
	delete[] namePart;
	return part;
}

void FileLfn::createSize(UCHAR * record)
{
	int offset=0;
	while (record[offset + 11] == 0x0f) offset += 32;
	File::createSize(record + offset);
}

UINT32 FileLfn::createFirstCluster(UCHAR * record)
{
	int offset = 0;
	while (record[offset + 11] == 0x0f) offset += 32;
	File::createFirstCluster(record + offset);
	return this->getFirstCluster();
}
