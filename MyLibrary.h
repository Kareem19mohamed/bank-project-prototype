#pragma once

#include <iostream>
#include <limits>

using namespace std;

namespace MyLibrary
{

	int ReadNumber(string message)
	{
		int num;

		cout << message;
		cin >> num;
		while (cin.fail())
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "Invalid Number, Enter a valid one:" << endl;
			cin >> num;
		}

		return num;
	}

	int RandomNumber(int From, int To)
	{
		return rand() % (To - From + 1) + From;
	}
}