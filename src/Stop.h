#ifndef STOP_H
#define STOP_H

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

typedef pair<double, double> coord_t;

class Stop {

	string	_zone;
	string	_scode;
	coord_t	_coords;

public:

	Stop() {
	}

	Stop(string zone, string scode) {
		_zone = zone;
		_scode = scode;
		_coords = make_pair(0.0, 0.0);
	}

	string getCode() const {
		return _scode;
	}

	string getZone() const {
		return _zone;
	}

	coord_t getCoords() const {
		return _coords;
	}

	void setCoords(coord_t coords) {
		_coords = coords;
	}

	friend void to_json(json &j, const Stop &s);
	friend void from_json(const json &j, Stop &s);
};

void to_json(json &j, const Stop &s) {
	j = json{ { "zone", s._zone },{ "code", s._scode },{ "coords", s._coords } };
}

void from_json(const json &j, Stop &s) {
	s._zone = j.at("zone").get<string>();
	s._scode = j.at("code").get<std::string>();
	s._coords = j.at("coords").get<coord_t>();
}

#endif