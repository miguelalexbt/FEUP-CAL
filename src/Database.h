#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <chrono>
#include <conio.h>

#include "Graph.h"
#include "Loader.h"

class Database {

	Graph<Stop*>	_graph;
	Loader			_loader;

	void _load();

	void _generateGraph(std::string option);

	void _runAlgorithm(std::string start, std::string end, int algorithm);

	void _parseResults(std::pair<std::vector<Stop*>, std::vector<std::string>> path);

	void _addVertices();

	void _addEdges(int option);

	double _applyModifiers(int option, string start, string end, double distance, mode_t mode);

	void _connect(std::string stop_src, std::string stop_dst, std::string line, double weight);

	void _connect2way(std::string stop_src, std::string stop_dst, double weight);

public:

	Database() {};

	void run(std::string start, std::string end, std::string option, int algorithm);
};

#endif