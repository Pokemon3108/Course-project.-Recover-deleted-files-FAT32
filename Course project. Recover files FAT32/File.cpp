#include "pch.h"
#include "File.h"

UINT32 File::getStartCluster() const {
	return startCluster;
}

void File::setStartCluster(UINT32 startCluster) {
	this->startCluster = startCluster;
}

wstring File::getFileName() const {
	return fileName;
}

void File::setFileName(const wstring &fileName) {
	this->fileName = fileName;
}

UINT32 File::getSize() const {
	return size;
}

void File::setSize(UINT32 size) {
	this->size = size;
}

FILE_TYPE File::getFileType() const {
	return fileType;
}

void File::setFileType(FILE_TYPE fileType) {
	this->fileType = fileType;
}

void File::setFileType(int fileType)
{
	this->fileType = FILE_TYPE(fileType);
}
