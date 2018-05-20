#ifndef TRIP_PLANNER_GRAPH_H
#define TRIP_PLANNER_GRAPH_H

#include <vector>
#include <set>
#include <algorithm>

#include "trip_planner/graph/vertex.h"
#include "trip_planner/graph/edge.h"
#include "trip_planner/graph/mutable_priority_queue.h"
#include "trip_planner/utility/geographic.h"
#include "trip_planner/type/graph_fwd.h"

#undef max

namespace trip_planner {

	namespace {
		const double INF = std::numeric_limits<double>::max();
	}

	template <class T>
	class graph {

	public:

		graph();

		vertex<T> *find_vertex(const T &in) const;

		bool add_vertex(const T &in);

		bool add_edge(const T &sourc, const T &dest, std::string owner, double weight);

		int get_size() const;

		std::vector<vertex<T> *> get_vertex_set() const;

		vertex<T> *init_single_source(const T &origin);

		bool relax(vertex<T> *v, vertex<T> *w, edge<T> e);

		path_t<T> get_path(const T &origin, const T &dest) const;

		void dijkstra(const T &origin);

		void bellman_ford(const T &origin);

		void johnson(const T &origin);

		void a_star(const T &origin, const T &destination);

	private:

		std::vector<vertex<T> *> _vertex_set;
	};


	/**
	* Default constructor
	*/
	template <class T>
	graph<T>::graph() {}

	/**
	* Auxiliary function to find a vertex with a given content.
	*/
	template <class T>
	vertex<T> *graph<T>::find_vertex(const T &in) const {
		for (auto v : _vertex_set) {
			if (v->_info == in)
				return v;
		}

		return nullptr;
	}

	/**
	*  Adds a vertex with a given content or info (in) to a graph (this).
	*  Returns true if successful, and false if a vertex with that content already exists.
	*/
	template <class T>
	bool graph<T>::add_vertex(const T &in) {
		if (find_vertex(in) != nullptr)
			return false;

		_vertex_set.push_back(new vertex<T>(in));
		return true;
	}

	/**
	* Adds an edge to a graph (this), given the contents of the source and
	* destination vertices and the edge weight (w).
	* Returns true if successful, and false if the source or destination vertex does not exist.
	*/
	template <class T>
	bool graph<T>::add_edge(const T &sourc, const T &dest, std::string owner, double weight) {
		auto src = find_vertex(sourc);
		auto dst = find_vertex(dest);

		if (src == nullptr || dst == nullptr)
			return false;

		src->add_edge(dst, owner, weight);
		return true;
	}

	/**
	* Returns the number of existing vertexes
	*/
	template <class T>
	int graph<T>::get_size() const {
		return _vertex_set.size();
	}

	/**
	* Returns the existing vertexes
	*/
	template <class T>
	std::vector<vertex<T> *> graph<T>::get_vertex_set() const {
		return _vertex_set;
	}

	template <class T>
	vertex<T> * graph<T>::init_single_source(const T &origin) {
		for (auto v : _vertex_set) {
			v->_dist = INF;
			v->_path = nullptr;
		}

		auto s = find_vertex(origin);
		s->_dist = 0;

		return s;
	}

	template <class T>
	bool graph<T>::relax(vertex<T> *v, vertex<T> *w, edge<T> e) {

		if (v->_dist + e._weight < w->_dist) {
			w->_dist = v->_dist + e._weight;
			w->_path = v;
			w->_path_info = e._owner;
			
			return true;
		}

		return false;
	}

	template <class T>
	path_t<T> graph<T>::get_path(const T &origin, const T &dest) const {

		std::vector<T> nodes;
		std::vector<lcode_t> edges;

		auto dst = find_vertex(dest);

		if (dst == nullptr || dst->_dist == INF)
			return { nodes, edges };

		while (dst != nullptr) {
			nodes.push_back(dst->_info);
			edges.push_back(dst->_path_info);
			dst = dst->_path;
		}

		std::reverse(nodes.begin(), nodes.end());
		std::reverse(edges.begin(), edges.end());

		return { nodes, edges };
	}

	template <class T>
	void graph<T>::dijkstra(const T &origin) {

		auto *src = init_single_source(origin);

		mutable_priority_queue<vertex<T>> queue;

		queue.insert(src);

		while (!queue.empty()) {

			auto v = queue.extract_min();

			for (auto e : v->_adj) {

				auto old_dist = e._dest->_dist;

				if (relax(v, e._dest, e)) {
					old_dist == INF ? queue.insert(e._dest) : queue.decrease_key(e._dest);
				}
			}
		}
	}

	template <class T>
	void graph<T>::bellman_ford(const T &origin) {

		init_single_source(origin);

		for (size_t i = 0; i < _vertex_set.size() - 1; ++i)
			for (auto v : _vertex_set)
				for (auto e : v->_adj)
					relax(v, e._dest, e);

		for (auto v : _vertex_set)
			for (auto e : v->_adj)
				if (relax(v, e._dest, e))
					throw std::runtime_error("negative weights");
	}

	template <class T>
	void graph<T>::johnson(const T &origin) {

		T obj = T();
		add_vertex(obj);

		for (size_t i = 0; i < _vertex_set.size(); ++i)
			add_edge(obj, _vertex_set[i]->_info, "", 0);

		bellman_ford(obj);

		_vertex_set.pop_back();

		for (size_t i = 0; i < _vertex_set.size(); ++i)
			for (auto e : _vertex_set[i]->_adj)
				e._weight += _vertex_set[i]->_dist - e._dest->_dist;

		dijkstra(origin);
	}

	template <class T>
	void graph<T>::a_star(const T &origin, const T &destination) {

		auto heuristic_cost_estimate = [](T start, T end) {
			return utility::haversine_distance(start->get_coords(), end->get_coords());
		};

		std::set<vertex<T> *> closed_set;
		mutable_priority_queue<vertex<T>> open_set;

		for (auto v : _vertex_set) {
			v->_dist = INF;
			v->_path = nullptr;
			v->_path_info = "";
			v->_fscore = INF;
		}

		auto src = find_vertex(origin);
		auto dst = find_vertex(destination);

		src->_dist = 0;
		src->_fscore = heuristic_cost_estimate(src->get_info(), dst->get_info());

		open_set.insert(src);

		while (!open_set.empty()) {

			auto current = open_set.extract_min();

			if (current == dst)
				return;

			closed_set.insert(current);

			for (auto e : current->_adj) {

				if (closed_set.find(e._dest) != closed_set.end())
					continue;

				auto heap = open_set.get_heap();

				if (find(heap.begin(), heap.end(), e._dest) == heap.end())
					open_set.insert(e._dest);

				double tentative_gscore = current->_dist + e._weight;
				if (tentative_gscore >= e._dest->_dist)
					continue;

				e._dest->_dist = tentative_gscore;
				e._dest->_path = current;
				e._dest->_path_info = e._owner;
				e._dest->_fscore = tentative_gscore + heuristic_cost_estimate(e._dest->get_info(), dst->get_info());
			}
		}
	}
}

#endif