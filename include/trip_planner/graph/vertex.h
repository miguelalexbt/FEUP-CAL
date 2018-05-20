#ifndef TRIP_PLANNER_VERTEX_H
#define TRIP_PLANNER_VERTEX_H

#include <vector>
#include <string>

#include "trip_planner/type/graph_fwd.h"

namespace trip_planner {

	template <class T>
	class vertex {

	public:

		vertex(T in);

		T get_info() const;

		double get_dist() const;

		vertex<T> *get_path() const;

		std::vector<edge<T>> get_adj() const;

		bool operator<(vertex<T> &v) const;

	private:

		T						_info;

		double					_dist;

		vertex<T>				*_path;

		std::string				_path_info;

		std::vector<edge<T>>	_adj;

		bool					_visited;

		double					_fscore;

		int						_queue_index;

		/** Auxiliary function to add an outgoing edge to a vertex (this),
		*	with a given destination vertex, and edge weight.
		*	@param dest Destination vertex.
		*	@param owner Line that the edge represents.
		*	@param weight Weight of the edge. */
		void add_edge(vertex<T> *dest, std::string owner, double weight);

		friend class graph<T>;
		friend class mutable_priority_queue<vertex<T>>;
	};

	template <class T>
	vertex<T>::vertex(T in) : _info(in) {}

	template <class T>
	T vertex<T>::get_info() const {
		return _info;
	}

	template <class T>
	double vertex<T>::get_dist() const {
		return _dist;
	}

	template <class T>
	vertex<T> *vertex<T>::get_path() const {
		return _path;
	}

	template <class T>
	std::vector<edge<T>> vertex<T>::get_adj() const {
		return _adj;
	}

	template <class T>
	bool vertex<T>::operator<(vertex<T> & v) const {
		return _dist < v._dist;
	}

	template <class T>
	void vertex<T>::add_edge(vertex<T> *dest, std::string owner, double weight) {
		_adj.push_back(edge<T>(dest, owner, weight));
	}
}

#endif