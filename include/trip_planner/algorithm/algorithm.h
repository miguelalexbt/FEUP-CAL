#ifndef TRIP_PLANNER_ALGORITHM_H
#define TRIP_PLANNER_ALGORITHM_H

#include <vector>
#include <algorithm>

#include "trip_planner/utility/helper.h"

namespace trip_planner {

	namespace algorithm {

		/** Naive algorithm.
		*	@param pattern Pattern to search for.
		*	@param text Text to search on.
		*	@return Vector containing the occurrence indexes of the pattern in the text. */
		std::vector<int> naive(const std::string &pattern, const std::string &text);

		/**  Knuth-Morris-Pratt algorithm.
		*	@param pattern Pattern to search for.
		*	@param text Text to search on.
		*	@return Vector containing the occurrence indexes of the pattern in the text. */
		std::vector<int> knuth_morris_pratt(const std::string &pattern, const std::string &text);

		/** Rabin Karp algorithm.
		*	@param pattern Pattern to search for.
		*	@param text Text to search on.
		*	@return Vector containing the occurrence indexes of the pattern in the text.*/
		std::vector<int> rabin_karp(const std::string &pattern, const std::string &text);

		/** Boyer Moore algorithm.
		*	@param pattern Pattern to search for.
		*	@param text Text to search on.
		*	@return Vector containing the occurrence indexes of the pattern in the text.*/
		std::vector<int> boyer_moore(const std::string &pattern, const std::string &text);

		/** Boyer Moore Horspool algorithm.
		*	@param pattern Pattern to search for.
		*	@param text Text to search on.
		*	@return Vector containing the occurrence indexes of the pattern in the text. */
		std::vector<int> boyer_moore_horspool(const std::string &pattern, const std::string &text);

		/** Levenshtein algorithm.
		*	@param pattern Pattern to search for.
		*	@param text Text to search on.
		*	@return Edit distance. */
		int	levenshtein(const std::string &pattern, const std::string &text);

		/** dp_fuzzy_match_old algorithm.
		*	@param pattern Pattern to search for.
		*	@param text Text to search on.
		*	@return Edit distance. */
		int dp_fuzzy_match_old(const std::string &pattern, const std::string &text);

		/** dp_fuzzy_match (improved version of dp_fuzzy_match_old) algorithm.
		*	@param pattern Pattern to search for.
		*	@param text Text to search on.
		*	@return Edit distance. */
		int dp_fuzzy_match(std::string pattern, std::string text);

		/** dp_fuzzy_substr_match algorithm.
		*	@param pattern Pattern to search for.
		*	@param text Text to search on.
		*	@return Edit distance. */
		int dp_fuzzy_substr_match(const std::string &pattern, const std::string &text);
	}
}	

#endif