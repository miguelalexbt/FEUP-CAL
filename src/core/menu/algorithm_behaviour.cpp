
#include "trip_planner/core/menu/algorithm_behaviour.h"

using namespace std;

namespace trip_planner {

	int algorithm_behaviour::display() {
		cout << " -- Trip Planner --" << endl << endl;
		cout << " [1] Dijkstra's algorithm." << endl;
		cout << " [2] Bellman-Ford's algorithm." << endl;
		cout << " [3] Johnson's algorithm." << endl;
		cout << " [4] A * algorithm." << endl;
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
			menu::instance().set_algorithm(DIJKSTRA);
			break;
		case '2':
			menu::instance().set_algorithm(BELLMAN_FORD);
			break;
		case '3':
			menu::instance().set_algorithm(JOHNSON);
			break;
		case '4':
			menu::instance().set_algorithm(A_STAR);
			break;
		default:
			return 1;
		}

		menu::instance().set_behaviour(GRAPH);
		database::instance().run_algorithm(menu::instance().get_source(), menu::instance().get_destination(), menu::instance().get_algorithm());
		database::instance().parse_results(menu::instance().get_source(), menu::instance().get_destination());

		return 1;
	}
}