#ifndef TRIP_PLANNER_BUILDER_H
#define TRIP_PLANNER_BUILDER_H

#include <iostream>
#include <chrono>

#include "trip_planner/core/database.h"
#include "trip_planner/type/data_fwd.h"
#include "trip_planner/utility/geographic.h"

namespace trip_planner {

	class builder {

	public:

		/** Builder's instance. */
		static builder &instance();

		/** Builds the graph according to the criteria.
		*	@param option Criteria to build the graph on. */
		void build(criteria_t option);

	private:

		/** Creates an instance of Builder. */
		builder();

		/** Adds vertices to the graph. */
		void add_vertices();

		/** Adds edges to the graph, following the criteria.
		*	@param option Criteria chosen by the user. */
		void add_edges(criteria_t option);

		/** Clears the existing Graph. */
		void clean();

		/** Applies modifiers to the initial weight, depending on the criteria.
		*	@param option Criteria chosen by the user.
		*	@param start Starting stop.
		*	@param end Destination stop.
		*	@param weight Initial weight.
		*	@param mode Traveling mode.
		*	@return Returns the modified weight. */
		double apply_modifiers(criteria_t option, scode_t start, scode_t end, double weight, mode_t mode);

		/** Connects two stops, adding an one-way edge between them.
		*	@param stop_src Starting stop.
		*	@param stop_dst	Destination stop.
		*	@param owner Line that connects the two stops.
		*	@param weight Weight of the edge. */
		void connect(scode_t stop_src, scode_t stop_dst, pubcode_t owner, double weight);

		/** Connects two stops, adding an bidirectional edge between them.
		*	@param stop_src Starting stop.
		*	@param stop_dst	Destination stop.
		*	@param weight Weight of the edge. */
		void connect2way(scode_t stop_src, scode_t stop_dst, double weight);
	};
}

#endif