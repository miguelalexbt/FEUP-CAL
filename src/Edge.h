#ifndef EDGE_H
#define EDGE_H

#include <string>

template <class T> class Graph;
template <class T> class Vertex;

template <class T>
class Edge {
	std::string owner;	// owner (line)
	Vertex<T> * dest;	// destination vertex
	double weight;		// edge weight
public:
	Edge(std::string o, Vertex<T> *d, double w);
	Vertex<T> * getDest() const;

	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(std::string o, Vertex<T> *d, double w) : owner(o), dest(d), weight(w) {}

template <class T>
Vertex<T> * Edge<T>::getDest() const {
	return dest;
}

#endif