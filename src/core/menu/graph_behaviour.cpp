
#include "trip_planner\core\menu\graph_behaviour.h"

using namespace std;

namespace trip_planner {

	int graph_behaviour::display() {
		cout << " -- Trip Planner --" << endl << endl;
		cout << " [1] Show graph." << endl;
		cout << " [0] Exit." << endl;
		cout << endl << " Input: ";

		string input;
		getline(cin, input);

		if (input.length() > 1)
			return 1;

		switch (input[0]) {
		case '0':
			menu::instance().set_behaviour(DEFAULT);
			break;
		case '1': {
			database::instance().view_graph(menu::instance().get_source(), menu::instance().get_destination());
		}
		default:
			break;
		}

		menu::instance().set_behaviour(DEFAULT);

		return 1;
	}
}