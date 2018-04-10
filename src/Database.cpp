
/* Max distance a person is willing to walk to another stop */
#define MAX_WALK_DISTANCE	0.1		/* 100 meters */

/* Speeds for each means of transport */
#define WALKING_SPEED		5.0		
#define BUS_SPEED			50.0
#define TRAIN_SPEED			80.0

/* Penalties for some actions */
#define NO_PENALTY			1.0		/* no penalty */
#define ZONE_PENALTY		1.95	/* penalty for entering a new zone */

/* Priorities for each means of transport (calculations in addEdgesTime) */
#define BUS_RATIO			1.0		/* base time */
#define WALKING_RATIO		10.0	/* 10 times slower than bus */
#define SUBWAY_RATIO		0.625	/* 1.6 times faster than bus */

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

//void Database::_addEdgesDistance() {
//
//	//Add walking edges
//	for (auto src = _loader._stops.begin(); src != _loader._stops.end(); src++) {
//
//		for (auto dst = next(src, 1); dst != _loader._stops.end(); dst++) {
//
//			double distance = utils::haversineDistance((*src).second.getCoords(), (*dst).second.getCoords());
//
//			if (distance <= MAX_WALK_DISTANCE)
//				_connect2way((*src).first, (*dst).first, distance);
//		}
//	}
//
//	//Add line's routes
//	for (auto it = _loader._lines.begin(); it != _loader._lines.end(); it++) {
//
//		vector<route_t> routeA = (*it).second.getRoute(0);
//
//		if (routeA.size() > 0) {
//			for (size_t i = 0; i < routeA.size() - 1; i++)
//				_connect(routeA[i].first, routeA[i + 1].first, (*it).first, routeA[i].second);
//		}
//
//		vector<route_t> routeD = (*it).second.getRoute(1);
//
//		if (routeD.size() > 0) {
//			for (size_t i = 0; i < routeD.size() - 1; i++)
//				_connect(routeD[i].first, routeD[i + 1].first, (*it).first, routeD[i].second);
//		}
//	}
//}
//
//void Database::_addEdgesChanges() {
//
//	//Add walking edges
//	for (auto src = _loader._stops.begin(); src != _loader._stops.end(); src++) {
//
//		for (auto dst = next(src, 1); dst != _loader._stops.end(); dst++) {
//
//			double distance = utils::haversineDistance((*src).second.getCoords(), (*dst).second.getCoords());
//
//			if (distance <= MAX_WALK_DISTANCE)
//				_connect2way((*src).first, (*dst).first, 1);
//		}
//	}
//
//	//Add line's routes
//	for (auto it = _loader._lines.begin(); it != _loader._lines.end(); it++) {
//
//		vector<route_t> routeA = (*it).second.getRoute(0);
//
//		if (routeA.size() > 0) {
//			for (size_t i = 0; i < routeA.size() - 1; i++)
//				_connect(routeA[i].first, routeA[i + 1].first, (*it).first, 1);
//		}
//
//		vector<route_t> routeD = (*it).second.getRoute(1);
//
//		if (routeD.size() > 0) {
//			for (size_t i = 0; i < routeD.size() - 1; i++)
//				_connect(routeD[i].first, routeD[i + 1].first, (*it).first, 1);
//		}
//	}
//}
//
//void Database::_addEdgesTime() {
//
//	/*
//	Reasoning behind the values SUBWAY_PRIORITY AND WALKING_PRIORITY:
//
//	> Formula :
//	-> v = d / t <=> t = d / v.
//
//	> Considering the speed of a bus as 50 km/h, the speed of a train as 80 km/h and the speed of a person as 5km/h:
//	-> A subway is 1.6 times faster than the bus;
//	-> A person is 10 times slower than the bus.
//
//	> With this we can calculate:
//	-> vB = d / tB <=> tB = d / vB
//	-> vT = d / tT <=> 1.6 * vB = d / tT <=> 1.6 * (d / tB) = d / tT <=> 1.6 / tB = 1 / tT <=>
//	<=> tT = (1 / 1.6) * tB <=> tT = 0.625 * tB
//	-> vP = d / tP <=> 0.1 * vB = d / tP <=> 0.1 * (d / tB) = d / tP <=> 0.1 / tB = 1 / tP <=>
//	<=> tP = (1 / 0.1) * tB <=> tP = 10 * tB
//
//	> Therefore:
//	-> SUBWAY_PRIORITY = 0.625 (it takes 0.625 times less to make d kilometers)
//	-> WALKING_PRIORITY = 10 (it takes 10 times more to make d kilometers)
//
//	> Conclusion:
//	-> We can calculate tB: tB = d / 50.0
//	-> Then we get tT: tT = 0.625 * tB
//	-> And we get tP: tP = 10 * tB
//	*/
//
//	auto time_calculator = [](double ratio, double distance) {
//		return ratio * (distance / BUS_SPEED);
//	};
//
//	//Add walking edges
//	for (auto src = _loader._stops.begin(); src != _loader._stops.end(); src++) {
//
//		for (auto dst = next(src, 1); dst != _loader._stops.end(); dst++) {
//
//			double distance = utils::haversineDistance((*src).second.getCoords(), (*dst).second.getCoords());
//
//			if (distance <= MAX_WALK_DISTANCE)
//				((*src).first, (*dst).first, time_calculator(WALKING_RATIO, distance));
//		}
//	}
//
//	//Add line's routes
//	for (auto it = _loader._lines.begin(); it != _loader._lines.end(); it++) {
//
//		vector<route_t> routeA = (*it).second.getRoute(0);
//
//		if (routeA.size() > 0) {
//			for (size_t i = 0; i < routeA.size() - 1; i++) {
//
//				double distance = routeA[i].second;
//				double time;
//
//				if ((*it).second.getMode() == SUBWAY)
//					time = time_calculator(SUBWAY_RATIO, distance);
//				else
//					time = time_calculator(BUS_RATIO, distance);
//
//				_connect(routeA[i].first, routeA[i + 1].first, (*it).first, time);
//			}
//		}
//
//		vector<route_t> routeD = (*it).second.getRoute(1);
//
//		if (routeD.size() > 0) {
//			for (size_t i = 0; i < routeD.size() - 1; i++) {
//
//				double distance = routeD[i].second;
//				double time;
//
//				if ((*it).second.getMode() == SUBWAY)
//					time = time_calculator(SUBWAY_RATIO, distance);
//				else
//					time = time_calculator(BUS_RATIO, distance);
//
//				_connect(routeD[i].first, routeD[i + 1].first, (*it).first, distance);
//			}
//		}
//	}
//}
//
//void Database::_addEdgesPrice() {
//
//	//Add walking edges
//	for (auto src = _loader._stops.begin(); src != _loader._stops.end(); src++) {
//
//		for (auto dst = next(src, 1); dst != _loader._stops.end(); dst++) {
//
//			double distance = utils::haversineDistance((*src).second.getCoords(), (*dst).second.getCoords());
//
//			if (distance <= MAX_WALK_DISTANCE) {
//
//				if ((*src).second.getZone() != (*dst).second.getZone())
//					distance *= ZONE_EXTRA_PENALTY;
//				else
//					distance *= NO_PENALTY;
//
//				_connect2way((*src).first, (*dst).first, distance);
//			}
//		}
//	}
//
//	//Add line's routes
//	for (auto it = _loader._lines.begin(); it != _loader._lines.end(); it++) {
//
//		vector<route_t> routeA = (*it).second.getRoute(0);
//
//		if (routeA.size() > 0) {
//			for (size_t i = 0; i < routeA.size() - 1; i++) {
//
//				double distance = routeA[i].second;
//
//				if (_loader._stops[routeA[i].first].getZone() != _loader._stops[routeA[i + 1].first].getZone())
//					distance *= ZONE_EXTRA_PENALTY;
//				else
//					distance *= NO_PENALTY;
//
//				_connect(routeA[i].first, routeA[i + 1].first, (*it).first, distance);
//			}
//		}
//
//		vector<route_t> routeD = (*it).second.getRoute(1);
//
//		if (routeD.size() > 0) {
//			for (size_t i = 0; i < routeD.size() - 1; i++) {
//
//				double distance = routeD[i].second;
//
//				if (_loader._stops[routeD[i].first].getZone() != _loader._stops[routeD[i + 1].first].getZone())
//					distance *= ZONE_EXTRA_PENALTY;
//				else
//					distance *= NO_PENALTY;
//
//				_connect(routeD[i].first, routeD[i + 1].first, (*it).first, distance);
//			}
//		}
//	}
//}

