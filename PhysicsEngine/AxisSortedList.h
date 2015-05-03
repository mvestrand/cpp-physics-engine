#pragma once

#include <assert.h>

#include <vector>
#include <unordered_set>

#include "PhysicsMath.h"
#include "CollisionPair.h"

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
	void generateCollisions(std::unordered_set<CollisionPair> &collisions);

	void insert(CollidableSphere *obj);
	void remove(CollidableSphere *obj);

	std::vector<CollidableSphere *> axis;
};
