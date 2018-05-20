
#include "trip_planner/algorithm/algorithm.h"

using namespace std;

namespace trip_planner {

	namespace algorithm {

		void bad_char(vector<int> &table, const string &pattern) {
			for (size_t i = 0; i < pattern.length(); ++i)
				table[(int)pattern[i]] = i;
		}

		void good_suffix(vector<int> &table, const string &pattern) {

			vector<int> f(pattern.length() + 1);

			size_t i = pattern.length(), j = pattern.length() + 1;
			f[i] = j;

			while (i > 0) {
				while (j <= pattern.length() && pattern[i - 1] != pattern[j - 1]) {
					if (table[j] == 0) table[j] = j - i;
					j = f[j];
				}

				i--;
				j--;

				f[i] = j;
			}

			j = f[0];

			for (i = 0; i <= pattern.length(); i++) {
				if (table[i] == 0) table[i] = j;
				if (i == j) j = f[j];
			}
		}

		vector<int> boyer_moore(const string &pattern, const string &text) {

			const int alphabet_size = 256;

			vector<int> res;
			vector<int> bad_char_table(alphabet_size, -1);
			vector<int> good_suffix_table(alphabet_size, 0);

			bad_char(bad_char_table, pattern);
			good_suffix(good_suffix_table, pattern);

			size_t i = 0;
			int j;
			while (i <= text.length() - pattern.length()) {

				j = (int) pattern.length() - 1;

				while (j >= 0 && pattern[j] == text[i + j])
					j--;

				if (j < 0) {
					res.push_back(i);
					i += good_suffix_table[0];
				}
				else
					i += max(good_suffix_table[j + 1], (int)j - bad_char_table[text[i + j]]);
			}

			return res;
		}
	}
}