#include "stdafx.h"

#include "PointerGrid3d.h"
#include "SpatialMultiRes3d.h"
#include "AxisSortedList.h"
#include "CollisionObject.h"

class TestCollisionObject : public CollisionObject
{
public:
	TestCollisionObject(real r, vec3 p) {
		radius = r;
		pos.x = (real)p.x;
		pos.y = (real)p.y;
		pos.z = (real)p.z;
	}
	~TestCollisionObject() {}
	real getAADiameter() { return radius * 2; };
	void getBVCells(std::vector<vec3> &cells, vec3 cell_size) {
		// !! Does not account for being in multiple cells
		int x = (int)floor(pos.x / cell_size.x);
		int y = (int)floor(pos.y / cell_size.y);
		int z = (int)floor(pos.z / cell_size.z);
		cells.push_back(vec3(x, y, z));
	};
	real getMinPosx(){ return pos.x - radius; }
	real getMinPosy(){ return pos.y - radius; }
	real getMinPosz(){ return pos.z - radius; }
	real getMaxPosx(){ return pos.x + radius; }
	real getMaxPosy(){ return pos.y + radius; }
	real getMaxPosz(){ return pos.z + radius; }
	bool collisionTest(CollisionObject *obj){ 
		TestCollisionObject *object = (TestCollisionObject *)obj;
		vec3 pp = pos - object->pos;
		real sqr_dist = pp.x * pp.x + pp.y * pp.y + pp.z * pp.z;
		real sqr_radius = radius + object->radius;
		sqr_radius = sqr_radius * sqr_radius;
		if (sqr_dist < sqr_radius)
			return true;
	};

	real radius;
	vec3 pos;
};

int main(int argc, char **argv)
{
	SpatialMultiRes3d<AxisSortedList> grid(vec3((real)16.0, (real)16.0, (real)16.0));
	TestCollisionObject a(1.0, vec3(0.0, 0.0, 5.0));
	TestCollisionObject b(1.0, vec3(0.0, 0.0, 0.0));
	TestCollisionObject c(1.0, vec3(10.0, 0.0, 5.0));
	grid.addObject(&a);
	grid.addObject(&b);
	grid.addObject(&c);
	vec3 va((real)0.0, (real)0.0, (real)-0.05);
	vec3 vb((real)0.0, (real)0.0, (real)0.05);
	vec3 vc((real)0.05, (real)-0.5, (real)-0.5);

	int t = 0;
	while (t < 10000) {
		grid.updateObjects();
		CollisionSet collisions;
		grid.getCollisions(collisions);
		for (auto it = collisions.begin(); it != collisions.end(); it++) {
			printf("collision[%p][%p] at t=%d\n", it->a, it->b, t);
		}
		a.pos += va;
		b.pos += vb;
		c.pos += vc;
		t++;
	}



	grid.removeObject(&a);
	grid.removeObject(&b);
	grid.removeObject(&c);
	/* Testing pointer grid*/
	//int x, y, z, val;

	//PointerGrid3d<int> *grid = new PointerGrid3d < int >;
	//bool run = true;
	//while (run) {
	//	int opt = 0;
	//	printf("enter(1), print(2), exit(3): ");
	//	scanf_s("%d", &opt);

	//	switch (opt) {
	//	case 1:
	//		printf("x y z: ");
	//		scanf_s("%d %d %d", &x, &y, &z);
	//		printf("value: ");
	//		scanf_s("%d", &val);
	//		grid->set(x, y, z, val);
	//		break;
	//	case 2:
	//		printf("x y z: ");
	//		scanf_s("%d %d %d", &x, &y, &z);
	//		printf("(%d,%d,%d) = %d\n", x, y, z, grid->get(x, y, z));
	//		break;
	//	default:
	//		run = false;
	//	}

	//}

	//delete grid;
	//PointerGridChunk3d<int>::deletePool();

	return 0;
}