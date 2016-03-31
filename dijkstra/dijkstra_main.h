/**
		@author Zorah Lähner (laehner@in.tum.de)
    @version 03.2016
*/
double dijkstra_main( 	const double* 	vertices,
												const long 			nVertices,
												const int 			dimVertices,
												const double* 	triangles,
												const long 			nTriangles,
												const double* 	contour,
												const long 			nContour,
												const int 			dimContour,
												list<long>& 		vPath
					) {
// build mesh data structure
	vector<list<long>* > mesh(nVertices);
	for (long i=0; i<nVertices; i++) {
		mesh[i] = new list<long>();
	}
	buildMesh(triangles, nTriangles, mesh);

	double* const result = new double[nVertices * nContour];
	long* const predecessors = new long[nVertices * nContour];

	std::cout << "Precalculating energies... " << std::flush;

	double* const all_energies = new double[nVertices * nContour];

	for (int i=0; i<nContour; ++i)
	{
		if (i%25==0)
			std::cout << (100.*i)/nContour << "%" << std::endl;
		for (int j=0; j<nVertices; ++j)
			all_energies[getIndex(j, i, nVertices)] = pairwise_energy(vertices, nVertices, dimVertices, contour, nContour, dimContour, i, j);
	}

	std::cout << "done." << std::endl;

	// find upper bound until loop

	bool optimum = false;

	// first cut is the whole shape
	int* const cuts 		= new int[nVertices];
	MinHeap upperBounds(nVertices);

	for(int i=0; i < nVertices; i++) {
		cuts[i] = 0;
	}
	int cutID, maxID = 0;
	upperBounds.push(0, 0);

	long meshIndex, minIndex;
	long pathBack, pathFront;
	double min;

	// keep searching until there are no more cuts or the optimum (a loop) was found
	while(!upperBounds.isEmpty() && !optimum)
	{
		cutID = upperBounds.pop().second;

		// run dijkstra on product manifold
		manifoldDijkstra(all_energies, mesh, vertices, nVertices, dimVertices, triangles, nTriangles, contour, nContour, dimContour, result, predecessors, cuts, cutID);

		// find minimum
		min = result[nVertices * (nContour-1)];
		minIndex = nVertices * (nContour-1);
		for(int i=(nVertices * (nContour-1)); i < nVertices * nContour; i++) {
			meshIndex = i - nVertices * (nContour-1);
			if(cuts[meshIndex] == cutID && result[i] < min) {
				min = result[i];
				minIndex = i;
			}
		}
		minIndex = getVertex(minIndex, nVertices);

		// create path
		vPath.clear();
		calcPath(getIndex(minIndex, nContour - 1, nVertices), predecessors, vPath);

		pathFront = getVertex(vPath.front(), nVertices);
		pathBack = getVertex(vPath.back(), nVertices);
		if(!vPath.empty() && pathFront == pathBack) {
			if(min <= upperBounds.peak()) {
				optimum = true;
			} else {
				upperBounds.push(min, cutID);
			}
		}  else {
			// cut shape in two smaller parts
			maxID++;
			shapeCut(mesh, nVertices, result, cuts, maxID, pathFront, pathBack);

			upperBounds.push(min, cutID);
			upperBounds.push(min, maxID);
		}
	}

	const double energy = result[getIndex(minIndex, nContour - 1, nVertices)];

// clean memory
	for (int i=0; i<nVertices; i++) {
		delete mesh.at(i);
	}

	delete[] result;
	delete[] predecessors;
	delete[] cuts;
	delete[] all_energies;

	return energy;
}
