
#include "trip_planner/data/line.h"

using namespace std;

using json = nlohmann::json;

namespace trip_planner {

	line::line() {}

	line::line(lcode_t code, pubcode_t pubcode, mode_t mode) : _code(code), _pubcode(pubcode), _mode(mode) {}

	lcode_t line::get_code() const {
		return _code;
	}

	pubcode_t line::get_pubcode() const {
		return _pubcode;
	}

	mode_t line::get_mode() const {
		return _mode;
	}

	route_t line::get_route(direction_t direction) const {
		return direction == ASCENDING ? _routeA : _routeD;
	}

	void line::set_route(direction_t direction, route_t route) {
		direction == ASCENDING ? _routeA = route : _routeD = route;
	}

	void to_json(json &j, const line &l) {
		j = json{ { "code", l._code },{"pubcode", l._pubcode},{ "mode", l._mode },{ "routeA", l._routeA },{ "routeD", l._routeD } };
	}

	void from_json(const json &j, line &l) {
		l._code = j["code"].get<lcode_t>();
		l._pubcode = j["pubcode"].get<pubcode_t>();
		l._mode = j["mode"].get<mode_t>();
		l._routeA = j["routeA"].get<route_t>();
		l._routeD = j["routeD"].get<route_t>();
	}
}