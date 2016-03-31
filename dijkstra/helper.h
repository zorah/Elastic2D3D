/**
    Helper functions..
    @author Zorah Lähner (laehner@in.tum.de)
    @version 03.2016
*/

void calcPath(long end, const long* predecessors, list<long>& path) {
	long index = end;
	while(index > -1) {
		path.push_front(index);
		index = predecessors[index];
	}
}

long getVertex(long index, long nVertices) {
	return long(index % nVertices);
}

long getContour(long index, long nVertices) {
	return index / nVertices;
}

long getIndex(long vertex, long contour, long nVertices) {
	return contour * nVertices + vertex;
}

double curvature(double p1, double p2, double p3, double q1, double q2, double q3, double r1, double r2, double r3) {
	double a = sqrt((p1 - q1)*(p1 - q1) + (p2 - q2)*(p2 - q2) + (p3 - q3)*(p3 - q3));
	double b = sqrt((q1 - r1)*(q1 - r1) + (q2 - r2)*(q2 - r2) + (q3 - r3)*(q3 - r3));
	double c = sqrt((r1 - p1)*(r1 - p1) + (r2 - p2)*(r2 - p2) + (r3 - p3)*(r3 - p3));

	double p = (a + b + c) / 2;
	double k = sqrt(p*(p - a)*(p-b)*(p-c));

	return (4 * k) / (a * b * c);
}
