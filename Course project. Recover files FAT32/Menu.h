#pragma once
#include "Volume.h"
#include "ComputerInfo.h"

class Menu
{
public:
	Menu() {};
	~Menu() {};

	int chooseVolume(ComputerInfo& info);
	void printVolumesInformation(ComputerInfo& info) const;
};

