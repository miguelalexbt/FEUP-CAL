#ifndef TRIP_PLANNER_STOP_H
#define TRIP_PLANNER_STOP_H

#include <utility>
#include <string>

#include <nlohmann/json.hpp>

#include "trip_planner/type/data_fwd.h"
#include "trip_planner/utility/formatting.h"

namespace trip_planner {

	class stop {

	public:

		/** Creates an instance of Stop. */
		stop();

		/** Creates an instance of Stop.
		*	@param code Code of the stop.
		*	@param name Name of the stop.
		*	@param zone Zone of the stop. */
		stop(scode_t code, sname_t name, zone_t zone);

		/** @return Code of the stop. */
		scode_t get_code() const;

		/** @return Name of the stop. */
		sname_t get_name() const;

		/** @return Zone of the stop. */
		zone_t get_zone() const;

		/** @return Coordinates of the stop. */
		coord_t get_coords() const;

		/** Sets the coordinates of the stop.
		*	@param coords New coordinates of the stop. */
		void set_coords(coord_t coords);

	private:

		/** Stop's code. */
		scode_t		_code;

		/** Stop's name. */
		sname_t		_name;

		/** Stop's zone. */
		zone_t		_zone;

		/** Geographical coordinates of the stop */
		coord_t		_coords;

		/** Serialize a stop to a JSON string.
		*	@param j JSON string.
		*	@param s Stop to be serialized. */
		friend void to_json(nlohmann::json &j, const stop &s);

		/** Deserialize a stop from a JSON string.
		*	@param j JSON string.
		*	@param s Stop to be deserialized. */
		friend void from_json(const nlohmann::json &j, stop &s);
	};
}

#endif
