#ifndef MESH_H
#define MESH_H

#include <memory>
#include <vector>
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
	Mesh(const std::vector<Point3>& points);
	~Mesh();

	// Delete copy constructor and assignment operator (Mesh contains unique_ptr)
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	// Allow move constructor and move assignment
	Mesh(Mesh&&) = default;
	Mesh& operator=(Mesh&&) = default;

	const std::vector<std::tuple<int, int, int>>& GetFacets() const;
	std::vector<std::tuple<int, int, int>> GetTriangulationResult(const std::vector<Point3>& points);
private:
	void ConstructConvexHull(const std::vector<Point3>& points);
	void AddPointToHull(const Point3& point);
	void RemoveUnnecessaryTriangles();
	void SubdivideTriangle(Triangle* triangle, const Point3& point);
	void UpdateTriangleNeighborhood(Triangle* target, Triangle* oldNeighbor, Triangle* newNeighbor);
	void OptimizeTrianglePair(Triangle* t0, Triangle* t1);
	bool TrySwapDiagonal(Triangle* t0, Triangle* t1);
	bool HasMinimumValueAtIndex(const std::vector<double>& vec, int index);
	double ComputeVectorDistance(const Point3& v0, const Point3& v1);
	double CalculateDeterminant(const Point3& v0, const Point3& v1, const Point3& v2);
	double CalculateDeterminant(const std::vector<double>& matrix);

	Point3 _SupportingPoints[6];
	std::vector<Point3> _ProjectedPoints;
	std::vector<std::unique_ptr<Triangle>> _Mesh;
	std::vector<std::tuple<int, int, int>> _Facets;
};
#endif
