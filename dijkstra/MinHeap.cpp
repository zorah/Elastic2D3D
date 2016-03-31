/**
		@author Zorah Lähner (laehner@in.tum.de)
    @version 03.2016
*/

#include "MinHeap.h"

// ****************************************************************************
//	Public Functions
// ****************************************************************************


// ****************************************************************************

// Constructor
MinHeap::MinHeap(long maxSize) {
	// initialise heap and map
	heap_ 	= new pair<double, long>[maxSize];
	map_ 	= new long[maxSize];
	for(long i=0; i < maxSize; i++) {
		//heap_[i] = make_pair<double, long>(-1.0, -1);
		heap_[i].first = -1.0;
		heap_[i].second = -1;
		map_[i] = -1;
	}

	maxSize_ = maxSize;
	size_ = 0;
	minEmpty_ = 0;
	maxFull_ = -1;
}


// ****************************************************************************
// Returns true if the decrease action actually changed the position in the
// Heap. False otherwise.
bool MinHeap::decrease(long key, double newValue) {
	if(key < 0 || key >= maxSize_)
		return false;

	const long index = map_[key];
	// check if bubbling is needed
	if(index != -1 && newValue < heap_[index].first) {
		heap_[index].first = newValue;
		bubbleUp(index);
		// check if position actually changed
		if(heap_[index].first != newValue)
			return true;
	}

	return false;
}


// ****************************************************************************
// Returns true if the decrease action actually changed the position in the
// Heap. False otherwise.
bool MinHeap::increase(long key, double newValue) {
	if(key < 0 || key >= maxSize_)
		return false;

	const long index = map_[key];
	// check if bubbling is needed
	if(index != -1 && newValue > heap_[index].first) {
		heap_[index].first = newValue;
		bubbleDown(index);
		// check if position actually changed
		if(heap_[index].first != newValue)
			return true;
	}

	return false;
}

// ****************************************************************************
// Returns the value of key, -1 if the key does not exist.
double MinHeap::peakKey(long key) const {
	if(key < 0 || key >= maxSize_ || map_[key] == -1)
		return -1.0;

	return heap_[map_[key]].first;
}

// ****************************************************************************
// Returns the key, value pair with smallest value in the heap and removes them
// from the heap. (-1, -1) if heap is empty.
pair<double, long> MinHeap::pop() {
	if(size_ == 0)
		return std::make_pair(-1, -1);

	const pair<double, long> minimum = heap_[0];

	// bubble down to maintain heap property
	heap_[0].first = std::numeric_limits<double>::infinity();

	switchEntries(0, maxFull_);
	// clean minimum object
	heap_[maxFull_] = std::make_pair(-1, -1);
	map_[minimum.second] = -1;

	long newIndex = bubbleDown(0);

	// update minimum/maximum empty/full index if necessary
	if(maxFull_ < minEmpty_)
		minEmpty_ = maxFull_;

	while(maxFull_ >= 0 && heap_[maxFull_].second == -1)
		maxFull_--;

	if(newIndex > maxFull_)
		maxFull_ = newIndex;

	size_--;

	return minimum;
}


// ****************************************************************************
void MinHeap::printMap() const {
	for(long i = 0; i < maxSize_; i++) {
		mexPrintf("%d: %d \n", i, map_[i]);
	}
}

// ****************************************************************************
void MinHeap::printHeap() const {
	for(long i = 0; i < maxSize_; i++) {
		mexPrintf("%d : %f: %d \n", i, heap_[i].first, heap_[i].second);
	}
}


// ****************************************************************************
// Inserts the (value, key) pair into the heap. Nothing happens if the key
// already exists in the heap or the key is larger than maxSize_.
void MinHeap::push(double value, long key) {
	if(key < 0 || key >= maxSize_ || map_[key] != -1 || size_ == maxSize_)
		return;

	heap_[minEmpty_] = std::make_pair(value, key);
	map_[key] = minEmpty_;
	bubbleUp(minEmpty_);
	size_++;

	// update minimum/maximum empty/full index
	if(minEmpty_ == maxFull_ + 1) {
		maxFull_++;
	}

	if(size_ == maxSize_) {
		minEmpty_ = size_ + 1;
		return;
	}

	while(heap_[minEmpty_].second > -1) {
		minEmpty_++;
	}
}


// ****************************************************************************
//	Private Functions
// ****************************************************************************


// ****************************************************************************
long MinHeap::bubbleDown(long i) {
	// i invalid
	if (i < 0 || i >= maxSize_)
		return -1;

	long left = i<0 ? -1 : (i * 2) + 1;
	long right = i<0 ? -1 : (i * 2) + 2;
	while ((left < maxSize_ || right < maxSize_) && (left != -1 || right != -1)) {
		// determine whether to bubble left or right
		bool goLeft = false;
		bool goRight = false;

		bool leftPossible = (left != -1 && left < maxSize_ && heap_[left].second != -1 && heap_[left].first <= heap_[i].first);
		bool rightPossible = (right != -1 && right < maxSize_ && heap_[right].second != -1 && heap_[right].first <= heap_[i].first);

		if (leftPossible && rightPossible) {
			if(heap_[right].first <= heap_[left].first)
				goRight = true;
			else
				goLeft = true;
		} else if (leftPossible) {
			goLeft = true;
		} else if (rightPossible) {
			goRight = true;
		}

		// bubble process
		if(goLeft) {
			// bubble to the left
			pair<double, long> newParent = heap_[left];
			heap_[left] = heap_[i];
			heap_[i] = newParent;

			// update map_
			map_[newParent.second] = i;
			map_[heap_[left].second] = left;

			// update indices
			i = left;
			left = i<0 ? -1 : (i * 2) + 1;
			right = i<0 ? -1 : (i * 2) + 2;
		} else if(goRight) {
			// bubble to the right
			pair<double, long> newParent = heap_[right];
			heap_[right] = heap_[i];
			heap_[i] = newParent;

			// update map_
			map_[newParent.second] = i;
			map_[heap_[right].second] = right;

			// update indices
			i = right;
			left = i<0 ? -1 : (i * 2) + 1;
			right = i<0 ? -1 : (i * 2) + 2;
		} else {
			// bubbling ends
			left = -1;
			right = -1;
		}
	}

	return i;
}


// ****************************************************************************
long MinHeap::bubbleUp(long i) {
	// i invalid
	if (i < 0 || i >= maxSize_)
		return -1;

	long parent = i<=0 ? -1 : (i - 1) / 2;
	while (parent != -1) {
		if(heap_[parent].first >= heap_[i].first) {
			// continue bubbling
			pair<double, long> newChild = heap_[parent];
			heap_[parent] = heap_[i];
			heap_[i] = newChild;

			// update map_
			map_[newChild.second] = i;
			map_[heap_[parent].second] = parent;

			// update indices
			i = parent;
			parent = i<=0 ? -1 : (i - 1) / 2;
		} else {
			// bubbling ends
			parent = -1;
		}
	}

	return i;
}

// ****************************************************************************
void MinHeap::switchEntries(long i, long j) {
	if(i == j || i < 0 || j < 0 || i >= maxSize_ || j >= maxSize_ || heap_[i].second == -1 || heap_[j].second == -1)
		return;

	// switch heap
	pair<double, long> pairi = heap_[i];
	heap_[i] = heap_[j];
	heap_[j] = pairi;

	// update map
	map_[pairi.second] = j;
	map_[heap_[i].second] = i;
}
