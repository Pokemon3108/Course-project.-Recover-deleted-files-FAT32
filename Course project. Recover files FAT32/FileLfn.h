#pragma once
#include "File.h"

class FileLfn : public File
{

public:
	FileLfn() {};
	~FileLfn() {};

	std::wstring getPartName(UCHAR * record, int offsetStart, int offsetEnd);

	virtual std::wstring createFileName(UCHAR* record);
	virtual void createSize(UCHAR* record);
	virtual UINT32 createFirstCluster(UCHAR* record);
};

