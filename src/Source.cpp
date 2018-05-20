
#include <iostream>

#include "trip_planner/core/menu/menu.h"

using namespace std;

int main() {

	trip_planner::menu::instance().run();

	cout << endl << " Press any key to exit...";
	cin.get();

	return 0;
}