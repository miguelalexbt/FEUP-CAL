#ifndef TRIP_PLANNER_MUTABLEPRIORITYQUEUE_H
#define TRIP_PLANNER_MUTABLEPRIORITYQUEUE_H

#include <vector>

namespace trip_planner {

	template <class T>
	class mutable_priority_queue {
		std::vector<T *> _heap;

		void heapify_up(unsigned i);
		void heapify_down(unsigned i);
		inline void set(unsigned i, T * x);
	public:
		mutable_priority_queue();
		std::vector<T *> get_heap() const;
		void insert(T * x);
		T * extract_min();
		void decrease_key(T * x);
		bool empty();
	};

	template <class T>
	mutable_priority_queue<T>::mutable_priority_queue() {
		_heap.push_back(NULL);
	}

	template <class T>
	bool mutable_priority_queue<T>::empty() {
		return _heap.size() == 1;
	}

	template <class T>
	T* mutable_priority_queue<T>::extract_min() {
		auto x = _heap[1];
		x->_queue_index = 0;
		_heap[1] = _heap.back();
		_heap.pop_back();
		if (!empty())
			heapify_down(1);
		return x;
	}

	template <class T>
	void mutable_priority_queue<T>::insert(T *x) {
		_heap.push_back(x);
		heapify_up(_heap.size() - 1);
	}

	template <class T>
	void mutable_priority_queue<T>::heapify_up(unsigned i) {

		auto parent = [](unsigned i) {
			return i >> 1;
		};

		auto x = _heap[i];
		while (i > 1 && *x < *_heap[parent(i)]) {
			set(i, _heap[parent(i)]);
			i = parent(i);
		}
		set(i, x);
	}

	template <class T>
	void mutable_priority_queue<T>::decrease_key(T *x) {
		heapify_up(x->_queue_index);
	}

	template <class T>
	void mutable_priority_queue<T>::heapify_down(unsigned i) {

		auto left_child = [](unsigned i) {
			return i << 1;
		};

		auto x = _heap[i];
		while (true) {
			unsigned k = left_child(i);
			if (k >= _heap.size())
				break;
			if (k + 1 < _heap.size() && *_heap[k + 1] < *_heap[k])
				k++;
			if (!(*_heap[k] < *x))
				break;
			set(i, _heap[k]);
			i = k;
		}
		set(i, x);
	}

	template <class T>
	void mutable_priority_queue<T>::set(unsigned i, T * x) {
		_heap[i] = x;
		x->_queue_index = i;
	}

	template <class T>
	std::vector<T *> mutable_priority_queue<T>::get_heap() const {
		return _heap;
	}
}
#endif