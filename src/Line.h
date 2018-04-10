#ifndef LINE_H
#define LINE_H

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

typedef enum { null, BUS, SUBWAY }	mode_t;
typedef pair<string, double>		route_t;

class Line {

	string			_lcode;
	mode_t			_mode;
	vector<route_t>	_routeA;
	vector<route_t>	_routeD;

public:

	Line() {
	}

	Line(int mode, string lcode) {
		_mode = static_cast<mode_t>(mode);
		_lcode = lcode;
	}

	string getCode() const {
		return _lcode;
	}

	mode_t getMode() const {
		return _mode;
	}

	vector<route_t> getRoute(int dir) const {

		if (dir == 0)
			return _routeA;
		else
			return _routeD;
	}

	void addRoute(vector<route_t> route, int dir) {

		if (dir == 0)
			_routeA = route;
		else if (dir == 1)
			_routeD = route;
	}

	friend void to_json(json &j, const Line &s);
	friend void from_json(const json &j, Line &s);
};

void to_json(json &j, const Line &s) {
	j = json{ { "code", s._lcode },{ "mode", s._mode },{ "routeA", s._routeA },{ "routeD", s._routeD } };
}

void from_json(const json &j, Line &s) {
	s._lcode = j.at("code").get<string>();
	s._mode = j.at("mode").get<mode_t>();
	s._routeA = j.at("routeA").get<vector<route_t>>();
	s._routeD = j.at("routeD").get<vector<route_t>>();
}

#endif