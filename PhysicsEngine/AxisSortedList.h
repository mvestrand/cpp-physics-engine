#pragma once

#include <assert.h>

#include <vector>
#include <unordered_set>

#include "PhysicsMath.h"
#include "CollisionPair.h"
#include "CollisionTester.h"

#define SORT_AXIS x



//inline bool operator==(CollisionPair const& lhs, CollisionPair const& rhs) {
//	return (lhs.a == rhs.a && lhs.b == rhs.b);
//}

class AxisSortedList
{
public:
	AxisSortedList();
	~AxisSortedList();

	void sort();
	void getCollisions(CollisionSet &collisions);
	void getInterCollisions(CollisionSet &collisions, AxisSortedList *list);
	void getObjectCollisions(CollisionSet &collisions, CollisionObject *object);
	//void insert(CollidableSphere *obj);
	//void remove(CollidableSphere *obj);

	void addObject(CollisionObject *obj);
	void removeObject(CollisionObject *obj);
	unsigned int getObjectCount();

	static AxisSortedList *newInstance() { return new AxisSortedList; }
	static AxisSortedList *deleteInstance(AxisSortedList *inst) { delete inst; }

	std::vector<CollisionObject *> axis;
};
