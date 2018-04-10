#ifndef STOP_H
#define STOP_H

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

typedef std::pair<double, double> coord_t;

class Stop {

	std::string	_zone;
	std::string	_scode;
	coord_t		_coords;

public:

	Stop();

	Stop(std::string zone, std::string scode);

	std::string getCode() const;

	std::string getZone() const;

	coord_t getCoords() const;

	void setCoords(coord_t coords);

	friend void to_json(nlohmann::json &j, const Stop &s);
	friend void from_json(const nlohmann::json &j, Stop &s);
};

#endif