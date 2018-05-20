
#include "trip_planner/core/viewer/viewer.h"

using namespace std;

namespace trip_planner {

	viewer::viewer(graph<stop*>* g) {
		_g = g;
	}

	void viewer::view() {

		int hor_res = 800;
		int vert_res = 800;

		_gv = new GraphViewer(hor_res, vert_res, false);
		_gv->createWindow(hor_res, vert_res);

		_gv->defineVertexColor("blue");
		_gv->defineEdgeColor("black");
		_gv->defineEdgeCurved(false);

		double latMin, latMax, lonMin, lonMax;
		limit_coords(latMin, latMax, lonMin, lonMax);

		// adicionar bordas de 2%
		latMin -= (latMax - latMin) * 0.02;
		latMax += (latMax - latMin) * 0.02;
		lonMin -= (lonMax - lonMin) * 0.02;
		lonMax += (lonMax - lonMin) * 0.02;

		for (size_t i = 0; i < _g->get_vertex_set().size(); i++) {

			auto vertice = _g->get_vertex_set()[i];

			double x = hor_res - (vertice->get_info()->get_coords().first - lonMin) * (hor_res / (lonMax - lonMin));
			double y = vert_res - (vertice->get_info()->get_coords().second - latMin) * (vert_res / (latMax - latMin));

			_gv->addNode(i, (int)x, (int)y);
			_gv->setVertexLabel(i, vertice->get_info()->get_code());

			if (_g->get_vertex_set()[i]->get_info()->get_code().rfind("M_", 0) == 0)
				_gv->setVertexColor(i, "yellow");

			stops_index[vertice->get_info()->get_code()] = i;
		}

		int edge_index = 0;
		for (size_t i = 0; i < _g->get_vertex_set().size(); i++) {
			auto vertice = _g->get_vertex_set()[i];

			auto it = stops_index.find(vertice->get_info()->get_code());
			if (it == stops_index.end())
				continue;

			for (size_t j = 0; j < vertice->get_adj().size(); j++) {
				edge<stop*> edge = vertice->get_adj()[j];

				auto it2 = stops_index.find(edge.get_dest()->get_info()->get_code());
				if (it2 == stops_index.end())
					continue;

				_gv->addEdge(edge_index++, it->second, it2->second, EdgeType::DIRECTED);

				lines_index[make_pair(it->second, it2->second)] = edge_index;
			}
		}
	}

	void viewer::show_path(vector<stop*> path) {

		auto nodes = path;

		for (size_t i = 0; i < nodes.size() - 1; i++) {

			auto it = stops_index.find(nodes[i]->get_code());
			if (it == stops_index.end())
				continue;

			_gv->setVertexColor(it->second, "red");

			auto it3 = stops_index.find(nodes[i]->get_code());
			auto it4 = stops_index.find(nodes[i + 1]->get_code());

			auto it2 = lines_index.find(make_pair(it3->second, it4->second));

			_gv->setEdgeColor(it2->second - 1, "red");

		}

		auto it = stops_index.find(nodes[nodes.size() - 1]->get_code());
		_gv->setVertexColor(it->second, "red");

		_gv->rearrange();

	}

	void viewer::limit_coords(double& latMin, double& latMax, double& lonMax, double& lonMin) {

		latMin = numeric_limits<double>::max();
		latMax = numeric_limits<double>::lowest();
		lonMin = numeric_limits<double>::max();
		lonMax = numeric_limits<double>::lowest();

		for (size_t i = 0; i < _g->get_vertex_set().size(); i++) {

			if (_g->get_vertex_set()[i]->get_info()->get_coords().second < latMin) latMin = _g->get_vertex_set()[i]->get_info()->get_coords().second;
			if (_g->get_vertex_set()[i]->get_info()->get_coords().second > latMax) latMax = _g->get_vertex_set()[i]->get_info()->get_coords().second;

			if (_g->get_vertex_set()[i]->get_info()->get_coords().first < lonMin) lonMin = _g->get_vertex_set()[i]->get_info()->get_coords().first;
			if (_g->get_vertex_set()[i]->get_info()->get_coords().first > lonMax) lonMax = _g->get_vertex_set()[i]->get_info()->get_coords().first;
		}
	}


}