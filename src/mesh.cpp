#include <string>
#include <tuple>
#include <vector>
#include <cmath>
#include <limits>
#include <mesh.h>

using namespace std;

Mesh::Mesh(const std::vector<Point3>& points)
	: Mesh() {
	_Facets = GetTriangulationResult(points);
}

Mesh::Mesh() {
	for (int i = 0; i < 6; i++) {
		_SupportingPoints[i] = Point3(
			(i % 2 == 0 ? 1 : -1) * (i / 2 == 0 ? 1 : 0),
			(i % 2 == 0 ? 1 : -1) * (i / 2 == 1 ? 1 : 0),
			(i % 2 == 0 ? 1 : -1) * (i / 2 == 2 ? 1 : 0),
			true
		);
	}
}

Mesh::~Mesh() {
}

const std::vector<std::tuple<int, int, int>>& Mesh::GetFacets() {
	return this->_Facets;
}

vector<tuple<int, int, int>> Mesh::GetTriangulationResult(const vector<Point3>& points) {
	_ProjectedPoints.reserve(points.size());

	// N random points can form 8+(N-6)*2 triangles based on the algorithm
	_Mesh.reserve(8 + (points.size() - 6) * 2);

	// project points to an unit shpere for triangulation
	for(const auto& point : points) {
		_ProjectedPoints.emplace_back(point, 1.0);
	}

	// prepare initial convex hull with 6 vertices and 8 triangle faces
	ConstructConvexHull(_ProjectedPoints);

	for(const auto& projectedPoint : _ProjectedPoints) {
		if (!projectedPoint.IsVisited) {
			AddPointToHull(projectedPoint);
		}
	}

	RemoveUnnecessaryTriangles();

	vector<tuple<int, int, int>> mesh;
	mesh.reserve(_Mesh.size());
	for (auto it = _Mesh.begin(); it != _Mesh.end(); it++) {
		Triangle* triangle = *it;
		mesh.emplace_back(
			triangle->Points[0].Id,
			triangle->Points[1].Id,
			triangle->Points[2].Id
		);
	}
	return mesh;
}

void Mesh::ConstructConvexHull(const vector<Point3>& points) {
	std::vector<Point3> initialPoints(6);

	for (int i = 0; i < initialPoints.size(); i++) {
		initialPoints[i] = _SupportingPoints[i];
	}

	// if close enough, use input points to replace auxiliary points so won't be removed in the end
	std::vector<double> minDistance(initialPoints.size(), 0.0);
	for (auto it = points.begin(); it != points.end(); ++it) {
		std::vector<double> distance(initialPoints.size());
		for (int i = 0; i < distance.size(); ++i) {
			distance[i] = ComputeVectorDistance(_SupportingPoints[i], *it);
			if (minDistance[i] == 0 || distance[i] < minDistance[i]) {
				minDistance[i] = distance[i];
			}
		}

		for (int i = 0; i < distance.size(); i++) {
			if (minDistance[i] == distance[i] && HasMinimumValueAtIndex(distance, i)) {
				initialPoints[i] = *it;
			}
		}
	}

	std::vector<int> vertex0Index = { 0, 0, 0, 0, 1, 1, 1, 1 };
	std::vector<int> vertex1Index = { 4, 3, 5, 2, 2, 4, 3, 5 };
	std::vector<int> vertex2Index = { 2, 4, 3, 5, 4, 3, 5, 2 };
	std::vector <Triangle*> initialHullFaces(8);

	for (int i = 0; i < initialHullFaces.size(); i++) {
		const Point3& v0 = initialPoints[vertex0Index[i]];
		const Point3& v1 = initialPoints[vertex1Index[i]];
		const Point3& v2 = initialPoints[vertex2Index[i]];

		Triangle* triangle = new Triangle(v0, v1, v2);
		initialHullFaces[i] = triangle;

		_Mesh.push_back(triangle);
	}

	int neighbor0Index[] = { 1, 2, 3, 0, 7, 4, 5, 6 };
	int neighbor1Index[] = { 4, 5, 6, 7, 0, 1, 2, 3 };
	int neighbor2Index[] = { 3, 0, 1, 2, 5, 6, 7, 4 };

	for (int i = 0; i < initialHullFaces.size(); i++) {
		Triangle* n0 = initialHullFaces[neighbor0Index[i]];
		Triangle* n1 = initialHullFaces[neighbor1Index[i]];
		Triangle* n2 = initialHullFaces[neighbor2Index[i]];
		initialHullFaces[i]->AssignNeighbors(n0, n1, n2);
	}

	// point already in the mesh, avoid being visited by InsertDot() again
	for (int i = 0; i < initialPoints.size(); i++) {
		initialPoints[i].IsVisited = true;
	}
}

