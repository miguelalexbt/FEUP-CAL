
#include "trip_planner/utility/io.h"

using namespace std;

using json = nlohmann::json;

namespace trip_planner {

	namespace utility {

		size_t write_curl(void *ptr, size_t size, size_t nmemb, string* data) {
			data->append((char*)ptr, size * nmemb);
			return size * nmemb;
		}


		json get(string url) {

			auto curl = curl_easy_init();
			if (curl) {
				curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

				string response_string;
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_curl);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

				curl_easy_perform(curl);
				curl_easy_cleanup(curl);

				return json::parse(response_string);
			}
			else
				throw runtime_error("making get request");
		}

		bool is_file_empty(string file_name) {
			ifstream test(file_name);
			return test.peek() == ifstream::traits_type::eof();
		}

		void clear_screen() {
#ifdef _WIN32
			system("cls");
#else
			system("clear");
#endif
		}
	}
}