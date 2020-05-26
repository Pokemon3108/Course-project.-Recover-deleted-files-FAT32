#include "pch.h"
#include "FileLfn.h"
#include "Conversion.h"
#include <algorithm>

void FileLfn::createFileName(const UCHAR * record)
{
	int offset=0;
	wstring name;

	while (record[offset + 11 +32] == 0x0f) offset += 32;
	
	
	while (offset!=-32) {
		name += getPartName(record+offset, 1, 10);
		name += getPartName(record+offset, 14, 25);
		name += getPartName(record+offset, 28, 31);

		name.erase(std::remove(name.begin(), name.end(), 0xffff), name.end());
		

		offset -= 32;

	}

	this->fileName = name;
	//return name;
}

std::wstring FileLfn::getPartName(const UCHAR * record, int offsetStart, int offsetEnd)
{
	UCHAR* buffer = new UCHAR[12];
	wstring part;

	memcpy(buffer, record + offsetStart, offsetEnd - offsetStart + 1);

	Conversion conversion;
	int i = 0;
	for (int offset = offsetStart; offset < offsetEnd + 1; offset+=2, ++i) {
		part.push_back(conversion.converseToType(record, offset, offset + 1));
	}
	
	delete[] buffer;
	
	return part;
}

void FileLfn::createSize(const UCHAR * record)
{
	int offset=0;
	while (record[offset + 11] == 0x0f) offset += 32;
	File::createSize(record + offset);
}

UINT32 FileLfn::createFirstCluster(const UCHAR * record)
{
	int offset = 0;
	while (record[offset + 11] == 0x0f) offset += 32;
	File::createFirstCluster(record + offset);
	return this->firstCluster;
}
