/**
		@author Zorah Lähner (laehner@in.tum.de)
    @version 03.2016
*/
#include <vector>
#include <list>

using std::vector;
using std::list;

// creates the adjacency list of triangles into mesh
// vector should have appropriate size and be filled with initialised lists already
void buildMesh(const double* triangles, int nTriangles, vector<list<long>* >& mesh) {
	// add all edges
	for(int i=0; i < nTriangles; i++) {
		for (int j=0; j < 3; j++) {
			mesh.at(int(triangles[3*i+j]))->push_back(int(triangles[3*i+((j + 1) % 3)]));
			mesh.at(int(triangles[3*i+j]))->push_back(int(triangles[3*i+((j + 2) % 3)]));
		}
	}
	// remove double entries
	for(int i=0; i < mesh.size(); i++) {
		mesh.at(i)->sort();
		mesh.at(i)->unique();
	}
}
