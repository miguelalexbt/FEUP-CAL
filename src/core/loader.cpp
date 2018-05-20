
#include "trip_planner/core/loader.h"

using namespace std;

using json = nlohmann::json;

namespace trip_planner {

	namespace {
		const string BASE_URL = "http://www.stcp.pt/pt/itinerarium/callservice.php?action=";
		const string LINES_ENDPOINT = "lineslist&service=";
		const string STOPS_ENDPOINT = "linestops&lcode=";
		const string ROUTE_ENDPOINT = "linedraw&lcode=";
		const string DIRECTION_CHOICE = "&ldir=";

		const string LINES_FILE_PATH = "misc\\lines.json";
		const string STOPS_FILE_PATH = "misc\\stops.json";
	}

	loader::loader() {}

	loader &loader::instance() {
		static loader instance;
		return instance;
	}

	void loader::load() {

		if (utility::is_file_empty(LINES_FILE_PATH) || utility::is_file_empty(STOPS_FILE_PATH)) {
			request_lines(BUS);
			request_lines(SUBWAY);
			save_to_file();
		}
		else {
			load_from_file();
		}
	}

	void loader::request_lines(mode_t mode) {

		json response = utility::get(BASE_URL + LINES_ENDPOINT + to_string(mode));

		for (size_t i = 0; i < response["records"].size(); i++) {

			if (response["records"][i]["code"].get<string>().back() == 'M')
				continue;

			line new_line(response["records"][i]["code"].get<lcode_t>(), response["records"][i]["pubcode"].get<pubcode_t>(), mode);

			if (database::instance()._lines.find(new_line.get_code()) == database::instance()._lines.end()) {
				database::instance()._lines[new_line.get_code()] = new_line;

				request_stops(new_line.get_code(), ASCENDING);
				request_stops(new_line.get_code(), DESCENDING);
			}
		}
	}

	void loader::request_stops(lcode_t code, direction_t direction) {

		json response = utility::get(BASE_URL + STOPS_ENDPOINT + code + DIRECTION_CHOICE + to_string(direction));

		route_t route;

		for (size_t i = 0; i < response["records"].size(); ++i) {

			stop new_stop(response["records"][i]["code"].get<scode_t>(), response["records"][i]["name"].get<sname_t>(), response["records"][i]["zone"].get<zone_t>());

			if (database::instance()._stops.find(new_stop.get_code()) == database::instance()._stops.end())
				database::instance()._stops[new_stop.get_code()] = new_stop;

			route.push_back({ new_stop.get_code(), 0.0 });
		}

		request_route(code, direction, route);

		database::instance()._lines[code].set_route(direction, route);
	}

	void loader::request_route(lcode_t code, direction_t direction, route_t &route) {

		json response = utility::get(BASE_URL + ROUTE_ENDPOINT + code + DIRECTION_CHOICE + to_string(direction));

		for (size_t i = 0; i < response["route"].size(); ++i) {

			json geo_json = json::parse(response["route"][i]["geomdesc"].get<string>());

			double distance = 0.0;

			if (geo_json["type"].get<string>() == "LineString")
				distance += utility::calc_linestring(geo_json["coordinates"].get<linestring_t>());
			else
				distance += utility::calc_multilinestring(geo_json["coordinates"].get<multilinestring_t>());

			if (i < route.size())
				route[i].second = distance;
		}

		for (size_t i = 0; i < response["locations"].size(); ++i) {

			json coordinates = json::parse(response["locations"][i]["geomdesc"].get<string>());

			scode_t code = response["locations"][i]["code"].get<string>();

			database::instance()._stops[code].set_coords(coordinates["coordinates"].get<coord_t>());
		}
	}

	void loader::save_to_file() {

		cout << " Saving data for later uses...";

		ofstream out_lines(LINES_FILE_PATH);
		json json_lines = database::instance()._lines;
		out_lines << json_lines;
		out_lines.close();

		ofstream out_stops(STOPS_FILE_PATH);
		json json_stops = database::instance()._stops;
		out_stops << json_stops;
		out_stops.close();

		cout << " done." << endl;
	}

	void loader::load_from_file() {

		cout << " Data found. Loading data...";

		ifstream in_lines(LINES_FILE_PATH);
		json json_lines;
		in_lines >> json_lines;
		database::instance()._lines = json_lines.get<map<lcode_t, line>>();
		in_lines.close();

		ifstream in_stops(STOPS_FILE_PATH);
		json json_stops;
		in_stops >> json_stops;
		database::instance()._stops = json_stops.get<map<scode_t, stop>>();
		in_stops.close();

		cout << " done." << endl;
	}
}