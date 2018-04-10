#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <iostream>
#include <sstream>
#include <map>

#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

#include "Stop.h"
#include "Line.h"
#include "Utils.h"

using namespace std;

using json = nlohmann::json;

typedef vector<coord_t>			linestring_t;
typedef vector<linestring_t>	multilinestring_t;

class Loader {

	string				_path;
	map<string, Line>	_lines;
	map<string, Stop>	_stops;

public:

	Loader() {
		_path = "http://www.stcp.pt/pt/itinerarium/callservice.php?action=";
	}

	void run() {

		//If files are empty, get info from the API
		if (utils::isEmpty("lines.json") || utils::isEmpty("stops.json")) {
			requestAPI();
			save();
		}
		else
			load();
	}

	json GET(string url) {

		ostringstream os;
		auto response = cpr::Get(cpr::Url{ url });
		os << response.text;

		return json::parse(os.str());
	}

	void requestAPI() {

		cout << " No data found. Starting HTTP GET requests... ";

		//Get bus lines
		requestLines(1);

		//Get subway lines
		requestLines(2);

		cout << " done." << endl;
	}

	void requestLines(int service) {

		json response = GET(_path + "lineslist&service=" + to_string(service));

		for (size_t i = 0; i < response["records"].size(); i++) {

			if (response["records"][i]["code"].get<string>().back() == 'M')
				continue;

			//New line
			Line line = Line(service, response["records"][i]["code"].get<string>());

			//Add line + get info
			if (_lines.find(line.getCode()) == _lines.end()) {
				_lines[line.getCode()] = line;

				requestStops(line.getCode(), 0);
				requestStops(line.getCode(), 1);
			}
		}
	}

	void requestStops(string lcode, int dir) {

		json response = GET(_path + "linestops&lcode=" + lcode + "&ldir=" + to_string(dir));

		vector<route_t> route;

		for (size_t i = 0; i < response["records"].size(); i++) {

			//New station
			Stop s(response["records"][i]["zone"].get<string>(), response["records"][i]["code"].get<string>());

			//Add station
			if (_stops.find(s.getCode()) == _stops.end())
				_stops[s.getCode()] = s;

			route.push_back(make_pair(s.getCode(), 0.0));
		}

		requestRoute(lcode, route, dir);

		_lines[lcode].addRoute(route, dir);
	}

	void requestRoute(string lcode, vector<route_t> &route, int dir) {

		json response = GET(_path + "linedraw&lcode=" + lcode + "&ldir=" + to_string(dir));

		for (size_t i = 0; i < response["route"].size(); i++) {

			json coords = json::parse(response["route"][i]["geomdesc"].get<string>());

			double distance = 0;

			if (coords["type"].get<string>() == "LineString")
				distance += utils::calcLineString(coords["coordinates"].get<linestring_t>());
			else if (coords["type"].get<string>() == "MultiLineString")
				distance += utils::calcMultiLineString(coords["coordinates"].get<multilinestring_t>());

			if (i < route.size())
				route[i].second = distance;
		}

		//Set stop's coordinates
		for (size_t i = 0; i < response["locations"].size(); i++) {

			json coords = json::parse(response["locations"][i]["geomdesc"].get<string>());

			string scode = response["locations"][i]["code"].get<string>();

			_stops[scode].setCoords(coords["coordinates"].get<coord_t>());
		}
	}

	void save() {

		cout << " Saving data for later uses... ";

		saveLines();
		saveStops();

		cout << " done." << endl;
	}

	void saveLines() {

		ofstream out("lines.json");

		json j = _lines;
		out << j;

		out.close();
	}

	void saveStops() {

		ofstream out("stops.json");

		json j = _stops;
		out << j;

		out.close();
	}

	void load() {

		cout << " Data found. Loading data... ";

		loadLines();
		loadStops();

		cout << " done." << endl;
	}

	void loadLines() {

		ifstream in("lines.json");

		json j;
		in >> j;
		_lines = j.get<map<string, Line>>();

		in.close();
	}

	void loadStops() {

		ifstream in("stops.json");

		json j;
		in >> j;
		_stops = j.get<map<string, Stop>>();

		in.close();
	}

	friend class Grid;
};

#endif