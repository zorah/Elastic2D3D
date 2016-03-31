/**
		@author Zorah Lähner (laehner@in.tum.de)
    @version 03.2016
*/

#ifndef __MinHeap__
#define __MinHeap__

#include "mex.h"
#include <math.h>
#include <utility>
#include <vector>
#include <limits>

using std::pair;

class MinHeap {
public:

	explicit MinHeap(long nVertices);
	~MinHeap() { delete[] heap_; delete[] map_; };

	bool				decrease(long key, double newValue); // O(log n)
	bool 				increase(long key, double newValue); // O(log n)

	inline bool isEmpty() const { // O(1)
		return (size_ == 0);
	}

	// Returns the smallest value in the heap. Infinity if the heap is empty.
	inline double peak() const { // O(1)
		return size_ == 0 ? std::numeric_limits<double>::infinity() : heap_[0].first;
	}

	double 				peakKey(long key) const; // O(1)
	pair<double, long> 	pop(); // O(log n)
	void 				push(double newValue, long key); // worst: O(n), avg: O(log n)

	// debug, delete late
	void 				printMap() const;
	void 				printHeap() const;

private:

	MinHeap(const MinHeap&); // not copyable

	long 	bubbleDown(long);
	long 	bubbleUp(long);
	inline void		switchEntries(long, long);

	// heap structure, double: current energy, long: index
	pair<double, long>*		heap_;
	// maps each index to the current index in the heap_
	long*					map_;
	// current size of the heap
	long 					size_;
	// maximum size of the heap
	long 					maxSize_;
	// minimum/maximum empty/full index in the heap_
	long 					minEmpty_;
	long 					maxFull_;
};

#endif
