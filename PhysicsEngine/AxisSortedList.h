#pragma once

#include <assert.h>

#include <vector>
#include <unordered_set>

#include "PhysicsMath.h"

#define SORT_AXIS x

class CollidableSphere {
public:
	virtual vec3 getCenter() = 0;
	virtual real getRadius() = 0;
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

	inline bool operator==(const CollisionPair &other) const {
		return (a == other.a && b == other.b);
	}

	CollidableSphere *a;
	CollidableSphere *b;
};

namespace std
{
	template <>
	struct hash < CollisionPair >
	{
		size_t operator() (const CollisionPair& k) const
		{
			return ((hash<void*>()(k.a)) ^ (hash<void*>()(k.b) << 1) >> 1);
		}
	};
}

//inline bool operator==(CollisionPair const& lhs, CollisionPair const& rhs) {
//	return (lhs.a == rhs.a && lhs.b == rhs.b);
//}

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
