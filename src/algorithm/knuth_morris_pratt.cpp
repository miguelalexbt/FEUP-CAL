
#include "trip_planner/algorithm/algorithm.h"

using namespace std;

namespace trip_planner {

	namespace algorithm {

		vector<int> compute_table(const string &pattern) {

			vector<int> table(pattern.length(), 0);

			table[0] = 0;
			int k = 0;

			for (size_t q = 1; q < pattern.length(); q++) {

				while (k > 0 && pattern[k] != pattern[q])
					k = table[k - 1];

				if (pattern[k] == pattern[q])
					k++;

				table[q] = k;
			}

			return table;
		}

		vector<int> knuth_morris_pratt(const string &pattern, const string &text) {

			vector<int> res;
			vector<int> table = compute_table(pattern);

			int q = 0;

			for (size_t i = 0; i < text.length(); i++) {

				while (q > 0 && pattern[q] != text[i])
					q = table[q - 1];

				if (pattern[q] == text[i])
					q++;

				if (q == pattern.length()) {
					res.push_back(i - q + 1);
					q = table[q - 1];
				}
			}

			return res;
		}
	}
}
