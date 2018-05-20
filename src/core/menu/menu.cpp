
#include "trip_planner/core/menu/menu.h"

using namespace std;

namespace trip_planner {

	menu::menu() {

		database::instance().load_data();

		_behaviour = behaviour_t(new default_behaviour);
	}

	menu &menu::instance() {
		static menu instance;
		return instance;
	}

	void menu::run() {
		int ret;
		do {
			ret = display();
		} while (ret != 0);
	}

	criteria_t menu::get_criteria() const {
		return _criteria;
	}

	search_t menu::get_search_mode() const {
		return _search_mode;
	}

	algorithm_t menu::get_algorithm() const {
		return _algorithm;
	}

	scode_t menu::get_source() const {
		return _src;
	}

	scode_t menu::get_destination() const {
		return _dst;
	}

	void menu::set_behaviour(menu_t menu) {

		switch (menu) {
		case DEFAULT:
			_behaviour = behaviour_t(new default_behaviour);
			break;
		case CRITERIA:
			_behaviour = behaviour_t(new criteria_behaviour);
			break;
		case ALGORITHM:
			_behaviour = behaviour_t(new algorithm_behaviour);
			break;
		case SEARCH:
			_behaviour = behaviour_t(new search_behaviour);
			break;
		case SELECTION:
			_behaviour = behaviour_t(new selection_behaviour);
			break;
		case GRAPH:
			_behaviour = behaviour_t(new graph_behaviour);
			break;
		}
	}

	void menu::set_criteria(criteria_t criteria) {
		_criteria = criteria;
	}

	void menu::set_search_mode(search_t search_mode) {
		_search_mode = search_mode;
	}

	void menu::set_algorithm(algorithm_t  algorithm) {
		_algorithm = algorithm;
	}

	void menu::set_stops(scode_t src, scode_t dst) {
		_src = src;
		_dst = dst;
	}

	int menu::display() {
		utility::clear_screen();
		return _behaviour->display();
	}
}