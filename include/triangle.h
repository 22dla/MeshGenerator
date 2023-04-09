#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <point.h>

/* The Triangle class represents a triangle in a 3D mesh, 
* defined by three points in 3D space*/

class Triangle {
public:
	Vec3* Points[3];

	Triangle* Neighbor[3];

	Triangle(Vec3* v0, Vec3* v1, Vec3* v2);
	~Triangle() = default;

	bool ContainsPoint(Vec3 point);
	void AssignNeighbors(Triangle* n0, Triangle* n1, Triangle* n2);
	int Id = 0;
private:
	int GenerateNextId();
};
#endif
