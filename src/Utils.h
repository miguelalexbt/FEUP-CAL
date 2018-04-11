#ifndef UTILS_H
#define UTILS_H

#define _USE_MATH_DEFINES

#include <fstream>
#include <math.h>

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

/** Coordinates (longitude, latitude). */
typedef std::pair<double, double>	coord_t;

/** LineString. */
typedef std::vector<coord_t>		linestring_t;

/** MultiLineString. */
typedef std::vector<linestring_t>	multilinestring_t;

/** Utility functions. */
namespace utils {

	/** Makes a GET request to a given URL.
	 *	@param url URL of the API. */
	nlohmann::json GET(std::string url);

	/** Algorithm to calculate distance between two points, using its coordinates.
	 *	@param p1 Coordinates of starting point.
	 *	@param p2 Coordinates of destination point
	 *	@return Distance between the points. */
	double haversineDistance(const coord_t p1, const coord_t p2);

	/** Converts degrees to radians.
	 *	@param deg Degrees to be converted.
	 *	@return Degrees converted to radians. */
	double degToRad(const double deg);

	/** Checks if a file is empty.
	 *	@param file File to be checked.
	 *	@return True if the file is empty, false otherwise. */
	bool isEmpty(std::string file);

	/** Calculates the distance of a LineString.
	 *	@param line LineString to be measured.
	 *	@return Distance of the LineString. */
	double calcLineString(linestring_t line);

	/** Calculates the distance of a MultiLineString.
	 *	@param multiline MultiLineString to be measured.
	 *	@return Distance of MultiLineString. */
	double calcMultiLineString(multilinestring_t multiline);
}

#endif