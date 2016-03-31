/**
 *
 * Calculates a closed path on a 3D model minimizing the descriptor distortion
 * to a given 2D contour.
 * Usage: [energy, path, correspondence] = dijkstra(vertices, triangles, contour)
 * vertices - k x n where the first three rows are the coordinates of each vertex
 * 						and the next k - 3 the features, n is the number of vertices
 * triangles - 3 x m, m is the number of triangles
 * contour - k x c where the first two rows are the coordinates of each contour
 * 					 point, the third row is zero, and the next k - 3 rows are the
 * 					 features, c is the number of points on the contour
 * energy - energy of the minimal energy path
 * path - ordered list of indices on the 3D model corresponding to the minimal
 * 				energy path
 * correspondence - ordered list of indices on the 2D contour corresponding
 * 									to the minimal energy path
 *
 * Compile with: mex -v dijkstra.cpp MinHeap.cpp
 *
 * Indices are expected to be in cpp format (starting at 0). No further
 * checking of the mesh is done, faulty meshes will lead to general exceptions.
 *
 * @author Zorah Lähner (laehner@in.tum.de)
 * @version 03.2016
 *
 */

#include "mex.h"

#include <exception>
#include <fstream>
#include <list>
#include <iostream>
#include <string>
#include <cmath>

#include "buildMesh.h"
#include "helper.h"
#include "energy.h"
#include "manifoldDijkstra.h"
#include "MinHeap.h"
#include "shapeCut.h"
#include "dijkstra_main.h"

using namespace std;

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray*prhs[])
{
// check and read mex input
	if (nrhs != 3 || nlhs != 3)
		mexErrMsgTxt("Usage: [energy, path, correspondence] = dijkstra(vertices, triangles, contour). Transform indices in triangles to initial 0.");

	// read vertices
	const double* const vertices = mxGetPr(prhs[0]);
	const long nVertices = long( mxGetN(prhs[0]) );
	const int dimVertices = int( mxGetM(prhs[0]) );

	// read triangles
	const double* const triangles = mxGetPr(prhs[1]);
	const long nTriangles = long( mxGetN(prhs[1]) );
	const int dimTriangles = int( mxGetM(prhs[1]) );

	// read contour
	const double* const contour = mxGetPr(prhs[2]);
	const long nContour = long( mxGetN(prhs[2]) );
	const int dimContour = int( mxGetM(prhs[2]) );

	if (dimTriangles != 3)
		mexErrMsgTxt("Triangles must be given in 3xm.");

	if (dimContour != dimVertices)
		mexErrMsgTxt("Descriptors must have the same dimension. (Remove this constraint if you changed the energy function to handle different dimensional features.)");

try
{

	list<long> vPath;
	const double energy = dijkstra_main(vertices, nVertices, dimVertices, triangles, nTriangles, contour, nContour, dimContour, vPath);

	// transform results for mex

	// return loop
	plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
	plhs[1] = mxCreateDoubleMatrix(vPath.size(), 1, mxREAL);
	plhs[2] = mxCreateDoubleMatrix(vPath.size(), 1, mxREAL);
	double* e = mxGetPr(plhs[0]);
	double* resultPath = mxGetPr(plhs[1]);
	double* resultCorr = mxGetPr(plhs[2]);

	// energy
	e[0] = energy;

	// path
	const int s = vPath.size();
	for(long i=0; i < s; i++) {
		resultPath[i] = getVertex(vPath.front(), nVertices);
		resultCorr[i] = getContour(vPath.front(), nVertices);
		vPath.pop_front();
	}

} catch (std::exception& e)
	{
		const std::string msg = "Exception caught: " + std::string(e.what());
		mexErrMsgTxt(msg.c_str());
	}

} // close mexFunction
