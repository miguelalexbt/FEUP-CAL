#ifndef UTILS_H
#define UTILS_H

#define _USE_MATH_DEFINES
#define EARTH_RADIUS 6371.0

#include <math.h>

using namespace std;

typedef pair<double, double>	coord_t;
typedef vector<coord_t>			linestring_t;
typedef vector<linestring_t>	multilinestring_t;

namespace utils {

	/* Algorithm to calculate distance between two points, using its coordinates */
	double haversineDistance(const coord_t p1, const coord_t p2);
	double degToRad(const double deg);

	/* Checks if a file is empty */
	bool isEmpty(string file);

	/* Calculates the distance of a LineString */
	double calcLineString(linestring_t line);

	/* Calculates the distance of a MultiLineString */
	double calcMultiLineString(multilinestring_t multiline);
}

double utils::haversineDistance(const coord_t p1, const coord_t p2) {

	double latRad1 = degToRad(p1.second);
	double latRad2 = degToRad(p2.second);
	double lonRad1 = degToRad(p1.first);
	double lonRad2 = degToRad(p2.first);

	double diffLa = latRad2 - latRad1;
	double doffLo = lonRad2 - lonRad1;

	double computation = asin(sqrt(sin(diffLa / 2.0) * sin(diffLa / 2.0) + cos(latRad1) * cos(latRad2) * sin(doffLo / 2.0) * sin(doffLo / 2.0)));
	return 2.0 * EARTH_RADIUS * computation;
}

double utils::degToRad(const double deg) {
	return (M_PI * deg) / 180.0;
}

bool utils::isEmpty(string file) {
	ifstream test(file);

	return test.peek() == ifstream::traits_type::eof();
}

double utils::calcLineString(linestring_t line) {

	double distance = 0.0;

	for (size_t i = 0; i < line.size() - 1; i++) {

		auto start = line[i];
		auto end = line[i + 1];

		distance += utils::haversineDistance(start, end);
	}

	return distance;
}

double utils::calcMultiLineString(multilinestring_t multiline) {

	double distance = 0.0;

	for (size_t i = 0; i < multiline.size(); i++) {
		distance += calcLineString(multiline[i]);
	}

	return distance;
}

#endif