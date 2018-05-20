
#include "trip_planner/algorithm/algorithm.h"

using namespace std;

namespace trip_planner {

	namespace algorithm {

		int dp_fuzzy_match_old(const string &pattern, const string &text) {

			vector<vector<int>> d(pattern.length() + 1, vector<int>(text.length() + 1));

			for (size_t i = 0; i <= pattern.length(); ++i) {

				for (size_t j = 0; j <= text.length(); ++j) {

					if (i == 0)
						d[i][j] = j;
					else if (j == 0)
						d[i][j] = i;
					else if (pattern[i - 1] == text[j - 1])
						d[i][j] = d[i - 1][j - 1];
					else
						d[i][j] = 1 + utility::minimum(d[i][j - 1], d[i - 1][j], d[i - 1][j - 1]);
				}
			}

			return d[pattern.length()][text.length()];
		}
	}
}
