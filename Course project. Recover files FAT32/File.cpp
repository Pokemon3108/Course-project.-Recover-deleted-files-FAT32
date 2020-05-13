#include "pch.h"
#include "File.h"
#include <algorithm>

void File::deleteSpacesInName()
{
	wstring::iterator it = fileName.begin();
	std::advance(it, 8);
	fileName.insert(it, '.');
	fileName.erase(std::remove(fileName.begin(), fileName.end(), ' '), fileName.end());

}

UINT32 File::getFirstCluster() const {
	return firstCluster;
}

void File::setFirstCluster(UINT32 firstCluster) {
	this->firstCluster = firstCluster;
}

UINT32 File::getSize() const {
	return size;
}

void File::setSize(UINT32 size) {
	this->size = size;
}

wstring File::getFileName() const
{
	return fileName;
}

void File::setFileName(wstring & fileName)
{
	this->fileName = fileName;
}


