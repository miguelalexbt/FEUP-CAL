#ifndef GRID_H
#define GRID_H

/* Max distance a person is willing to walk to another stop */
#define MAX_WALK_DISTANCE	0.1		/* 100 meters */

/* Penalties for some actions */
#define NO_PENALTY			1.0		/* no penalty */
#define ZONE_PENALTY		1.95	/* penalty for entering a new zone */
#define	ZONE_EXTRA_PENALTY	100		/* unrealistic penalty, used to force Dijsktra to change zones as few times as possible */

/* Priorities for each means of transportation (calculations in addEdgesTime) */
#define BUS_PRIORITY		1.0		/* base speed */
#define WALKING_PRIORITY	10.0	/* 10 times slower than bus */
#define SUBWAY_PRIORITY		0.625	/* 1.6 times faster than bus */

/* Weights for each type of constraints */
#define DISTANCE_WEIGHT		0.4
#define TIME_WEIGHT			0.3			
#define PRICE_WEIGHT		0.3

#include <iostream>
#include <fstream>
#include <chrono>
#include <conio.h>

#include "Graph.h"
#include "Loader.h"

using namespace std;

class Grid {

	Graph<Stop*>	_graph;
	Loader			_loader;

public:

	Grid() {}

	void run(string start, string end, string option, int algorithm) {

		load();

		generateGraph(option);

		if (_loader._stops.find(start) == _loader._stops.end() || _loader._stops.find(end) == _loader._stops.end()) {
			cerr << " [ERROR]: invalid stop(s). " << endl;
			return;
		}

		runAlgorithm(start, end, algorithm);

		parseResults(_graph.getPath(&_loader._stops[start], &_loader._stops[end]));

		cout << "\n Press any key to exit... " << endl;
		_getch();
	}
	
	void load() {

		cout << " Running loader... " << endl;
		auto loading_start = chrono::high_resolution_clock::now();

		_loader.run();

		auto loading_finish = chrono::high_resolution_clock::now();
		cout << " Loader finished in " << chrono::duration_cast<chrono::milliseconds>(loading_finish - loading_start).count() << " ms." << endl;
	}

	void generateGraph(string option) {

		cout << " Generating graph... ";
		auto generating_start = chrono::high_resolution_clock::now();

		addVertices();

		if (option == "-d")
			addEdgesDistance();
		else if (option == "-s")
			addEdgesChanges();
		else if (option == "-t")
			addEdgesTime();
		else if (option == "-p")
			addEdgesPrice();
		else if (option == "-r")
			addEdgesReal();

		auto generating_finish = chrono::high_resolution_clock::now();
		cout << " done in " << chrono::duration_cast<chrono::milliseconds>(generating_finish - generating_start).count() << " ms." << endl;

		cout << " Created a graph with " << _graph.getNumVertex() << " vertices." << endl << endl;
	}

	void runAlgorithm(string start, string end, int algorithm) {

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

		cout << alg_name << " took " << chrono::duration_cast<chrono::nanoseconds>(algorithm_end - algorithm_start).count() << "ns to run." << endl;
	}

	void parseResults(pair<vector<Stop*>, vector<string>> path) {

		cout << endl << " Shortest path: " << endl;

		auto nodes = path.first;
		auto edges = path.second;

		for (size_t i = 0; i < nodes.size() - 1; i++) {

			cout << " " << nodes[i]->getCode() << " to " << nodes[i + 1]->getCode();

			if (edges[i + 1] == "walk")
				cout << ", walking." << endl;
			else
				cout << ", through " << edges[i + 1] << "." << endl;
		}
	}

	void addVertices() {

		//Add stops to graph
		for (auto it = _loader._stops.begin(); it != _loader._stops.end(); it++)
			_graph.addVertex(&_loader._stops[(*it).first]);
	}

	void addEdgesDistance() {

		//Add walking edges
		for (auto src = _loader._stops.begin(); src != _loader._stops.end(); src++) {

			for (auto dst = next(src, 1); dst != _loader._stops.end(); dst++) {
				
				double distance = utils::haversineDistance((*src).second.getCoords(), (*dst).second.getCoords());

				if (distance <= MAX_WALK_DISTANCE)
					connect2way((*src).first, (*dst).first, distance);
			}
		}

		//Add line's routes
		for (auto it = _loader._lines.begin(); it != _loader._lines.end(); it++) {

			vector<route_t> routeA = (*it).second.getRoute(0);

			if (routeA.size() > 0) {
				for (size_t i = 0; i < routeA.size() - 1; i++)
					connect(routeA[i].first, routeA[i + 1].first, (*it).first, routeA[i].second);
			}

			vector<route_t> routeD = (*it).second.getRoute(1);

			if (routeD.size() > 0) {
				for (size_t i = 0; i < routeD.size() - 1; i++)
					connect(routeD[i].first, routeD[i + 1].first, (*it).first, routeD[i].second);
			}
		}
	}

