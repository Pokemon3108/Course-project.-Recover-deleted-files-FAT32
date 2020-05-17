#pragma once
#include "File.h"

class FileLfn : public File
{

public:
	FileLfn() {};
	~FileLfn() {};

	wstring getPartName(const UCHAR * record, int offsetStart, int offsetEnd);

	virtual void createFileName(const UCHAR* record);
	virtual void createSize(const UCHAR* record);
	virtual UINT32 createFirstCluster(const UCHAR* record);
};

