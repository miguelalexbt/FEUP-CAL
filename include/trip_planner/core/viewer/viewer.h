	#ifndef TRIP_PLANNER_VIEWER_H
#define TRIP_PLANNER_VIEWER_H

#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

#include "trip_planner/graph/vertex.h"
#include "trip_planner/graph/edge.h"
#include "trip_planner/graph/graph.h"
#include "trip_planner/data/stop.h"
#include "trip_planner/core/viewer/graph_viewer.h"

#undef max

namespace trip_planner {

	class viewer {

	public:

		/** Creates the viewer
		*	@param g Graph to generate GraphViewer from. */
		viewer(graph<stop*>* g);

		/** Shows the created Graph. */
		void view();

		/** Shows the path determined by the used algorithm of choice. */
		void show_path(vector<stop*> path);

		/** Calculates the extremes latitude and longitude of all stops of the Graph.
		*	@param latMin Minimum latitude variable to be stored in.
		*	@param latMax Maximum latitude variable to be stored in.
		*	@param lonMax Maximum longitude variable to be stored in.
		*	@param lonMin Minimum longitude variable to be stored in. */
		void limit_coords(double& latMin, double& latMax, double& lonMax, double& lonMin);


	private:

		/** Graph to generate GraphViewer from. */
		graph<stop*>* _g;

		/** GraphViewer. */
		GraphViewer * _gv;

		/** Map with the relation between a stop and a node index for the GraphViewer. */
		std::map<std::string, int> stops_index;

		/** Map with the relation between a line and a edge index for the GraphViewer. */
		std::map<pair<int, int>, int> lines_index;
	};
}

#endif