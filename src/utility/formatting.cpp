
#include "trip_planner/utility/formatting.h"

using namespace std;

namespace trip_planner {

	namespace utility {

		void to_upper(string &str) {
			transform(str.begin(), str.end(), str.begin(), toupper);
		}

		void remove_accented(string &str) {

			wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

			unordered_multimap<wchar_t, wchar_t> dictionary = {
				{ L'�', L'A' },{ L'�', L'A' },{ L'�', L'A' },{ L'�', L'A' }, { L'�', L'A' },{ L'�', L'A' },{ L'�', L'A' },{ L'�', L'A' },
				{ L'�', L'E' },{ L'�', L'E' },{ L'�', L'E' },{ L'�', L'E' },{ L'�', L'E' },{ L'�', L'E' },
				{ L'�', L'I' },{ L'�', L'I' },{ L'�', L'I' },{ L'�', L'I' },{ L'�', L'I' },{ L'�', L'I' },
				{ L'�', L'O' },{ L'�', L'O' },{ L'�', L'O' },{ L'�', L'O' },{ L'�', L'O' },{ L'�', L'O' },{ L'�', L'O' },{ L'�', L'O' },
				{ L'�', L'U' },{ L'�', L'U' },{ L'�', L'U' },{ L'�', L'U' },{ L'�', L'U' },{ L'�', L'U' },
				{ L'�', L'C' },{ L'�', L'C' },{ L'�', L'X'}
			};

			wstring wide = converter.from_bytes(str);

			for (size_t i = 0; i < wide.length(); ++i) {

				unordered_map<wchar_t, wchar_t>::iterator it;

				if ((it = dictionary.find(wide[i])) != dictionary.end()) {

					if ((*it).second == L'X')
						wide.erase(wide.begin() + i);
					else
						wide[i] = (*it).second;

				}
			}

			str = converter.to_bytes(wide);
		}
	}
}