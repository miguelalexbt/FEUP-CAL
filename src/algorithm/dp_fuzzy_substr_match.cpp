
#include "trip_planner/algorithm/algorithm.h"

using namespace std;

namespace trip_planner {

	namespace algorithm {

		int dp_fuzzy_substr_match(const string &pattern, const string &text) {

			if (pattern.length() <= 1 || text.length() == 0)
				return -1;

			vector<int> first_row(text.length() + 1, 0);

			for (size_t i = 0; i < pattern.length(); ++i) {

				vector<int> second_row(1, i + 1);

				for (size_t j = 0; j < text.length(); ++j) {
					int cost = pattern[i] != text[j];
					second_row.push_back(utility::minimum(first_row[j + 1] + 1, second_row[j] + 1, first_row[j] + cost));
				}

				first_row.swap(second_row);
			}

			return *min_element(first_row.begin(), first_row.end());
		}
	}
}