void Mesh::AddPointToHull(const Point3& point) {
	std::vector<double> det = { 0, 0, 0 };

	auto it = _Mesh.begin();
	Triangle* triangle = *it;

	while (it != _Mesh.end()) {
		det[0] = CalculateDeterminant(triangle->Points[0], triangle->Points[1], point);
		det[1] = CalculateDeterminant(triangle->Points[1], triangle->Points[2], point);
		det[2] = CalculateDeterminant(triangle->Points[2], triangle->Points[0], point);

		// if this point projected into an existing triangle, split the existing triangle to 3 new ones
		if (det[0] >= 0 && det[1] >= 0 && det[2] >= 0) {
			if (!triangle->ContainsPoint(point)) {
				SubdivideTriangle(triangle, point);
			}
			return;
		}

		// on one side, search neighbors
		else if (det[1] >= 0 && det[2] >= 0)
			triangle = triangle->Neighbor[0];
		else if (det[0] >= 0 && det[2] >= 0)
			triangle = triangle->Neighbor[1];
		else if (det[0] >= 0 && det[1] >= 0)
			triangle = triangle->Neighbor[2];

		// cannot determine effectively 
		else if (det[0] >= 0)
			triangle = triangle->Neighbor[1];
		else if (det[1] >= 0)
			triangle = triangle->Neighbor[2];
		else if (det[2] >= 0)
			triangle = triangle->Neighbor[0];
		else
			triangle = *it++;
	}
}

void Mesh::RemoveUnnecessaryTriangles() {
	for (auto it = _Mesh.begin(); it != _Mesh.end();) {
		Triangle* triangle = *it;
		bool isUnnecessaryTriangle = false;
		for (int i = 0; i < 3; i++) {
			if (triangle->Points[i].IsSupportPoint) {
				isUnnecessaryTriangle = true;
				break;
			}
		}

		if (isUnnecessaryTriangle) {
			delete* it;
			it = _Mesh.erase(it);
		}
		else {
			++it;
		}
	}
}

void Mesh::SubdivideTriangle(Triangle* triangle, const Point3& point) {
	Triangle* newTriangle1 = new Triangle(point, triangle->Points[1], triangle->Points[2]);
	Triangle* newTriangle2 = new Triangle(point, triangle->Points[2], triangle->Points[0]);

	triangle->Points[2] = triangle->Points[1];
	triangle->Points[1] = triangle->Points[0];
	triangle->Points[0] = point;

	newTriangle1->AssignNeighbors(triangle, triangle->Neighbor[1], newTriangle2);
	newTriangle2->AssignNeighbors(newTriangle1, triangle->Neighbor[2], triangle);
	triangle->AssignNeighbors(newTriangle2, triangle->Neighbor[0], newTriangle1);

	UpdateTriangleNeighborhood(newTriangle1->Neighbor[1], triangle, newTriangle1);
	UpdateTriangleNeighborhood(newTriangle2->Neighbor[1], triangle, newTriangle2);

	_Mesh.push_back(newTriangle1);
	_Mesh.push_back(newTriangle2);

	// optimize triangles according to delaunay triangulation definition
	OptimizeTrianglePair(triangle, triangle->Neighbor[1]);
	OptimizeTrianglePair(newTriangle1, newTriangle1->Neighbor[1]);
	OptimizeTrianglePair(newTriangle2, newTriangle2->Neighbor[1]);
}

void Mesh::UpdateTriangleNeighborhood(Triangle* target, Triangle* oldNeighbor, Triangle* newNeighbor) {
	for (int i = 0; i < 3; i++) {
		if (target->Neighbor[i] == oldNeighbor) {
			target->Neighbor[i] = newNeighbor;
			break;
		}
	}
}

