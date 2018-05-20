#ifndef TRIP_PLANNER_HELPER_H
#define TRIP_PLANNER_HELPER_H

#include <string>
#include <algorithm>

namespace trip_planner {

	namespace utility {

		/**
		 * Calculates the minimum of three integers.
		 * @param val1 First value.
		 * @param val2 Second value.
		 * @param val3 Third value.
		 * @return Minimum of the three values.
		 */
		int minimum(const int &val1, const int &val2, const int &val3);

		/**
		 * Pops the last character of a string.
		 * @param str String to be modified.
		 * @return Modified string.
		 */
		std::string pop(std::string str);

		/**
		* Checks if the contents of the string are all numbers.
		* @param str String to be checked.
		* @return True if the content of the string is formed by numbers, false otherwise.
		*/
		bool is_number(const std::string &str);
	}
}

#endif