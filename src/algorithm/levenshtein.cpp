
#include "trip_planner/algorithm/algorithm.h"

using namespace std;

namespace trip_planner {

	namespace algorithm {

		int	levenshtein(const string &pattern, const string &text) {

			if (pattern.length() == 0) return text.length();
			if (text.length() == 0) return pattern.length();

			int cost = pattern.back() != text.back();

			return utility::minimum(levenshtein(utility::pop(pattern), text) + 1, 
				levenshtein(pattern, utility::pop(text)) + 1, 
				levenshtein(utility::pop(pattern), utility::pop(text)) + cost);
		}
	}
}
