#ifndef TRIP_PLANNER_FORMATTING_H
#define TRIP_PLANNER_FORMATTING_H

#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include <string>
#include <locale>
#include <codecvt>
#include <unordered_map>
#include <algorithm>

namespace trip_planner {

	namespace utility {

		/** 
		 * Converts the characters of a string to uppercase.
		 * @param str String to be modified.
		 */
		void to_upper(std::string &str);

		/**
		 *	Removes any accented characters of a string.
		 *	@param str String to be modified.
		*/
		void remove_accented(std::string &str);

	}
}

#endif