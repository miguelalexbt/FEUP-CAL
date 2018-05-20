
#include "trip_planner/utility/helper.h"

using namespace std;

namespace trip_planner {

	namespace utility {

		int minimum(const int &val1, const int &val2, const int &val3) {
			return min(val1, min(val2, val3));
		};

		string pop(string str) {
			return str.substr(0, str.length() - 1);
		};

		bool is_number(const string &str) {
			return !str.empty() && all_of(str.begin(), str.end(), isdigit);
		}
	}
}