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

class Loader {

	std::string					_path;
	std::map<std::string, Line>	_lines;
	std::map<std::string, Stop>	_stops;

	void _requestAPI();

	void _requestLines(int service);

	void _requestStops(std::string lcode, int dir);

	void _requestRoute(std::string lcode, std::vector<route_t> &route, int dir);

	void _save();

	void _load();

public:

	Loader();

	void run();

	friend class Database;
};

#endif