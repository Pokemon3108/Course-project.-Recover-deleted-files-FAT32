#include "pch.h"
#include "Menu.h"
#include <iostream>
#include <iomanip>
#include <string>
#include "InputCheck.h"

int Menu::chooseVolume(ComputerInfo& info)
{
	list<Volume> volumes = info.getVolumes();
	int choice;
	bool flag=false;
	do {
		cout << "Choose volume (1-" << volumes.size() << "). 0 for quit" << endl;
		inputCheck(choice, 0, volumes.size());
		--choice;
		if (choice == -1) break;

		list<Volume>::iterator it = volumes.begin();
		advance(it, choice);
		if (it->getFileSystemName() != L"FAT32") {
			cout << "File system of this volume is not FAT32. Input again" << endl;
			continue;
		}
		flag = true;

	} while (!flag);
	return choice;
}

void Menu::printVolumesInformation(ComputerInfo& info) const
{
	list<Volume> volumes = info.getVolumes();
	list<Volume>::iterator it = volumes.begin();
	int i=1;
	int lenShifts = to_string(volumes.size()).length() + 3;
	for (it = volumes.begin(); it != volumes.end(); ++it, ++i) {
		//int lenShifts = to_string(i).length() + 3;
		wcout << i << ". " <<"Drive letter : " << it->getLetter() << endl
		<<"   File system: " << it->getFileSystemName() << endl
		<<"   Total capacity of volume: " << it->getTotalCapacity().QuadPart / pow(2, 30) << " Gb" << "\n" << endl;
	}
}