	void addEdgesChanges() {

		//Add walking edges
		for (auto src = _loader._stops.begin(); src != _loader._stops.end(); src++) {

			for (auto dst = next(src, 1); dst != _loader._stops.end(); dst++) {

				double distance = utils::haversineDistance((*src).second.getCoords(), (*dst).second.getCoords());

				if (distance <= MAX_WALK_DISTANCE)
					connect2way((*src).first, (*dst).first, 1);
			}
		}

		//Add line's routes
		for (auto it = _loader._lines.begin(); it != _loader._lines.end(); it++) {

			vector<route_t> routeA = (*it).second.getRoute(0);

			if (routeA.size() > 0) {
				for (size_t i = 0; i < routeA.size() - 1; i++)
					connect(routeA[i].first, routeA[i + 1].first, (*it).first, 1);
			}

			vector<route_t> routeD = (*it).second.getRoute(1);

			if (routeD.size() > 0) {
				for (size_t i = 0; i < routeD.size() - 1; i++)
					connect(routeD[i].first, routeD[i + 1].first, (*it).first, 1);
			}
		}
	}

	void addEdgesTime() {

		/* 
		Reasoning behind the values SUBWAY_PRIORITY AND WALKING_PRIORITY:

		> Formula :
			-> v = d / t, therefore t = d / v.

		> Considering the speed of a bus as 50 km/h, the speed of a train as 80 km/h and the speed of a person as 5km/h:
			-> A subway is 1.6 times faster than the bus;
			-> A person is 10 times slower than the bus.

		> With this we can calculate:
			-> vB = d / tB <=> tB = d / vB
			-> vT = d / tT <=> 1.6 * vB = d / tT <=> 1.6 * (d / tB) = d / tT <=> 1.6 / tB = 1 / tT <=>
				<=> tT = (1 / 1.6) * tB <=> tT = 0.625 * tB
			-> vP = d / tP <=> 0.1 * vB = d / tP <=> 0.1 * (d / tB) = d / tP <=> 0.1 / tB = 1 / tP <=>
				<=> tP = (1 / 0.1) * tB <=> tT = 10 * tB

		> Conclusion:
			-> SUBWAY_PRIORITY = 0.625
			-> WALKING_PRIORITY = 10
		*/

		//Add walking edges
		for (auto src = _loader._stops.begin(); src != _loader._stops.end(); src++) {

			for (auto dst = next(src, 1); dst != _loader._stops.end(); dst++) {

				double distance = utils::haversineDistance((*src).second.getCoords(), (*dst).second.getCoords());

				if (distance <= MAX_WALK_DISTANCE)
					connect2way((*src).first, (*dst).first, WALKING_PRIORITY * distance);
			}
		}

		//Add line's routes
		for (auto it = _loader._lines.begin(); it != _loader._lines.end(); it++) {

			vector<route_t> routeA = (*it).second.getRoute(0);

			if (routeA.size() > 0) {
				for (size_t i = 0; i < routeA.size() - 1; i++) {

					double distance = routeA[i].second;

					if ((*it).second.getMode() == SUBWAY)
						distance *= SUBWAY_PRIORITY;
					else
						distance *= BUS_PRIORITY;

					connect(routeA[i].first, routeA[i + 1].first, (*it).first, distance);
				}
			}

			vector<route_t> routeD = (*it).second.getRoute(1);

			if (routeD.size() > 0) {
				for (size_t i = 0; i < routeD.size() - 1; i++) {

					double distance = routeD[i].second;

					if ((*it).second.getMode() == SUBWAY)
						distance *= SUBWAY_PRIORITY;
					else
						distance *= BUS_PRIORITY;
					
					connect(routeD[i].first, routeD[i + 1].first, (*it).first, distance);
				}
			}
		}
	}

