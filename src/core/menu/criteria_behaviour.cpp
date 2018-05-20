
#include "trip_planner/core/menu/criteria_behaviour.h"

using namespace std;

namespace trip_planner {

	int criteria_behaviour::display() {
		cout << " -- Trip Planner --" << endl << endl;
		cout << " Choose criteria: " << endl;
		cout << " [1] Distance." << endl;
		cout << " [2] Stop." << endl;
		cout << " [3] Price." << endl;
		cout << " [4] Real." << endl;
		cout << " [0] Exit." << endl;
		cout << endl << " Input: ";

		string input;
		getline(cin, input);

		if (input.length() > 1)
			return 1;

		switch (input[0]) {
		case '0':
			menu::instance().set_behaviour(DEFAULT);
			return 1;
		case '1':
			menu::instance().set_criteria(DISTANCE);
			break;
		case '2':
			menu::instance().set_criteria(STOP);
			break;
		case '3':
			menu::instance().set_criteria(PRICE);
			break;
		case '4':
			menu::instance().set_criteria(REAL);
			break;
		default:
			return 1;
		}

		menu::instance().set_behaviour(SEARCH);
		database::instance().build_graph(menu::instance().get_criteria());

		return 1;
	}
}