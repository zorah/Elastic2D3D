/**
		@author Zorah Lähner (laehner@in.tum.de)
    @version 03.2016
*/

#include <vector>
#include <list>

#include "MinHeap.h"

void manifoldDijkstra(  const double* all_energies,
            const   vector<list<long>* >&   mesh,
            const   double*                 vertices,
            const long                      nVertices,
            const int                       dimVertices,
            const   double*       triangles,
            const long            nTriangles,
            const   double*       contour,
            const long            nContour,
            const int             dimContour,
            double*               result,
            long*                 predecessors,
            const   int*          cut,
            const   int           cutID // dijkstra will start only on vertices where cut[i]==cutID
        ) {

  // initialise and find upper bound

  MinHeap** heapStruct = new MinHeap*[nContour];

  heapStruct[0] = new MinHeap(nVertices);

  for(long i=0; i < nVertices; i++)
    if(cut[i] == cutID)
       heapStruct[0]->push(0, i);

  double* currentEnergy = new double[nVertices];

  for(long i=0; i < nContour; i++)
  {
    if (i>0)
      heapStruct[i] = new MinHeap(nVertices);

    for(long j=0; j < nVertices; j++)
    {
       heapStruct[i]->push(std::numeric_limits<double>::infinity(), j);

      result[i*nVertices+j] = -1;
      predecessors[i*nVertices+j] = -1;

      if (i==0)
        currentEnergy[j] = 0.;
      else
        currentEnergy[j] += energy(all_energies, vertices, nVertices, dimVertices, triangles, nTriangles, contour, nContour, dimContour, predecessors, i, i - 1, j, j);
    }

  }

  double upperbound = std::numeric_limits<double>::infinity();

  for (long i=0; i<nVertices; ++i)
    if (currentEnergy[i] < upperbound)
      upperbound = currentEnergy[i];

  delete[] currentEnergy;

  // run dijkstra
  long index;
  int level = 0;
  MinHeap* heap = heapStruct[level];

  while(!heap->isEmpty()) {
    pair<double, long> current = heap->pop();
    result[getIndex(current.second, level, nVertices)] = current.first;

    if(current.first <= upperbound) {

      // update neighbors
      double oldValue, newValue;

      int kbound = 0;
      // move to next layer on same vertex
      if(level < nContour - 1) {
        index = getIndex(current.second, level + 1, nVertices);

        oldValue = heapStruct[level + 1]->peakKey(current.second);
        newValue = current.first + energy(all_energies, vertices, nVertices, dimVertices, triangles, nTriangles, contour, nContour, dimContour, predecessors, level, level + 1, current.second, current.second);

        if(result[index] < 0 && newValue < oldValue) {
          // new way is faster
          heapStruct[level + 1]->decrease(current.second, newValue);
          predecessors[index] = getIndex(current.second, level, nVertices);
        }
        kbound = 1;
      }

      // move on mesh, this or next layer
      for(int k = 0; k <= kbound; k++)
      {
        for(auto it = mesh.at(current.second)->begin(); it != mesh.at(current.second)->end(); it++)
        {
          index = getIndex(*it, level + k, nVertices);
          if(result[index] < 0)
          {
            // index is not minimal yet
            oldValue = heapStruct[level + k]->peakKey(*it);
            newValue = current.first + energy(all_energies, vertices, nVertices, dimVertices, triangles, nTriangles, contour, nContour, dimContour, predecessors, level, level + k, current.second, *it);

            if (newValue <= upperbound && newValue < oldValue)
            {
              // new way is faster
              heapStruct[level + k]->decrease(*it, newValue);
              predecessors[index] = getIndex(current.second, level, nVertices);
            }
          }
        }
      }

    }

    if(heap->isEmpty() && level < nContour - 1) {
      level++;
      heap = heapStruct[level];
    }

  }

  for(int i = 0; i < nContour; i++)
    delete heapStruct[i];

  delete[] heapStruct;
}
