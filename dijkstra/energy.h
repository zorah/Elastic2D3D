/**
    Functions for calculating energies on the product manifold.
    @author Zorah Lähner (laehner@in.tum.de)
    @version 03.2016
*/

// ****************************************************************************
// Used for precalculating all point wise energies.
// Insert your distance function for comparing 2D and 3D features here.
// The current version assigns a very high energy if the last component of
// the features (= segments) don't match. See inline comments to remove
// this behavior.
// @param c_cur index on the contour
// @param v_cur index on the model
double pairwise_energy( const double*   vertices,
                        int             nVertices,
                        int             dimVertices,
                        const double*   contour,
                        int             nContour,
                        int             dimContour,
                        int             c_cur,
                        long            v_cur)
{
  double e = 0.;

    // this is for retrieval, remove if not using SEGMENTS or similar indicator functions
    if (vertices[dimContour*v_cur+dimContour-1] != 0 && contour[dimContour*c_cur+dimContour-1] != 0 && vertices[dimContour*v_cur+dimContour-1] != contour[dimContour*c_cur+dimContour-1])
      return 1e7;
    else if (vertices[dimContour*v_cur+dimContour-1] != contour[dimContour*c_cur+dimContour-1])
       return 1e3;
    // end of SEGMENTS part

  for (int i=3; i<dimContour-1; ++i)
  // for (int i=3; i<dimContour; ++i) use this for-loop if your are not using SEGMENTS
    e += fabs( vertices[dimContour*v_cur+i] - contour[dimContour*c_cur+i] );

  return e;
}

// ****************************************************************************
// Integrates the energy from the point (c_prev, v_prev) to (c_cur, v_cur)
// on the product manifold. The energy is assumed to be linear inbetween the
// points and point-wise the one calculated from pointwise_energy (see above).
// @param c_prev index on the contour belonging to point 1
// @param v_prev index on the model beloning to point 1
// @param c_cur index on the contour beloning to point 2
// @param v_cur index on the model beloning to point 2
// @param all_energies contains all pointwise energies obtained by pointwise_energy
//                     use with all_energies[getIndex([index on model], [index on contour], nVertices)
double energy(const double* all_energies,
              const double* vertices,
              int           nVertices,
              int           dimVertices,
              const double* triangles,
              int           nTriangles,
              const double* contour,
              int           nContour,
              int           dimContour,
              const long*   predecessors,
              int           c_prev,
              int           c_cur,
              long          v_prev,
              long          v_cur)
{

  double dl = 0.;

  for (int i=0; i<3; ++i)
  {
    const double diff = vertices[dimContour*v_prev+i] - vertices[dimContour*v_cur+i];
    dl += diff*diff;
  }
  for (int i=0; i<2; ++i)
  {
    const double diff = contour[dimContour*c_prev+i] - contour[dimContour*c_cur+i];
    dl += diff*diff;
  }
  dl = std::sqrt(dl);


  const double e1 = all_energies[getIndex(v_cur, c_cur, nVertices)];
  const double e2 = all_energies[getIndex(v_prev, c_prev, nVertices)];

  double energy = 0.5*(e1+e2)*dl;

  return energy;
}
