#ifndef LINE_H
#define LINE_H

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

typedef enum { null, BUS, SUBWAY }		mode_t;
typedef std::pair<std::string, double>	route_t;

class Line {

	std::string				_lcode;
	mode_t					_mode;
	std::vector<route_t>	_routeA;
	std::vector<route_t>	_routeD;

public:

	Line();

	Line(int mode, std::string lcode);

	std::string getCode() const;

	mode_t getMode() const;

	std::vector<route_t> getRoute(int dir) const;

	void addRoute(std::vector<route_t> route, int dir);

	friend void to_json(nlohmann::json &j, const Line &s);
	friend void from_json(const nlohmann::json &j, Line &s);
};

#endif