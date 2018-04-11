
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>

#include "Vertex.h"
#include "Edge.h"
#include "Stop.h"
#include "Graph.h"
#include "graphviewer.h"

#undef max

using namespace std;

class Viewer {

private:
	Graph<Stop*>* _g;
	GraphViewer * _gv;
	std::map<std::string, int> stops_index;
	std::map<pair<int, int>, int> lines_index;

public:

	Viewer(Graph<Stop*>* g) {
		_g = g;
	}

	void showGraph() {

		int hor_res = 800;
		int vert_res = 800;

		_gv = new GraphViewer(hor_res, vert_res, false);
		_gv->createWindow(hor_res, vert_res);

		_gv->defineVertexColor("blue");
		_gv->defineEdgeColor("black");
		_gv->defineEdgeCurved(false);

		double latMin, latMax, lonMin, lonMax;
		limitCoords(latMin, latMax, lonMin, lonMax);

		// adicionar bordas de 2%
		latMin -= (latMax - latMin) * 0.02;
		latMax += (latMax - latMin) * 0.02;
		lonMin -= (lonMax - lonMin) * 0.02;
		lonMax += (lonMax - lonMin) * 0.02;

		for (size_t i = 0; i < _g->getVertexSet().size(); i++) {

			auto vertice = _g->getVertexSet()[i];

			double x = (vertice->getInfo()->getCoords().first - lonMin) * (hor_res / (lonMax - lonMin));
			double y = vert_res - (vertice->getInfo()->getCoords().second - latMin) * (vert_res / (latMax - latMin));

			_gv->addNode(i, (int)x, (int)y);
			_gv->setVertexLabel(i, vertice->getInfo()->getCode());

			if (_g->getVertexSet()[i]->getInfo()->getCode().rfind("M_", 0) == 0)
				_gv->setVertexColor(i, "yellow");

			stops_index[vertice->getInfo()->getCode()] = i;
		}

		int edge_index = 0;
		for (size_t i = 0; i < _g->getVertexSet().size(); i++) {
			auto vertice = _g->getVertexSet()[i];

			auto it = stops_index.find(vertice->getInfo()->getCode());
			if (it == stops_index.end())
				continue;

			for (size_t j = 0; j < vertice->getAdj().size(); j++) {
				Edge<Stop*> edge = vertice->getAdj()[j];

				auto it2 = stops_index.find(edge.getDest()->getInfo()->getCode());
				if (it2 == stops_index.end())
					continue;

				_gv->addEdge(edge_index++, it->second, it2->second, EdgeType::DIRECTED);

				lines_index[make_pair(it->second, it2->second)] = edge_index;
			}
		}
	}

	void showPath(vector<Stop*> path) {

		auto nodes = path;

		for (size_t i = 0; i < nodes.size() - 1; i++) {

			auto it = stops_index.find(nodes[i]->getCode());
			if (it == stops_index.end())
				continue;

			_gv->setVertexColor(it->second, "red");

			auto it3 = stops_index.find(nodes[i]->getCode());
			auto it4 = stops_index.find(nodes[i + 1]->getCode());

			auto it2 = lines_index.find(make_pair(it3->second, it4->second));

			_gv->setEdgeColor(it2->second - 1, "red");

		}

		auto it = stops_index.find(nodes[nodes.size() - 1]->getCode());
		_gv->setVertexColor(it->second, "red");

		_gv->rearrange();

	}

	void limitCoords(double& latMin, double& latMax, double& lonMax, double& lonMin) {

		latMin = numeric_limits<double>::max();
		latMax = numeric_limits<double>::lowest();
		lonMin = numeric_limits<double>::max();
		lonMax = numeric_limits<double>::lowest();

		for (size_t i = 0; i < _g->getVertexSet().size(); i++) {

			if (_g->getVertexSet()[i]->getInfo()->getCoords().second < latMin) latMin = _g->getVertexSet()[i]->getInfo()->getCoords().second;
			if (_g->getVertexSet()[i]->getInfo()->getCoords().second > latMax) latMax = _g->getVertexSet()[i]->getInfo()->getCoords().second;

			if (_g->getVertexSet()[i]->getInfo()->getCoords().first < lonMin) lonMin = _g->getVertexSet()[i]->getInfo()->getCoords().first;
			if (_g->getVertexSet()[i]->getInfo()->getCoords().first > lonMax) lonMax = _g->getVertexSet()[i]->getInfo()->getCoords().first;
		}
	}


};