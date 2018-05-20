
#include "trip_planner/algorithm/algorithm.h"

using namespace std;

namespace trip_planner {

	namespace algorithm {

		void heuristic(vector<int> &table, const string &pattern) {
			for (size_t i = 0; i < pattern.length() - 1; ++i) {
				char a = pattern[i];
				table[(int)a] = i;
			}
		}

		vector<int> boyer_moore_horspool(const string &pattern, const string &text) {

			const int alphabet_size = 256;

			vector<int> res;
			vector<int> table(alphabet_size, -1);

			heuristic(table, pattern);

			size_t i = 0;
			int j;
			while (i <= text.length() - pattern.length()) {

				j = (int) pattern.length() - 1;

				while (j >= 0 && pattern[j] == text[i + j])
					j--;

				if (j < 0)
					res.push_back(i);

				i += pattern.length() - 1;
				i -= table[text[i]];
			}

			return res;
		}
	}
}