//void Database::_addEdgesReal() {
//
//	auto applyModifiers = [](double distance, vector<double> modifiers) {
//
//		double dist_weight = DIST_PRIORITY * distance;
//		double time_weight = TIME_PRIORITY * (modifiers[0] * (distance / BUS_SPEED));
//		double pric_weight = PRIC_PRIORITY * (modifiers[1] * distance);
//
//		return dist_weight + time_weight + pric_weight;
//	};
//
//	//Add walking edges
//	for (auto src = _loader._stops.begin(); src != _loader._stops.end(); src++) {
//
//		for (auto dst = next(src, 1); dst != _loader._stops.end(); dst++) {
//
//			double distance = utils::haversineDistance((*src).second.getCoords(), (*dst).second.getCoords());
//
//			if (distance <= MAX_WALK_DISTANCE) {
//
//				vector<double> modifiers{ WALKING_RATIO };
//
//				if ((*src).second.getZone() != (*dst).second.getZone())
//					modifiers.push_back(ZONE_PENALTY);
//				else
//					modifiers.push_back(NO_PENALTY);
//
//				_connect2way((*src).first, (*dst).first, applyModifiers(distance, modifiers));
//			}
//		}
//	}
//
//	//Add line's routes
//	for (auto it = _loader._lines.begin(); it != _loader._lines.end(); it++) {
//
//		vector<route_t> routeA = (*it).second.getRoute(0);
//
//		if (routeA.size() > 0) {
//			for (size_t i = 0; i < routeA.size() - 1; i++) {
//
//				double distance = routeA[i].second;
//				vector<double> modifiers;
//
//				if ((*it).second.getMode() == SUBWAY)
//					modifiers.push_back(SUBWAY_RATIO);
//				else
//					modifiers.push_back(BUS_RATIO);
//
//				if (_loader._stops[routeA[i].first].getZone() != _loader._stops[routeA[i + 1].first].getZone())
//					modifiers.push_back(ZONE_PENALTY);
//				else
//					modifiers.push_back(NO_PENALTY);
//
//				_connect(routeA[i].first, routeA[i + 1].first, (*it).first, applyModifiers(distance, modifiers));
//			}
//		}
//
//		vector<route_t> routeD = (*it).second.getRoute(1);
//
//		if (routeD.size() > 0) {
//			for (size_t i = 0; i < routeD.size() - 1; i++) {
//
//				double distance = routeD[i].second;
//				vector<double> modifiers;
//
//				if ((*it).second.getMode() == SUBWAY)
//					modifiers.push_back(SUBWAY_RATIO);
//				else
//					modifiers.push_back(BUS_RATIO);
//
//				if (_loader._stops[routeD[i].first].getZone() != _loader._stops[routeD[i + 1].first].getZone())
//					modifiers.push_back(ZONE_PENALTY);
//				else
//					modifiers.push_back(NO_PENALTY);
//
//				_connect(routeD[i].first, routeD[i + 1].first, (*it).first, applyModifiers(distance, modifiers));
//			}
//		}
//	}
//}

double Database::_applyModifiers(int option, string start, string end, double weight, mode_t mode) {

	auto time_calculator = [](double ratio, double distance) {
		return ratio * (distance / BUS_SPEED);
	};

	auto same_zone = [](Stop start, Stop end) {
		return start.getZone() == end.getZone();
	};

	//Option
	//0 -> distance
	//1 -> changes
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
			return time_calculator(WALKING_RATIO, weight);
		else if (mode == SUBWAY)
			return time_calculator(SUBWAY_RATIO, weight);
		else if(mode == BUS)
			return time_calculator(BUS_RATIO, weight);
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
		return 0;
	}
}

void Database::_connect(string stop_src, string stop_dst, string line, double weight) {
	_graph.addEdge(&_loader._stops[stop_src], &_loader._stops[stop_dst], line, weight);
}

void Database::_connect2way(string stop_src, string stop_dst, double weight) {
	_graph.addEdge(&_loader._stops[stop_src], &_loader._stops[stop_dst], "walk", weight);
	_graph.addEdge(&_loader._stops[stop_dst], &_loader._stops[stop_src], "walk", weight);
}