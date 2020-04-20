#include "pch.h"
#include <iostream>
#include "Volume.h"
#include "Menu.h"
#include "ComputerInfo.h"
#include "Reader.h"

int main() {
	const int bufferSize = 100;
	using namespace std;
	wstring fat = L"FAT32";
	
	ComputerInfo info;
	info.searchVolumes();
	info.eraseNoNamedVolumes();

	
	Menu menu;	
	menu.printVolumesInformation(info);
	if (!info.fileSystemisInComputer(fat)) {
		cout << "There is no FAT32 in computer" << endl;
		return 0;
	}
	//int numberOfVolume=menu.chooseVolume(info);

	//if (numberOfVolume == -1) return 0;
	int numberOfVolume = 2;
	Volume volume = info.getVolume(numberOfVolume);

	//wstring path = volume.getGUIDPath();
	//path.at(path.length() - 1) = 0;
	//HANDLE hDisk = CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	Reader reader;

	reader.OpenDevice(volume.getGUIDPath());

	UCHAR* sector = new UCHAR[512];
	reader.ReadSector(0, 32, sector);

	for (int i = 0; i < 512; ++i) cout << sector[i];

	return 0;
}




//#include <iostream>
//#include <fstream>
//#include <windows.h>
//using namespace std;
//
//int win_error()
//{
//	cout << "error" << endl;
//	return 0;
//}
//
//int main()
//{
//
//	HANDLE hDisk = ::CreateFile(L"\\\\.\\F:", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//	if (hDisk == INVALID_HANDLE_VALUE) throw win_error();
//
//	BYTE buf[512];
//	DWORD dwRead;
//	if (!::ReadFile(hDisk, buf, sizeof(buf), &dwRead, NULL) || dwRead != sizeof(buf)) throw win_error();
//	::CloseHandle(hDisk);
//
//
//	ofstream out("1.txt");
//	for (int i = 0; i < 512; i++)
//	{
//		char f[3];  
//		_itoa(buf[i], f, 16);
//		out << i << "\t" << f << "\t" << buf[i] << '\n';
//		cout << buf[i];
//	}
//	out.close();
//	system("1.txt");
//
//
//	
//
//	cin.get();
//}


