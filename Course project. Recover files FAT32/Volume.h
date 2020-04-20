#pragma once
#include "pch.h"
#include <list>
#include <vector>
using namespace std;

class Volume
{
	LPWSTR GUIDPath;
	wstring letter;
	wstring fileSystemName;
	ULARGE_INTEGER totalCapacity;
	//UINT64 startByte;
	//HANDLE handle;

public:
	Volume() { }

	Volume(LPCWSTR path) {
		GUIDPath = new WCHAR[100];
		wcscpy(GUIDPath, path);
		totalCapacity.QuadPart = 0;
	};

	Volume(const Volume& v) {
		GUIDPath = new WCHAR[100];
		wcscpy(GUIDPath, v.GUIDPath);
		totalCapacity = v.totalCapacity;
		letter = v.letter;
		fileSystemName = v.fileSystemName;
	}

	~Volume() { 
		delete[] GUIDPath;
	};

	wstring getLetter();
	void setLetter();

	wstring getFileSystemName();
	void setFileSystemName();

	LPCWSTR getGUIDPath();

	ULARGE_INTEGER getTotalCapacity();
	void setTotalCapacity();

	
};

