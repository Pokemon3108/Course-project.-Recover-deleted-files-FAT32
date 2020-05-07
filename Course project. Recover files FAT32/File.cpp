#include "pch.h"
#include "File.h"

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


