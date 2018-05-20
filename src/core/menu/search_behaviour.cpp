
#include "trip_planner/core/menu/search_behaviour.h"

using namespace std;

namespace trip_planner {

	int search_behaviour::display() {
		cout << " -- Trip Planner --" << endl << endl;
		cout << " Choose search mode: " << endl;
		cout << " [1] Search by stop" << endl;
		cout << " [2] Search in line" << endl;
		cout << " [0] Exit" << endl;
		cout << endl << " Input: ";

		string input;
		getline(cin, input);

		cout << endl;

		if (input.length() > 1)
			return 1;

		scode_t src, dst;

		switch (input[0]) {
		case '0':
			menu::instance().set_behaviour(DEFAULT);
			return 1;
		case '1':
			menu::instance().set_search_mode(BY_STOP);
			break;

		case '2':
			menu::instance().set_search_mode(BY_LINE);
			break;
		default:
			return 1;
		}

		menu::instance().set_behaviour(SELECTION);

		return 1;
	}
}