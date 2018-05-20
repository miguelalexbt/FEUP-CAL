#ifndef TRIP_PLANNER_IO_H
#define TRIP_PLANNER_IO_H

#include <fstream>
#include <string>

#include "curl/curl.h"
#include "nlohmann/json.hpp"

namespace trip_planner {

	namespace utility {

		/** Makes a GET request to a given URL.
		*	@param url URL of the API. */
		nlohmann::json get(std::string url);

		/** Checks if a file is empty.
		*	@param file_name File to be checked.
		*	@return True if the file is empty, false otherwise. */
		bool is_file_empty(std::string file_name);

		/** Clear the screen. */
		void clear_screen();
	}
}

#endif