	void addEdgesPrice() {

		//Add walking edges
		for (auto src = _loader._stops.begin(); src != _loader._stops.end(); src++) {

			for (auto dst = next(src, 1); dst != _loader._stops.end(); dst++) {

				double distance = utils::haversineDistance((*src).second.getCoords(), (*dst).second.getCoords());

				if (distance <= MAX_WALK_DISTANCE) {

					if ((*src).second.getZone() != (*dst).second.getZone())
						distance *= ZONE_EXTRA_PENALTY;
					else
						distance *= NO_PENALTY;

					connect2way((*src).first, (*dst).first, distance);
				}
			}
		}

		//Add line's routes
		for (auto it = _loader._lines.begin(); it != _loader._lines.end(); it++) {

			vector<route_t> routeA = (*it).second.getRoute(0);

			if (routeA.size() > 0) {
				for (size_t i = 0; i < routeA.size() - 1; i++) {

					double distance = routeA[i].second;

					if (_loader._stops[routeA[i].first].getZone() != _loader._stops[routeA[i + 1].first].getZone())
						distance *= ZONE_EXTRA_PENALTY;
					else
						distance *= NO_PENALTY;
						
					connect(routeA[i].first, routeA[i + 1].first, (*it).first, distance);
				}
			}

			vector<route_t> routeD = (*it).second.getRoute(1);

			if (routeD.size() > 0) {
				for (size_t i = 0; i < routeD.size() - 1; i++) {

					double distance = routeD[i].second;

					if (_loader._stops[routeD[i].first].getZone() != _loader._stops[routeD[i + 1].first].getZone())
						distance *= ZONE_EXTRA_PENALTY;
					else
						distance *= NO_PENALTY;

					connect(routeD[i].first, routeD[i + 1].first, (*it).first, distance);
				}
			}
		}
	}

	void addEdgesReal() {

		//Add walking edges
		for (auto src = _loader._stops.begin(); src != _loader._stops.end(); src++) {

			for (auto dst = next(src, 1); dst != _loader._stops.end(); dst++) {

				double distance = utils::haversineDistance((*src).second.getCoords(), (*dst).second.getCoords());

				if (distance <= MAX_WALK_DISTANCE) {
					
					vector<double> modifiers{ WALKING_PRIORITY };

					if ((*src).second.getZone() != (*dst).second.getZone())
						modifiers.push_back(ZONE_PENALTY);
					else
						modifiers.push_back(NO_PENALTY);

					connect2way((*src).first, (*dst).first, applyModifiers(distance, modifiers));
				}
			}
		}

		//Add line's routes
		for (auto it = _loader._lines.begin(); it != _loader._lines.end(); it++) {

			vector<route_t> routeA = (*it).second.getRoute(0);

			if (routeA.size() > 0) {
				for (size_t i = 0; i < routeA.size() - 1; i++) {
					
					double distance = routeA[i].second;
					vector<double> modifiers;

					if ((*it).second.getMode() == SUBWAY)
						modifiers.push_back(SUBWAY_PRIORITY);
					else
						modifiers.push_back(BUS_PRIORITY);

					if (_loader._stops[routeA[i].first].getZone() != _loader._stops[routeA[i + 1].first].getZone())
						modifiers.push_back(ZONE_PENALTY);
					else
						modifiers.push_back(NO_PENALTY);

					connect(routeA[i].first, routeA[i + 1].first, (*it).first, applyModifiers(distance, modifiers));
				}
			}

			vector<route_t> routeD = (*it).second.getRoute(1);

			if (routeD.size() > 0) {
				for (size_t i = 0; i < routeD.size() - 1; i++) {

					double distance = routeD[i].second;
					vector<double> modifiers;

					if ((*it).second.getMode() == SUBWAY)
						modifiers.push_back(SUBWAY_PRIORITY);
					else
						modifiers.push_back(BUS_PRIORITY);

					if (_loader._stops[routeD[i].first].getZone() != _loader._stops[routeD[i + 1].first].getZone())
						modifiers.push_back(ZONE_PENALTY);
					else
						modifiers.push_back(NO_PENALTY);

					connect(routeD[i].first, routeD[i + 1].first, (*it).first, applyModifiers(distance, modifiers));
				}
			}
		}
	}

	double applyModifiers(double distance, vector<double> modifiers) {

		double distance_weight = DISTANCE_WEIGHT * distance;
		double time_weight = TIME_WEIGHT * (modifiers[0] * distance);
		double price_weight = PRICE_WEIGHT * (modifiers[1] * distance);

		return distance_weight + time_weight + price_weight;
	}

	void connect(string stop_src, string stop_dst, string line, double weight) {
		_graph.addEdge(&_loader._stops[stop_src], &_loader._stops[stop_dst], line, weight);
	}

	void connect2way(string stop_src, string stop_dst, double weight) {
		_graph.addEdge(&_loader._stops[stop_src], &_loader._stops[stop_dst], "walk", weight);
		_graph.addEdge(&_loader._stops[stop_dst], &_loader._stops[stop_src], "walk", weight);
	}
};

#endif