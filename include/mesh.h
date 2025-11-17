#ifndef MESH_H
#define MESH_H

#include <memory>
#include <tuple>
#include <vector>

#include <point.h>
#include <triangle.h>

// Mesh class represents a 3D mesh constructed from a set of points.
// It provides methods for computing the mesh's facets and for generating
// a triangulated mesh from a vector of points. The class uses the convex
// hull algorithm to generate a set of triangles that cover the input points.
// It also provides methods for optimizing the mesh by removing unnecessary
// triangles and swapping edges to improve triangle quality.

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
	bool HasMinimumValueAtIndex(const std::vector<double>& vec, int index) const;
	double ComputeVectorDistance(const Point3& v0, const Point3& v1) const;
	double CalculateDeterminant(const Point3& v0, const Point3& v1, const Point3& v2) const;
	double CalculateDeterminant(const std::vector<double>& matrix) const;

	Point3 _SupportingPoints[6];
	std::vector<Point3> _ProjectedPoints;
	std::vector<std::unique_ptr<Triangle>> _Mesh;
	std::vector<std::tuple<int, int, int>> _Facets;
};
#endif
