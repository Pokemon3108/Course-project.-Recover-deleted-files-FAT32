#pragma once
#include "Volume.h"

class ComputerInfo
{
private:
	list<Volume> volumes;
public:
	ComputerInfo() {};
	~ComputerInfo() {};

	void searchVolumes();
	void eraseNoNamedVolumes();

	bool fileSystemisInComputer(wstring fsName);
	Volume getVolume(int number);

	list<Volume> getVolumes();
};

