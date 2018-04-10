
#include "Stop.h"

using namespace std;

using json = nlohmann::json;

Stop::Stop() {}

Stop::Stop(string zone, string scode) {
	_zone = zone;
	_scode = scode;
	_coords = make_pair(0.0, 0.0);
}

string Stop::getCode() const {
	return _scode;
}

string Stop::getZone() const {
	return _zone;
}

coord_t Stop::getCoords() const {
	return _coords;
}

void Stop::setCoords(coord_t coords) {
	_coords = coords;
}

void to_json(json &j, const Stop &s) {
	j = json{ { "zone", s._zone },{ "code", s._scode },{ "coords", s._coords } };
}

void from_json(const json &j, Stop &s) {
	s._zone = j.at("zone").get<string>();
	s._scode = j.at("code").get<std::string>();
	s._coords = j.at("coords").get<coord_t>();
}