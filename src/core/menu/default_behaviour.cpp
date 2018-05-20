
#include "trip_planner/core/menu/default_behavior.h"

using namespace std;

namespace trip_planner {

	int default_behaviour::display() {
		cout << " -- Trip Planner --" << endl << endl;
		cout << " [1] Start " << endl;
		cout << " [0] Exit. " << endl;
		cout << endl << " Input: ";

		string input;
		getline(cin, input);

		if (input.length() > 1)
			return 1;

		switch (input[0]) {
		case '0':
			return 0;
		case '1':
			break;
		default:
			return 1;
		}

		menu::instance().set_behaviour(CRITERIA);

		return 1;
	}
}