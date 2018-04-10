
#include "Loader.h"

using namespace std;

using json = nlohmann::json;

Loader::Loader() {
	_path = "http://www.stcp.pt/pt/itinerarium/callservice.php?action=";
}

void Loader::run() {

	if (utils::isEmpty("lines.json") || utils::isEmpty("stops.json")) {
		_requestAPI();
		_save();
	}
	else
		_load();
}

void Loader::_requestAPI() {

	cout << " No data found. Starting HTTP GET requests... ";

	//Get bus lines
	_requestLines(1);

	//Get subway lines
	_requestLines(2);

	cout << " done." << endl;
}

void Loader::_requestLines(int service) {

	json response = utils::GET(_path + "lineslist&service=" + to_string(service));

	for (size_t i = 0; i < response["records"].size(); i++) {

		if (response["records"][i]["code"].get<string>().back() == 'M')
			continue;

		//New line
		Line line = Line(service, response["records"][i]["code"].get<string>());

		//Add line + get info
		if (_lines.find(line.getCode()) == _lines.end()) {
			_lines[line.getCode()] = line;

			_requestStops(line.getCode(), 0);
			_requestStops(line.getCode(), 1);
		}
	}
}

void Loader::_requestStops(string lcode, int dir) {

	json response = utils::GET(_path + "linestops&lcode=" + lcode + "&ldir=" + to_string(dir));

	vector<route_t> route;

	for (size_t i = 0; i < response["records"].size(); i++) {

		//New station
		Stop s(response["records"][i]["zone"].get<string>(), response["records"][i]["code"].get<string>());

		//Add station
		if (_stops.find(s.getCode()) == _stops.end())
			_stops[s.getCode()] = s;

		route.push_back(make_pair(s.getCode(), 0.0));
	}

	_requestRoute(lcode, route, dir);

	_lines[lcode].addRoute(route, dir);
}

void Loader::_requestRoute(string lcode, vector<route_t> &route, int dir) {

	json response = utils::GET(_path + "linedraw&lcode=" + lcode + "&ldir=" + to_string(dir));

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

void Loader::_save() {

	cout << " Saving data for later uses... ";

	ofstream out_l("lines.json");
	json j_l = _lines;
	out_l << j_l;
	out_l.close();

	ofstream out_s("stops.json");
	json j_s = _stops;
	out_s << j_s;
	out_s.close();

	cout << " done." << endl;
}

void Loader::_load() {

	cout << " Data found. Loading data... ";

	ifstream in_l("lines.json");
	json j_l;
	in_l >> j_l;
	_lines = j_l.get<map<string, Line>>();
	in_l.close();

	ifstream in_s("stops.json");
	json j_s;
	in_s >> j_s;
	_stops = j_s.get<map<string, Stop>>();
	in_s.close();

	cout << " done." << endl;
}