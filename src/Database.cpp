
/* Max distance a person is willing to walk to another stop */
#define MAX_WALK_DISTANCE	0.1		/* 100 meters */

/* Speeds for each means of transport */
#define WALKING_SPEED		5.0		/* 5 km/h */
#define BUS_SPEED			50.0	/* 50 km/h */
#define SUBWAY_SPEED		80.0	/* 80 km/h */

/* Penalties for some actions */
#define NO_PENALTY			1.0		/* no penalty */
#define ZONE_PENALTY		1.95	/* penalty for entering a new zone */

/* Priorities for each type of weights */
#define DIST_PRIORITY		0.25
#define TIME_PRIORITY		0.5			
#define PRIC_PRIORITY		0.25

#include "Database.h"

using namespace std;

void Database::run(string start, string end, string option, int algorithm) {

	_load();

	if (_loader._stops.find(start) == _loader._stops.end() || _loader._stops.find(end) == _loader._stops.end()) {
		cerr << " [ERROR]: invalid stop(s). " << endl;
		return;
	}

	_generateGraph(option);

	_runAlgorithm(start, end, algorithm);

	_parseResults(_graph.getPath(&_loader._stops[start], &_loader._stops[end]));

	cout << "\n Press any key to exit... " << endl;
	_getch();
}

void Database::_load() {

	cout << " Running loader... " << endl;
	auto loading_start = chrono::high_resolution_clock::now();

	_loader.run();

	auto loading_finish = chrono::high_resolution_clock::now();
	cout << " Loader finished in " << chrono::duration_cast<chrono::milliseconds>(loading_finish - loading_start).count() << " ms." << endl;
}

void Database::_generateGraph(string option) {

	cout << " Generating graph... ";
	auto generating_start = chrono::high_resolution_clock::now();

	_addVertices();

	if (option == "-distance")
		_addEdges(0);
	else if (option == "-stop")
		_addEdges(1);
	else if (option == "-time")
		_addEdges(2);
	else if (option == "-price")
		_addEdges(3);
	else if (option == "-real")
		_addEdges(4);

	auto generating_finish = chrono::high_resolution_clock::now();
	cout << " done in " << chrono::duration_cast<chrono::milliseconds>(generating_finish - generating_start).count() << " ms." << endl;

	cout << " Created a graph with " << _graph.getNumVertex() << " vertices." << endl << endl;
}

void Database::_runAlgorithm(string start, string end, int algorithm) {

	string alg_name;

	cout << " Running algorithm... ";
	auto algorithm_start = chrono::high_resolution_clock::now();

	switch (algorithm) {
	case 0:
		_graph.dijkstra(&_loader._stops[start]);
		alg_name = " Dijkstra";
		break;
	case 1:
		_graph.bellman_ford(&_loader._stops[start]);
		alg_name = " Bellman-Ford";
		break;
	case 2:
		_graph.johnson(&_loader._stops[start]);
		alg_name = " Johnson";
		break;
	case 3:
		_graph.a_star(&_loader._stops[start], &_loader._stops[end]);
		alg_name = " A*";
		break;
	}

	auto algorithm_end = chrono::high_resolution_clock::now();

	cout << alg_name << " took " << chrono::duration_cast<chrono::nanoseconds>(algorithm_end - algorithm_start).count() << " ns to run." << endl;
}

void Database::_parseResults(pair<vector<Stop*>, vector<string>> path) {

	cout << endl << " Shortest path: " << endl;

	auto nodes = path.first;
	auto edges = path.second;

	for (size_t i = 0; i < nodes.size() - 1; i++) {

		cout << " " << nodes[i]->getCode() << " to " << nodes[i + 1]->getCode();

		if (edges[i + 1] == "walk")
			cout << ", walking." << endl;
		else
			cout << ", through " << edges[i + 1] << " (" << nodes[i]->getZone() << "->" << nodes[i + 1]->getZone() << ")." << endl;
	}
}

void Database::_addVertices() {

	//Add stops to graph
	for (auto it = _loader._stops.begin(); it != _loader._stops.end(); it++)
		_graph.addVertex(&_loader._stops[(*it).first]);
}

void Database::_addEdges(int option) {

	//Add walking edges
	for (auto src = _loader._stops.begin(); src != _loader._stops.end(); src++) {

		for (auto dst = next(src, 1); dst != _loader._stops.end(); dst++) {

			double distance = utils::haversineDistance((*src).second.getCoords(), (*dst).second.getCoords());

			if (distance <= MAX_WALK_DISTANCE) {
				
				double weight = _applyModifiers(option, (*src).first, (*dst).first, distance, WALK);
				_connect2way((*src).first, (*dst).first, weight);
			}
		}
	}

	//Add line's routes
	for (auto it = _loader._lines.begin(); it != _loader._lines.end(); it++) {

		mode_t mode = (*it).second.getMode();

		vector<route_t> routeA = (*it).second.getRoute(0);

		if (routeA.size() > 0) {
			for (size_t i = 0; i < routeA.size() - 1; i++) {

				double distance = routeA[i].second;
				double weight = _applyModifiers(option, routeA[i].first, routeA[i + 1].first, distance, mode);

				_connect(routeA[i].first, routeA[i + 1].first, (*it).first, weight);
			}
		}

		vector<route_t> routeD = (*it).second.getRoute(1);

		if (routeD.size() > 0) {
			for (size_t i = 0; i < routeD.size() - 1; i++) {

				double distance = routeD[i].second;
				double weight = _applyModifiers(option, routeD[i].first, routeD[i + 1].first, distance, mode);

				_connect(routeD[i].first, routeD[i + 1].first, (*it).first, weight);
			}
		}
	}
}

double Database::_applyModifiers(int option, string start, string end, double weight, mode_t mode) {

	auto time = [](double speed, double distance) {
		return distance / speed;
	};

	auto same_zone = [](Stop start, Stop end) {
		return start.getZone() == end.getZone();
	};

	//Option
	//0 -> distance
	//1 -> stop
	//2 -> time
	//3 -> price
	//4 -> real

	switch (option) {
	case 0:
		return weight;
	case 1:
		return 1;
	case 2:
		if (mode == WALK)
			return time(WALKING_SPEED, weight);
		else if (mode == SUBWAY)
			return time(SUBWAY_SPEED, weight);
		else if(mode == BUS)
			return time(BUS_SPEED, weight);
	case 3:
		if (same_zone(_loader._stops[start], _loader._stops[end]))
			return NO_PENALTY;
		else
			return ZONE_PENALTY;
	case 4:
		return DIST_PRIORITY * _applyModifiers(0, start, end, weight, mode)
			+ TIME_PRIORITY * _applyModifiers(2, start, end, weight, mode)
			+ PRIC_PRIORITY * _applyModifiers(3, start, end, weight, mode);
	default:
		throw runtime_error("Criteria doesn't exist.");
	}
}

void Database::_connect(string stop_src, string stop_dst, string line, double weight) {
	_graph.addEdge(&_loader._stops[stop_src], &_loader._stops[stop_dst], line, weight);
}

void Database::_connect2way(string stop_src, string stop_dst, double weight) {
	_graph.addEdge(&_loader._stops[stop_src], &_loader._stops[stop_dst], "walk", weight);
	_graph.addEdge(&_loader._stops[stop_dst], &_loader._stops[stop_src], "walk", weight);
}