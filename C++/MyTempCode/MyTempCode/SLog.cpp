
#include <iostream>
#include <tchar.h>
#include "SLog.h"

using namespace std;

int main()
{
	SLog<TCHAR> log(TEXT("hh.txt"));
	log << TEXT("²ÝÄàÂí");
	Sleep(5000);

	return 0;
}