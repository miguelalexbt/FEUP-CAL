#ifndef TRIP_PLANNER_LINE_H
#define TRIP_PLANNER_LINE_H

#include <utility>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "trip_planner/type/data_fwd.h"

namespace trip_planner {

	class line {

	public:

		/** Creates an instance of Line. */
		line();

		/** Creates an instance of Line.
		*	@param code Code of the Line to create.
		*	@param pubcode Pubcode of the Line to create.
		*	@param mode Mode of transport. */
		line(lcode_t code, pubcode_t pubcode, mode_t mode);

		/** @return Code of the line. */
		lcode_t get_code() const;

		/** @return Pubcode of the line. */
		pubcode_t get_pubcode() const;

		/** @return Mode of the line. */
		mode_t get_mode() const;

		/** @return Route of the line.
		*	@param direction Route's direction. */
		route_t get_route(direction_t direction) const;

		/** Sets the route to the line.
		*	@param direction Direction of the route.
		*	@param route Route to add. */
		void set_route(direction_t direction, route_t route);

	private:

		/** Code. */
		lcode_t		_code;

		/** Alternative code (same as _code for the Bus transport). */
		pubcode_t	_pubcode;

		/** Mode of transport. */
		mode_t		_mode;

		/** Ascendant route. */
		route_t		_routeA;

		/** Descendant route. */
		route_t		_routeD;

		/** Serialize a line to a JSON string.
		*	@param j JSON string.
		*	@param l Line to be serialized. */
		friend void to_json(nlohmann::json &j, const line &l);

		/** Deserialize a line from a JSON string.
		*	@param j JSON string.
		*	@param l Line to be deserialized. */
		friend void from_json(const nlohmann::json &j, line &l);
	};
}

#endif
 