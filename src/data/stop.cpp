
#include "trip_planner/data/stop.h"

using namespace std;

using json = nlohmann::json;

namespace trip_planner {

	stop::stop() {}

	stop::stop(scode_t code, sname_t name, zone_t zone) : _code(code), _name(name), _zone(zone) {
		utility::to_upper(_name);
		utility::remove_accented(_name);
	}

	scode_t stop::get_code() const {
		return _code;
	}

	sname_t stop::get_name() const {
		return _name;
	}

	zone_t stop::get_zone() const {
		return _zone;
	}

	coord_t stop::get_coords() const {
		return _coords;
	}

	void stop::set_coords(coord_t coords) {
		_coords = coords;
	}

	void to_json(nlohmann::json &j, const stop &s) {
		j = json{ { "code", s._code },{ "name", s._name},{ "zone", s._zone },{ "coords", s._coords } };
	}

	void from_json(const nlohmann::json &j, stop &s) {
		s._code = j["code"].get<scode_t>();
		s._name = j["name"].get<sname_t>();
		s._zone = j["zone"].get<zone_t>();
		s._coords = j["coords"].get<coord_t>();
	}
}