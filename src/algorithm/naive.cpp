
#include "trip_planner/algorithm/algorithm.h"

using namespace std;

namespace trip_planner {

	namespace algorithm {

		vector<int> naive(const string &pattern, const string &text) {

			vector<int> res;

			for (size_t i = 0; i < text.length(); i++) {

				if (i + pattern.length() > text.length())
					break;

				size_t j;

				for (j = 0; j < pattern.length(); j++) {
					if (pattern[j] != text[i + j])
						break;
				}

				if (j == pattern.length())
					res.push_back(i);
			}

			return res;
		}
	}
}