
#include "trip_planner/core/database.h"

using namespace std;

namespace trip_planner {

	namespace {
		const int MAX_LENGTH_DISTANCE = 2;
		const int MAX_EDIT_DISTANCE = 2;
	}

	database::database() {}

	database &database::instance() {
		static database instance;
		return instance;
	}

	vector<scode_t> database::query_stop(string stop_name) {

		vector<scode_t> res;

		if (stop_name.length() == 0)
			return res;

		for (auto it = _stops.begin(); it != _stops.end(); ++it) {

			if (abs((int)stop_name.length() - (int)(*it).second.get_name().length()) >= MAX_LENGTH_DISTANCE)
				continue;

			if (algorithm::knuth_morris_pratt(stop_name, (*it).second.get_name()).size() == 1) {
				res.push_back((*it).first);
				return res;
			}
		}

		for (auto it = _stops.begin(); it != _stops.end(); ++it) {

			if (abs((int)stop_name.length() - (int)(*it).second.get_name().length()) >= MAX_LENGTH_DISTANCE)
				continue;

			if (algorithm::dp_fuzzy_substr_match(stop_name, (*it).second.get_name()) <= MAX_EDIT_DISTANCE)
				res.push_back((*it).first);
		}

		return res;
	}

	vector<scode_t> database::query_stop(string stop_name, lcode_t line_code) {

		vector<scode_t> res;

		if (stop_name.length() == 0)
			return res;

		route_t route = _lines[line_code].get_route(ASCENDING);

		for (auto it = route.begin(); it != route.end(); ++it) {

			if (abs((int)stop_name.length() - (int)_stops[(*it).first].get_name().length()) >= MAX_LENGTH_DISTANCE)
				continue;

			if (algorithm::knuth_morris_pratt(stop_name, _stops[(*it).first].get_name()).size() == 1) {
				res.push_back((*it).first);
				return res;
			}
		}

		for (auto it = route.begin(); it != route.end(); ++it) {

			if (abs((int)stop_name.length() - (int)_stops[(*it).first].get_name().length()) >= MAX_LENGTH_DISTANCE)
				continue;

			if (algorithm::dp_fuzzy_substr_match(stop_name, _stops[(*it).first].get_name()) < MAX_EDIT_DISTANCE)
				res.push_back((*it).first);
		}

		return res;
	}

	lcode_t database::query_line(string line_name) {

		if (line_name.length() == 0)
			return "";

		for (auto it = _lines.begin(); it != _lines.end(); ++it) {
			if (algorithm::knuth_morris_pratt(line_name, (*it).second.get_pubcode()).size() == 1)
				return (*it).first;
		}

		return "";
	}

	void database::load_data() {

		utility::clear_screen();

		cout << " -- Trip Planner --" << endl << endl;

		cout << " Running loader..." << endl;

		auto start = chrono::high_resolution_clock::now();

		loader::instance().load();

		auto end = chrono::high_resolution_clock::now();

		cout << " Loader finished in " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms." << endl;
		
		cout << endl <<" Press any key to continue...";
		cin.get();
	}

	void database::build_graph(criteria_t option) {

		utility::clear_screen();

		cout << " -- Trip Planner --" << endl << endl;

		cout << " Running builder..." << endl;

		auto start = chrono::high_resolution_clock::now();

		builder::instance().build(option);

		auto end = chrono::high_resolution_clock::now();

		cout << " Builder finished in " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms." << endl;
		
		cout << endl <<" Press any key to continue...";
		cin.get();
	}

	void database::run_algorithm(scode_t start_stop, scode_t end_stop, algorithm_t algorithm) {

		utility::clear_screen();

		cout << " -- Trip Planner --" << endl << endl;

		cout << " Running algorithm... ";

		auto start = chrono::high_resolution_clock::now();

		string algorithm_name;

		switch (algorithm) {
		case DIJKSTRA:
			_graph.dijkstra(&_stops[start_stop]);
			algorithm_name = " Dijkstra";
			break;
		case 1:
			_graph.bellman_ford(&_stops[start_stop]);
			algorithm_name = " Bellman-Ford";
			break;
		case 2:
			_graph.johnson(&_stops[start_stop]);
			algorithm_name = " Johnson";
			break;
		case 3:
			_graph.a_star(&_stops[start_stop], &_stops[end_stop]);
			algorithm_name = " A*";
			break;
		}

		auto end = chrono::high_resolution_clock::now();

		cout << algorithm_name << " finished in " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ns." << endl;

		cout << endl << " Press any key to continue...";
		cin.get();
	}

	void database::parse_results(scode_t start_stop, scode_t end_stop) {

		utility::clear_screen();

		cout << " -- Trip Planner --" << endl << endl;

		cout << " Shortest path: " << endl;

		auto path = _graph.get_path(&_stops[start_stop], &_stops[end_stop]);

		auto nodes = path.first;
		auto edges = path.second;

		if (nodes.empty()) {
			cout << " No path from source stop to destination stop. " << endl;
			return;
		}

		for (size_t i = 0; i < nodes.size() - 1; i++) {

			cout << " > " << nodes[i]->get_name() << " (" << nodes[i]->get_code() << ")" << " to " << nodes[i + 1]->get_name() << " (" << nodes[i + 1]->get_code() << ")";

			if (edges[i + 1] == "walk")
				cout << ", walking." << endl;
			else
				cout << ", through line " << edges[i + 1] << " (" << nodes[i]->get_zone() << "->" << nodes[i + 1]->get_zone() << ")." << endl;
		}

		cout << endl << " Press any key to continue...";
		cin.get();
	}

	void database::view_graph(scode_t start_stop, scode_t end_stop) {

		utility::clear_screen();

		cout << " -- Trip Planner --" << endl << endl;

		cout << " Running viewer..." << endl;

		auto path = _graph.get_path(&_stops[start_stop], &_stops[end_stop]);

		viewer v(&_graph);
		v.view();
		v.show_path(path.first);

		cout << endl << " Press any key to continue...";
		cin.get();
	}
}