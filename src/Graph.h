/* Graph.h */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <set>

#include "Utils.h"
#include "Vertex.h"
#include "Edge.h"
#include "MutablePriorityQueue.h"

#undef max
#define INF std::numeric_limits<double>::max()

template <class T>
class Graph {
	std::vector<Vertex<T> *> vertexSet;    // vertex set

public:
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, std::string o, double w);
	int getNumVertex() const;
	std::vector<Vertex<T> *> getVertexSet() const;

	Vertex<T> * initSingleSource(const T &origin);
	bool relax(Vertex<T> *v, Vertex<T> *w, Edge<T> e);

	void dijkstra(const T &s);
	void bellman_ford(const T &s);
	void johnson(const T &s);
	void a_star(const T &origin, const T &destination);

	std::pair<std::vector<T>, std::vector<std::string>> getPath(const T &origin, const T &dest) const;
};

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

/*
* Auxiliary function to find a vertex with a given content.
*/
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/*
*  Adds a vertex with a given content or info (in) to a graph (this).
*  Returns true if successful, and false if a vertex with that content already exists.
*/
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if (findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in));
	return true;
}

/*
* Adds an edge to a graph (this), given the contents of the source and
* destination vertices and the edge weight (w).
* Returns true if successful, and false if the source or destination vertex does not exist.
*/
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, std::string o, double w) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2, o, w);
	return true;
}

/**************** Single Source Shortest Path Algorithms ************/

template<class T>
Vertex<T> * Graph<T>::initSingleSource(const T &origin) {

	for (auto v : vertexSet) {
		v->dist = INF;
		v->path = nullptr;
	}

	auto s = findVertex(origin);
	s->dist = 0;

	return s;
}

template<class T>
bool Graph<T>::relax(Vertex<T> *v, Vertex<T> *w, Edge<T> e) {

	if (v->dist + e.weight < w->dist) {
		w->dist = v->dist + e.weight;
		w->path = v;
		w->edge_info = e.owner;
		return true;
	}

	return false;
}

template<class T>
void Graph<T>::dijkstra(const T &origin) {

	auto *src = initSingleSource(origin);

	MutablePriorityQueue<Vertex<T>> queue;

	queue.insert(src);

	while (!queue.empty()) {

		auto v = queue.extractMin();

		for (Edge<T> e : v->adj) {

			auto oldDist = e.dest->dist;

			if (relax(v, e.dest, e)) {

				if (oldDist == INF)
					queue.insert(e.dest);
				else
					queue.decreaseKey(e.dest);
			}
		}
	}
}

template<class T>
void Graph<T>::bellman_ford(const T &origin) {

	initSingleSource(origin);

	for (size_t i = 0; i < vertexSet.size() - 1; i++) {

		for (size_t j = 0; j < vertexSet.size(); j++) {

			for (Edge<T> e : vertexSet[j]->adj)
				relax(vertexSet[j], e.dest, e);
		}
	}

	for (size_t i = 0; i < vertexSet.size(); i++) {

		for (Edge<T> e : vertexSet[i]->adj) {

			if (relax(vertexSet[i], e.dest, e))
				throw runtime_error("Negative weights.");
		}
	}
}

template<class T>
void Graph<T>::johnson(const T &s) {

	T obj = T();
	addVertex(obj);

	for (size_t i = 0; i < vertexSet.size(); i++)
		addEdge(obj, vertexSet[i]->info, "", 0);

	bellman_ford(obj);

	vertexSet.pop_back();

	for (size_t i = 0; i < vertexSet.size(); i++)
		for (Edge<T> e : vertexSet[i]->adj)
			e.weight += vertexSet[i]->dist - e.dest->dist;

	dijkstra(s);
}

template<class T>
void Graph<T>::a_star(const T &origin, const T &destination) {

	auto heuristic_cost_estimate = [](T start, T end) {
		return utils::haversineDistance(start->getCoords(), end->getCoords());
	};

	set<Vertex<T>*> closedSet;

	MutablePriorityQueue<Vertex<T>> openSet;

	for (auto v : vertexSet) {
		v->dist = INF;
		v->path = nullptr;
		v->fScore = INF;
		v->edge_info = "";
	}

	auto src = findVertex(origin);
	auto dst = findVertex(destination);

	src->dist = 0;
	src->fScore = heuristic_cost_estimate(src->getInfo(), dst->getInfo());

	openSet.insert(src);

	while (!openSet.empty()) {

		Vertex<T>* current = openSet.extractMin();

		if (current == dst)
			return;

		closedSet.insert(current);

		for (Edge<T> e : current->adj) {

			if (closedSet.find(e.dest) != closedSet.end())
				continue;

			auto heap = openSet.getHeap();
			if (find(heap.begin(), heap.end(), e.dest) == heap.end())
				openSet.insert(e.dest);

			double tentative_gScore = current->dist + e.weight;
			if (tentative_gScore >= e.dest->dist)
				continue;

			e.dest->path = current;
			e.dest->dist = tentative_gScore;
			e.dest->fScore = tentative_gScore + heuristic_cost_estimate(e.dest->getInfo(), dst->getInfo());
			e.dest->edge_info = e.owner;
		}
	}
}

template<class T>
std::pair<std::vector<T>, std::vector<std::string>> Graph<T>::getPath(const T &origin, const T &dest) const {

	std::pair<std::vector<T>, std::vector<std::string>> res;
	std::vector<T> nodes;
	std::vector<std::string> edges;

	auto v = findVertex(dest);

	if (v == nullptr || v->dist == INF)
		return res;

	for (; v != nullptr; v = v->path) {
		nodes.push_back(v->info);
		edges.push_back(v->edge_info);
	}

	std::reverse(nodes.begin(), nodes.end());
	std::reverse(edges.begin(), edges.end());

	res.first = nodes;
	res.second = edges;

	return res;
}

#endif /* GRAPH_H_ */