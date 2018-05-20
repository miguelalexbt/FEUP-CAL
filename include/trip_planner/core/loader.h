#ifndef TRIP_PLANNER_LOADER_H
#define TRIP_PLANNER_LOADER_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "trip_planner/core/database.h"
#include "trip_planner/utility/io.h"
#include "trip_planner/utility/geographic.h"

namespace trip_planner {

	class loader {

	public:

		/** Loader's instance */
		static loader &instance();

		/** Runs the loader. */
		void load();

	private:

		/** Creates an instance of Loader. */
		loader();

		/** Makes API calls to get lines.
		*	@param mode Mode of transportation. */
		void request_lines(mode_t mode);

		/** Makes API calls to get stops.
		*	@param code Code of the line.
		*	@param direction Direction of the line. */
		void request_stops(lcode_t code, direction_t direction);

		/** Makes API calls to get the route of a line.
		*	@param code Code of the line.
		*	@param direction Direction of the line.
		*	@param route Route to be completed. */
		void request_route(lcode_t code, direction_t direction, route_t &route);

		/** Saves current loaded information in a file, for later use. */
		void save_to_file();

		/** Loads previous loaded information. */
		void load_from_file();
	};
}

#endif