// Client.cpp : Defines the entry point for the application.
//

#include <iostream>
#include "spdlog/spdlog.h"

using namespace std;

int main()
{
	spdlog::info( "Logging time!" );

	cout << "Hello CMake again." << endl;
	return 0;
}
