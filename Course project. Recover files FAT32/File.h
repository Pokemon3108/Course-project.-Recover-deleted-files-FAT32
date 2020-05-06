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
	UCHAR fileType;
	UINT32 recordNumberInFat;

public:
	File() {};
	~File() {};

	UINT32 getStartCluster() const;
	void setStartCluster(UINT32 startCluster);

	wstring getFileName() const;
	void setFileName(const wstring &fileName);

	UINT32 getSize() const;
	void setSize(UINT32 size);

	UCHAR getFileType() const;
	void setFileType(UCHAR fileType);
	

	UINT32 getrecordNumberInFat() const;
	void setrecordNumberInFat(UINT32 recordNumberInFat);
};

