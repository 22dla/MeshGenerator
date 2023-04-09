#include <string>
#include <triangle.h>

using namespace std;

Triangle::Triangle(Vec3* v0, Vec3* v1, Vec3* v2) {
	Points[0] = v0; Points[1] = v1;	Points[2] = v2;
	Id = GenerateNextId();
}

int Triangle::GenerateNextId() {
	static int id = 0;
	return id++;
}

bool Triangle::ContainsPoint(Vec3 point) {
	return Points[0]->Equals(point)
		|| Points[1]->Equals(point)
		|| Points[2]->Equals(point);
}

void Triangle::AssignNeighbors(Triangle* n0, Triangle* n1, Triangle* n2) {
	Neighbor[0] = n0;
	Neighbor[1] = n1;
	Neighbor[2] = n2;
}
