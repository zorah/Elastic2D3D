/**
		@author Zorah Lähner (laehner@in.tum.de)
    @version 03.2016
*/

#include "MinHeap.h"

using std::vector;
using std::list;

// dijkstra from two points cutting the shape in half
// more or less the voronoi cells around the points
void shapeCut(	const 	vector<list<long>* >& 	mesh,
				 	long 					nVertices,
					double* 			result,
					int* 					cut,
				 	int 					newId,
				 	long 					vertex0,
				 	long 					vertex1
			 ) {

	const int oldId = cut[vertex0];
	if(oldId != cut[vertex1])
		return;

	MinHeap heap(nVertices);
	for(long i=0; i < nVertices; i++) {
		if(cut[i] != oldId) {
			result[i] = 0;
		} else {
			result[i] = -1;
		}
	}

	cut[vertex1] = newId;

	// initialise heap
	heap.push(0, vertex1);
	heap.push(0, vertex0);

	for(long i=0; i < nVertices; i++) {
		if(result[i] < 0) {
	 		heap.push(std::numeric_limits<double>::infinity(), i);
	 	}
	}

	while(!heap.isEmpty()) {
		pair<double, long> current = heap.pop();
		result[current.second] = current.first;

		double oldValue, newValue;

		for(auto it = mesh.at(current.second)->begin(); it != mesh.at(current.second)->end(); it++) {
			if(result[*it] < 0) {
				// index is not minimal yet
				oldValue = heap.peakKey(*it);
				newValue = current.first + 1;
				if (newValue < oldValue) {
					// new way is faster
					heap.decrease(*it, newValue);
					cut[*it] = cut[current.second];
				}
			}
		}
	}

}
