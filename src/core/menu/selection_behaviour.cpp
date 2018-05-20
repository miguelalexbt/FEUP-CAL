
#include "trip_planner/core/menu/selection_behaviour.h"

using namespace std;

namespace trip_planner {

	int selection_behaviour::display() {

		cout << " -- Trip Planner --" << endl << endl;

		string source, destination;
		scode_t src, dst;

		switch (menu::instance().get_search_mode()) {
		case BY_STOP: {

			cout << " Source station: ";
			getline(cin, source);
			utility::to_upper(source);

			vector<scode_t> res_src = database::instance().query_stop(source);

			if (res_src.empty() || (src = process_response(res_src)) == "") {
				cout << " Source station not found!" << endl;
				cin.get();

				menu::instance().set_behaviour(SEARCH);
				return 1;
			}

			cout << " Destination station: ";
			getline(cin, destination);
			utility::to_upper(destination);

			vector<scode_t> res_dst = database::instance().query_stop(destination);

			if (res_dst.empty() || (dst = process_response(res_dst)) == "") {
				cout << " Destination station not found!" << endl;
				cin.get();

				menu::instance().set_behaviour(SEARCH);
				return 1;
			}
			break;
		}
		case BY_LINE: {

			string line_name;
			lcode_t line_code;

			cout << " Line: ";
			getline(cin, line_name);

			utility::to_upper(line_name);

			if ((line_code = database::instance().query_line(line_name)) == "") {
				cout << " Line not found!" << endl;
				cin.get();

				menu::instance().set_behaviour(SEARCH);
				return 1;
			}

			cout << " Source station: ";
			getline(cin, source);

			utility::to_upper(source);

			vector<scode_t> res_src = database::instance().query_stop(source, line_code);

			if (res_src.empty() || (src = process_response(res_src)) == "") {
				cout << " Source station not found in specified line!" << endl;
				cin.get();

				menu::instance().set_behaviour(SEARCH);
				return 1;
			}

			cout << " Destination station: ";
			getline(cin, destination);

			utility::to_upper(destination);

			vector<scode_t> res_dst = database::instance().query_stop(destination, line_code);

			if (res_dst.empty() || (dst = process_response(res_dst)) == "") {
				cout << " Destination station not found in specified line!" << endl;
				cin.get();

				menu::instance().set_behaviour(SEARCH);
				return 1;
			}
			break;
		}
		}

		if (src == dst) {
			cout << " Source and destination stations cannot be the same!" << endl;
			cin.get();

			menu::instance().set_behaviour(SEARCH);
			return 1;
		}

		menu::instance().set_behaviour(ALGORITHM);
		menu::instance().set_stops(src, dst);

		return 1;
	}

	scode_t selection_behaviour::process_response(const vector<scode_t> &res) {

		if (res.size() == 1) {
			return res[0];
		}
		else {

			cout << endl << " Did you mean one of this stops: " << endl;

			for (size_t i = 0; i < res.size(); ++i) {
				cout << " [" << setw(2) << setfill('0') << i + 1 << "] " << database::instance()._stops[res[i]].get_name() << " (" << res[i] << ")" << endl;
			}

			cout << endl << " Input: ";

			string input;
			getline(cin, input);

			cout << endl;

			if (!utility::is_number(input)) {
				return "";
			}
			else {

				unsigned int choice = (unsigned int)stoi(input);

				if (choice == 0 || choice > res.size())
					return "";
				else
					return res[choice - 1];
			}
		}
	}
}