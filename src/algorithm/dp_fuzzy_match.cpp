
#include "trip_planner/algorithm/algorithm.h"

using namespace std;

namespace trip_planner {

	namespace algorithm {

		int dp_fuzzy_match(string pattern, string text) {

			pattern.insert(0, 1, ' ');
			text.insert(0, 1, ' ');

			vector<int> d(text.length());

			for (size_t j = 0; j < text.length(); ++j)
				d[j] = j;

			for (size_t i = 1; i < pattern.length(); ++i) {

				int old_val = d[0];

				d[0] = i;

				for (size_t j = 1; j < text.length(); ++j) {

					int new_val;

					if (pattern[i] == text[j])
						new_val = old_val;
					else
						new_val = 1 + utility::minimum(old_val, d[j], d[j - 1]);

					old_val = d[j];
					d[j] = new_val;
				}
			}

			return d[text.length() - 1];
		}
	}
}