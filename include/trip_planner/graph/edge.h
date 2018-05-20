#ifndef TRIP_PLANNER_EDGE_H
#define TRIP_PLANNER_EDGE_H

#include <string>

#include "trip_planner/type/graph_fwd.h"

namespace trip_planner {


	template <class T>
	class edge {

	public:

		edge(vertex<T> *dest, std::string owner, double weight);

		vertex<T> *get_dest() const;

	private:

		vertex<T>	*_dest;
		
		std::string	_owner;
		
		double		_weight;

		friend class vertex<T>;
		friend class graph<T>;
	};

	template <class T>
	edge<T>::edge(vertex<T> *dest, std::string owner, double weight) : _dest(dest), _owner(owner), _weight(weight) {}

	template <class T>
	vertex<T> * edge<T>::get_dest() const {
		return _dest;
	}
}

#endif