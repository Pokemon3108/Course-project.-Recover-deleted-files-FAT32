#pragma once
#include "pch.h"
#include "FAT32Structure.h"

using namespace std;
class File
{
private:
	UINT32 startCluster;
	wstring fileName;
	UINT32 size;
	FILE_TYPE fileType;

public:
	File() {};
	~File() {};

	UINT32 getStartCluster() const;
	void setStartCluster(UINT32 startCluster);

	wstring getFileName() const;
	void setFileName(const wstring &fileName);

	UINT32 getSize() const;
	void setSize(UINT32 size);

	FILE_TYPE getFileType() const;
	void setFileType(FILE_TYPE fileType);
	void setFileType(int fileType);
};

