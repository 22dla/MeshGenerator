#ifndef MESH_H
#define MESH_H

#include <point.h>
#include <triangle.h>

/* Mesh class represents a 3D mesh constructed from a set of points.
* It provides methods for computing the mesh's facets and for generating
* a triangulated mesh from a vector of points. The class uses the convex
* hull algorithm to generate a set of triangles that cover the input points.
* It also provides methods for optimizing the mesh by removing unnecessary
* triangles and swapping edges to improve triangle quality.
* The mesh is stored as a vector of triangles, each represented
* by three pointers to the vertices that define it.
* The class also stores a vector of facets, which are represented
* as tuples of three indices that correspond to the vertices of each facet.*/

class Mesh {
public:
	Mesh();
	Mesh(std::vector<Vec3*> points);
	~Mesh();

	std::vector<std::tuple<int, int, int>> GetFacets();
	std::vector<std::tuple<int, int, int>> GetTriangulationResult(std::vector<Vec3*>& points);
private:
	void ConstructConvexHull(std::vector<Vec3*>* points);
	void AddPointToHull(Vec3* point);
	void RemoveUnnecessaryTriangles();
	void SubdivideTriangle(Triangle* triangle, Vec3* point);
	void UpdateTriangleNeighborhood(Triangle* target, Triangle* oldNeighbor, Triangle* newNeighbor);
	void OptimizeTrianglePair(Triangle* t0, Triangle* t1);
	bool TrySwapDiagonal(Triangle* t0, Triangle* t1);
	bool HasMinimumValueAtIndex(const std::vector<double>& vec, int index);
	double ComputeVectorDistance(Vec3 v0, Vec3 v1);
	double CalculateDeterminant(Vec3 v0, Vec3 v1, Vec3 v2);
	double CalculateDeterminant(double matrix[]);

	Vec3* _SupportingPoints[6];
	std::vector<Vec3*>* _ProjectedPoints;
	std::vector<Triangle*>* _Mesh;
	std::vector<std::tuple<int, int, int>> _Facets;
};
#endif
