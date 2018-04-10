#ifndef UTILS_H
#define UTILS_H

#define _USE_MATH_DEFINES

#include <fstream>
#include <math.h>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

typedef std::pair<double, double>	coord_t;
typedef std::vector<coord_t>		linestring_t;
typedef std::vector<linestring_t>	multilinestring_t;

namespace utils {

	nlohmann::json GET(std::string url);

	/* Algorithm to calculate distance between two points, using its coordinates */
	double haversineDistance(const coord_t p1, const coord_t p2);
	double degToRad(const double deg);

	/* Checks if a file is empty */
	bool isEmpty(std::string file);

	/* Calculates the distance of a LineString */
	double calcLineString(linestring_t line);

	/* Calculates the distance of a MultiLineString */
	double calcMultiLineString(multilinestring_t multiline);
}

#endif