#ifndef TRIP_PLANNER_GEOGRAPHIC_H
#define TRIP_PLANNER_GEOGRAPHIC_H

#include "trip_planner/type/data_fwd.h"

namespace trip_planner {

	namespace utility {

		/**
		* Calculates the distance between two coordinates.
		* @param p1 Coordinates of starting point.
		* @param p2 Coordinates of destination point.
		* @return Distance between the two points.
		*/
		double haversine_distance(coord_t p1, coord_t p2);

		/**
		* Converts degrees to radians.
		* @param deg Degrees to be converted.
		* @return Degrees converted to radians.
		*/
		double deg_to_rad(double deg);

		/**
		 * Calculates the distance of a LineString.
		 * @param line LineString to be measured.
		 * @return Distance of the LineString. 
		 */
		double calc_linestring(linestring_t linestring);

		/** 
		 * Calculates the distance of a MultiLineString.
		 * @param multiline MultiLineString to be measured.
		 * @return Distance of MultiLineString. 
		 */
		double calc_multilinestring(multilinestring_t multilinestring);
	}
}

#endif