#include "pch.h"
#include "InputCheck.h"

void inputCheck(int & number, int range1, int range2)
{
	using namespace std;
	bool flag = false;
	do {
		try {
			flag = false;
			cin >> number;
			if (number<range1 || number>range2) throw out_of_range("Input is out of range. Try again");
			
		}
		catch (out_of_range e) {
			flag = true;
			cout << e.what() << endl;
		}
		cin.ignore(32768, '\n');
	} while (flag);
}
