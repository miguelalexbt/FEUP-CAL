
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
				{ L'á', L'A' },{ L'à', L'A' },{ L'ã', L'A' },{ L'â', L'A' }, { L'Á', L'A' },{ L'À', L'A' },{ L'Ã', L'A' },{ L'Â', L'A' },
				{ L'é', L'E' },{ L'è', L'E' },{ L'ê', L'E' },{ L'É', L'E' },{ L'È', L'E' },{ L'Ê', L'E' },
				{ L'Í', L'I' },{ L'Ì', L'I' },{ L'Î', L'I' },{ L'í', L'I' },{ L'ì', L'I' },{ L'î', L'I' },
				{ L'Ó', L'O' },{ L'Ò', L'O' },{ L'Õ', L'O' },{ L'Ô', L'O' },{ L'ó', L'O' },{ L'ò', L'O' },{ L'õ', L'O' },{ L'ô', L'O' },
				{ L'Ú', L'U' },{ L'Ù', L'U' },{ L'Û', L'U' },{ L'ú', L'U' },{ L'ù', L'U' },{ L'û', L'U' },
				{ L'Ç', L'C' },{ L'ç', L'C' },{ L'º', L'X'}
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