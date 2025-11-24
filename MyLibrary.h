#pragma once

#include <iostream>
using namespace std;

namespace MyLibrary
{

	int ReadNumber(string message)
	{
		int num;

		cout << message;
		cin >> num;

		return num;
	}

	int RandomNumber(int From, int To)
	{
		return rand() % (To - From + 1) + From;
	}
}