void Mesh::OptimizeTrianglePair(Triangle* t0, Triangle* t1) {
	for (int i = 0; i < 3; i++) {
		if (t1->Points[i].Id == t0->Points[0].Id ||
			t1->Points[i].Id == t0->Points[1].Id ||
			t1->Points[i].Id == t0->Points[2].Id) {
			continue;
		}

		std::vector<double> matrix = {
			t1->Points[i].X - t0->Points[0].X,
			t1->Points[i].Y - t0->Points[0].Y,
			t1->Points[i].Z - t0->Points[0].Z,

			t1->Points[i].X - t0->Points[1].X,
			t1->Points[i].Y - t0->Points[1].Y,
			t1->Points[i].Z - t0->Points[1].Z,

			t1->Points[i].X - t0->Points[2].X,
			t1->Points[i].Y - t0->Points[2].Y,
			t1->Points[i].Z - t0->Points[2].Z
		};

		if (CalculateDeterminant(matrix) <= 0) {
			break;
		}

		if (TrySwapDiagonal(t0, t1)) {
			return;
		}
	}
}

bool Mesh::TrySwapDiagonal(Triangle* t0, Triangle* t1) {
	for (int j = 0; j < 3; j++) {
		for (int k = 0; k < 3; k++) {
			if (t0->Points[j].Id != t1->Points[0].Id &&
				t0->Points[j].Id != t1->Points[1].Id &&
				t0->Points[j].Id != t1->Points[2].Id &&
				t1->Points[k].Id != t0->Points[0].Id &&
				t1->Points[k].Id != t0->Points[1].Id &&
				t1->Points[k].Id != t0->Points[2].Id) {
				t0->Points[(j + 2) % 3] = t1->Points[k];
				t1->Points[(k + 2) % 3] = t0->Points[j];

				t0->Neighbor[(j + 1) % 3] = t1->Neighbor[(k + 2) % 3];
				t1->Neighbor[(k + 1) % 3] = t0->Neighbor[(j + 2) % 3];
				t0->Neighbor[(j + 2) % 3] = t1;
				t1->Neighbor[(k + 2) % 3] = t0;

				UpdateTriangleNeighborhood(t0->Neighbor[(j + 1) % 3], t1, t0);
				UpdateTriangleNeighborhood(t1->Neighbor[(k + 1) % 3], t0, t1);

				OptimizeTrianglePair(t0, t0->Neighbor[j]);
				OptimizeTrianglePair(t0, t0->Neighbor[(j + 1) % 3]);
				OptimizeTrianglePair(t1, t1->Neighbor[k]);
				OptimizeTrianglePair(t1, t1->Neighbor[(k + 1) % 3]);

				return true;
			}
		}
	}

	return false;
}

bool Mesh::HasMinimumValueAtIndex(const std::vector<double>& vec, int index) {
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] < vec[index]) {
			return false;
		}
	}

	return true;
}

double Mesh::ComputeVectorDistance(const Point3& v0, const Point3& v1) {
	return sqrt(pow((v0.X - v1.X), 2) +
		pow((v0.Y - v1.Y), 2) +
		pow((v0.Z - v1.Z), 2));
}

double Mesh::CalculateDeterminant(const Point3& v0, const Point3& v1, const Point3& v2) {
	std::vector<double> matrix = {
		v0.X, v0.Y, v0.Z,
		v1.X, v1.Y, v1.Z,
		v2.X, v2.Y, v2.Z
	};

	return CalculateDeterminant(matrix);
}

double Mesh::CalculateDeterminant(const std::vector<double>& matrix) {
	double determinant = matrix[2] * matrix[4] * matrix[6]
		+ matrix[0] * matrix[5] * matrix[7]
		+ matrix[1] * matrix[3] * matrix[8]
		- matrix[0] * matrix[4] * matrix[8]
		- matrix[1] * matrix[5] * matrix[6]
		- matrix[2] * matrix[3] * matrix[7];
	return abs(determinant) <= std::numeric_limits<double>::epsilon() ? 0 : determinant;
}
