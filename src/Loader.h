#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <iostream>
#include <sstream>
#include <map>

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

#include "Stop.h"
#include "Line.h"
#include "Utils.h"

typedef std::vector<coord_t>		linestring_t;
typedef std::vector<linestring_t>	multilinestring_t;

/** Class responsible for loading the information regarding stops and lines. */
class Loader {

	/** URL path for the STCP's API. */
	std::string					_path;

	/** Loaded lines. */
	std::map<std::string, Line>	_lines;

	/** Loaded stops. */
	std::map<std::string, Stop>	_stops;

	void _requestAPI();

	/** Makes API calls to get lines.
	 *	@param service Mode of transportation. */
	void _requestLines(int service);

	/** Makes API calls to get stops. 
	 *	@param lcode Code of the line.
	 *	@param dir Direction of the line. */
	void _requestStops(std::string lcode, int dir);

	/** Makes API calls to get the route of a line. 
	 *	@param lcode Code of the line.
	 *	@param route Route to be completed.
	 *	@param dir Direction of the line. */
	void _requestRoute(std::string lcode, std::vector<route_t> &route, int dir);

	/** Saves current loaded information in a file, for later use. */
	void _save();

	/** Loads previous loaded information. */
	void _load();

public:

	/** Creates an instance of Loader. */
	Loader();

	/** Runs the loader. */
	void run();

	friend class Database;
};

#endif