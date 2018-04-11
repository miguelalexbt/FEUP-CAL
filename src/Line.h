#ifndef LINE_H
#define LINE_H

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

/** Possible modes of transportation. */
typedef enum { null, BUS, SUBWAY, WALK }	mode_t;

/** Stop of a route and its distance to the next stop. */
typedef std::pair<std::string, double>		route_t;

/** Line from a transportation provider. */
class Line {

	/** Code. */
	std::string				_lcode;

	/** Mode of transport. */
	mode_t					_mode;

	/** Ascendant route. */
	std::vector<route_t>	_routeA;

	/** Descendant route. */
	std::vector<route_t>	_routeD;

public:

	/** Creates an instance of Line. */
	Line();

	/** Creates an instance of Line. 
	 *	@param mode Mode of transport.
	 *	@param lcode Code of the line. */
	Line(int mode, std::string lcode);

	/** @return Code of the line. */
	std::string getCode() const;
	
	/** @return Mode of the line. */
	mode_t getMode() const;

	/** @return Route of the line. 
	 *	@param dir Route's direction. */
	std::vector<route_t> getRoute(int dir) const;

	/** Adds a route to the line. 
	 *	@param route Route to add.
	 *	@param dir Direction of the route. */
	void addRoute(std::vector<route_t> route, int dir);

	/** Serialize a line to a JSON string.
	*	@param j JSON string.
	*	@param l Line to be serialized. */
	friend void to_json(nlohmann::json &j, const Line &l);

	/** Deserialize a line from a JSON string.
	*	@param j JSON string.
	*	@param l Line to be deserialized. */
	friend void from_json(const nlohmann::json &j, Line &l);
};

#endif