#pragma once

#include <assert.h>

#include <vector>
#include <unordered_set>

#include "PhysicsMath.h"

#define SORT_AXIS x

class CollidableSphere {
public:
	vec3 getCenter();
	real getRadius();
};

class CollisionPair {
public:
	CollisionPair(CollidableSphere *ptr1, CollidableSphere *ptr2){
		assert(ptr1 != ptr2);
		if (ptr1 < ptr2) {
			a = ptr1;
			b = ptr2;
		}
		else {
			a = ptr2;
			b = ptr1;
		}
	}

	CollidableSphere *a;
	CollidableSphere *b;
};

inline bool operator==(CollisionPair const& lhs, CollisionPair const& rhs) {
	return (lhs.a == rhs.a && lhs.b == rhs.b);
}

class AxisSortedList
{
public:
	AxisSortedList();
	~AxisSortedList();

	void sort();
	void generateCollisions(std::unordered_set<CollisionPair> collisions);

	void insert(CollidableSphere *obj);
	void remove(CollidableSphere *obj);

	std::vector<CollidableSphere *> axis;
};
