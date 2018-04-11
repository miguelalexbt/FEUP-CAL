#ifndef STOP_H
#define STOP_H

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

typedef std::pair<double, double> coord_t;

/** Stop of a line. */
class Stop {

	/** Zone. */
	std::string	_zone;

	/** Code. */
	std::string	_scode;

	/** Coordinates. */
	coord_t		_coords;

public:

	/** Creates an instance of Stop. */
	Stop();

	/** Creates an instance of Stop.
	 *	@param zone Zone of the stop.
	 *	@param scode Code of the stop.*/
	Stop(std::string zone, std::string scode);

	/** @return Code of the stop. */
	std::string getCode() const;

	/** @return Zone of the stop. */
	std::string getZone() const;

	/** @return Coordinates of the stop. */
	coord_t getCoords() const;

	/** Sets the coordinates of the stop.
	 *	@param coords New coordinates of the stop. */
	void setCoords(coord_t coords);

	/** Serialize a stop to a JSON string. 
	 *	@param j JSON string.
	 *	@param s Stop to be serialized. */
	friend void to_json(nlohmann::json &j, const Stop &s);

	/** Deserialize a stop from a JSON string.
	 *	@param j JSON string.
	 *	@param s Stop to be deserialized. */
	friend void from_json(const nlohmann::json &j, Stop &s);
};

#endif