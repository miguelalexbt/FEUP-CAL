	#ifndef TRIP_PLANNER_DATABASE_H
#define TRIP_PLANNER_DATABASE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <chrono>

#include "trip_planner/core/loader.h"
#include "trip_planner/core/builder.h"
#include "trip_planner/core/viewer/viewer.h"
#include "trip_planner/core/menu/menu.h"
#include "trip_planner/core/menu/menu_behaviour.h"
#include "trip_planner/data/line.h"
#include "trip_planner/data/stop.h"
#include "trip_planner/graph/graph.h"
#include "trip_planner/algorithm/algorithm.h"

namespace trip_planner {

	class database {

	public:

		/** Database's instance. */
		static database &instance();

		/** Loader's instance. */
		std::vector<scode_t> query_stop(std::string stop_name);

		/** Loader's instance. */
		std::vector<scode_t> query_stop(std::string stop_name, lcode_t line_code);

		/** Loader's instance. */
		lcode_t query_line(std::string line_name);

	private:

		/** Loaded lines. */
		line_data		_lines;

		/** Loaded stops. */
		stop_data		_stops;

		/** Created Graph. */
		graph<stop*>	_graph;

		/** Creates an instance of Database. */
		database();

		/** Runs the loader. */
		void load_data();

		/** Runs the builder. */
		void build_graph(criteria_t option);

		/** Runs the algorithm of choice.
		*	@param start_stop Initial stop.
		*	@param end_stop Final stop.
		*	@param algorithm Algorithm to run.	*/
		void run_algorithm(scode_t start_stop, scode_t end_stop, algorithm_t algorithm);

		/** Shows on screen that path calculated by the algorithm.
		*	@param start_stop Initial stop.
		*	@param end_stop Final stop.	*/
		void parse_results(scode_t start_stop, scode_t end_stop);

		/** Shows the path on the GraphViewer.
		*	@param start_stop Initial stop.
		*	@param end_stop Final stop.	*/
		void view_graph(scode_t start_stop, scode_t end_stop);

		friend class loader;
		friend class builder;
		friend class viewer;
		
		friend class menu;
		friend class default_behaviour;
		friend class criteria_behaviour;
		friend class search_behaviour;
		friend class selection_behaviour;
		friend class algorithm_behaviour;
		friend class graph_behaviour;
	};
}

#endif
