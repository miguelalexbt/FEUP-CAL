#ifndef VERTEX_H
#define VERTEX_H

#include <vector>
#include <string>

template <class T> class Edge;
template <class T> class Graph;
template <class T> class MutablePriorityQueue;

template <class T>
class Vertex {
	T info;						// contents
	std::vector<Edge<T>> adj;  // outgoing edges
	bool visited;				// auxiliary field
	double dist = 0;
	Vertex<T> *path = NULL;
	double fScore;
	std::string edge_info = "";
	int queueIndex = 0; 		// required by MutablePriorityQueue

	void addEdge(Vertex<T> *dest, std::string o, double w);

public:
	Vertex(T in);
	bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
	std::vector<Edge<T>> getAdj() const;

	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};

template <class T>
Vertex<T>::Vertex(T in) : info(in) {}

/*
* Auxiliary function to add an outgoing edge to a vertex (this),
* with a given destination vertex (d) and edge weight (w).
*/
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, std::string o, double w) {
	adj.push_back(Edge<T>(o, d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
	return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
	return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
	return this->path;
}

template <class T>
std::vector<Edge<T>> Vertex<T>::getAdj() const {
	return adj;
}

#endif