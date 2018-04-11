
#include "Line.h"

using namespace std;

using json = nlohmann::json;

Line::Line() {}

Line::Line(int mode, string lcode) {
	_mode = static_cast<mode_t>(mode);
	_lcode = lcode;
}

string Line::getCode() const {
	return _lcode;
}

mode_t Line::getMode() const {
	return _mode;
}

vector<route_t> Line::getRoute(int dir) const {

	if (dir == 0)
		return _routeA;
	else
		return _routeD;
}

void Line::addRoute(vector<route_t> route, int dir) {

	if (dir == 0)
		_routeA = route;
	else if (dir == 1)
		_routeD = route;
}

void to_json(json &j, const Line &l) {
	j = json{ { "code", l._lcode },{ "mode", l._mode },{ "routeA", l._routeA },{ "routeD", l._routeD } };
}

void from_json(const json &j, Line &l) {
	l._lcode = j.at("code").get<string>();
	l._mode = j.at("mode").get<mode_t>();
	l._routeA = j.at("routeA").get<vector<route_t>>();
	l._routeD = j.at("routeD").get<vector<route_t>>();
}
