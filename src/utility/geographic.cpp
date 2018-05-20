
#include "trip_planner/utility/geographic.h"

namespace trip_planner {

	namespace utility {

		namespace {
			const double EARTH_RADIUS = 6371.0;
			const double PI = atan(1) * 4;
		}

		double haversine_distance(const coord_t p1, const coord_t p2) {
			double lat_diff = deg_to_rad(p2.second) - deg_to_rad(p1.second);
			double lon_diff = deg_to_rad(p2.first) - deg_to_rad(p1.first);

			double computation = asin(sqrt(pow(sin(lat_diff / 2.0), 2.0) +
				cos(p1.second) * cos(p2.second) * pow(sin(lon_diff / 2.0), 2.0)));

			return 2.0 * EARTH_RADIUS * computation;
		}

		double deg_to_rad(double deg) {
			return (PI * deg) / 180.0;
		}

		double calc_linestring(linestring_t linestring) {

			double distance = 0.0;

			for (size_t i = 0; i < linestring.size() - 1; ++i)
				distance += haversine_distance(linestring[i], linestring[i + 1]);

			return distance;
		}

		double calc_multilinestring(multilinestring_t multilinestring) {

			double distance = 0.0;

			for (size_t i = 0; i < multilinestring.size(); ++i)
				distance += calc_linestring(multilinestring[i]);

			return distance;
		}
	}
}