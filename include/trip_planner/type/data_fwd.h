#ifndef TRIP_PLANNER_DATAFWD_H
#define TRIP_PLANNER_DATAFWD_H

#include <string>
#include <vector>
#include <map>

namespace trip_planner {

	class line;
	class stop;

	/** Existing criteria to choose from for a search. */
	typedef enum { DISTANCE, STOP, TIME, PRICE, REAL }			criteria_t;

	/** Existing algorithms to choose from. */
	typedef enum { DIJKSTRA, BELLMAN_FORD, JOHNSON, A_STAR }	algorithm_t;

	/** Possible modes of transportation. */
	typedef enum { BUS = 1, SUBWAY, WALK }						mode_t;

	/** Line directions */
	typedef enum { ASCENDING, DESCENDING }						direction_t;

	/** Line code */
	typedef std::string											lcode_t;

	/** Stop code */
	typedef std::string											scode_t;

	/** Line name */
	typedef std::string											lname_t;

	/** Stop name */
	typedef std::string											sname_t;

	/** For easier user comprehension, lines (both Bus and Subway) alternative names */
	typedef std::string											pubcode_t;

	/** Zone code */
	typedef std::string											zone_t;

	/** Stop of a route and its distance to the next stop. */
	typedef std::pair<scode_t, double>							node_t;

	/** Vector of nodes forming a route. */
	typedef std::vector<node_t>									route_t;

	/** Pair of geographical coordinates. */
	typedef std::pair<double, double>							coord_t;

	/** Route formed by geographical coordinates. */
	typedef std::vector<coord_t>								linestring_t;

	/** Route formed by multiple smaller routes */
	typedef std::vector<linestring_t>							multilinestring_t;

	/** Storage for all the existing lines. */
	typedef std::map<lcode_t, line>								line_data;

	/** Storage for all the existing stops. */
	typedef std::map<scode_t, stop>								stop_data;
}

#endif