
#include "trip_planner/algorithm/algorithm.h"

using namespace std;

namespace trip_planner {

	namespace algorithm {

		vector<int> rabin_karp(const string &pattern, const string &text) {

			vector<int> res;

			hash<string> hasher;

			size_t hpattern = hasher(pattern);

			for (size_t i = 0; i < text.length() - pattern.length(); ++i) {

				string substr = text.substr(i, pattern.length());
				size_t hs = hasher(substr);

				if (hpattern == hs && substr == pattern)
					res.push_back(i);
			}

			return res;
		}
	}
}