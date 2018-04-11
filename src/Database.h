#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <chrono>
#include <conio.h>

#include "Graph.h"
#include "Loader.h"

/** Class responsible for generating a graph and running the algorithms. */
class Database {

	/** Graph. */
	Graph<Stop*>	_graph;

	/** Loader. */
	Loader			_loader;

	/** Runs the loader. */
	void _load();

	/** Generates a graph, followin the criteria. 
	 *	@param option Criteria chosen by the user. */
	void _generateGraph(std::string option);

	/** Runs the algorithm. 
	 *	@param start Starting stop.
	 *	@param end Destination stop.
	 *	@param algorithm Algorithm to run. */
	void _runAlgorithm(std::string start, std::string end, int algorithm);

	/** Parse the results from running the algorithm and displays it, in an user-friendly way.
	 *	@param path Best shortest path calculated by the algorithm. */
	void _parseResults(std::pair<std::vector<Stop*>, std::vector<std::string>> path);

	/** Adds vertices to the graph. */
	void _addVertices();

	/** Adds edges to the graph, following the criteria.
	 *	@param option Criteria chosen by the user. */
	void _addEdges(int option);

	/** Applies modifiers to the initial weight, depending on the criteria.
	 *	@param option Criteria chosen by the user.
	 *	@param start Starting stop.
	 *	@param end Destination stop.
	 *	@param distance Initial weight.
	 *	@param mode Traveling mode. 
	 *	@return Returns the modified weight. */
	double _applyModifiers(int option, std::string start, std::string end, double distance, mode_t mode);

	/** Connects two stops, adding an one-way edge between them.
	 *	@param stop_src Starting stop.
	 *	@param stop_dst	Destination stop.
	 *	@param line Line that connects the two stops.
	 *	@param weight Weight of the edge. */
	void _connect(std::string stop_src, std::string stop_dst, std::string line, double weight);

	/** Connects two stops, adding an bidirectional edge between them.
	*	@param stop_src Starting stop.
	*	@param stop_dst	Destination stop.
	*	@param weight Weight of the edge. */
	void _connect2way(std::string stop_src, std::string stop_dst, double weight);

public:

	/** Creates an instance of Database. */
	Database() {};

	/** Runs the algorithm specified by the user.
	 *	@param start Starting stop.
	 *	@param end Destination stop.
	 *	@param option Criteria used for best path.
	 *	@param algorithm Algorithm the user wants to run. */
	void run(std::string start, std::string end, std::string option, int algorithm);
};

#endif