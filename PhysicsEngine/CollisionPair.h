#pragma once

#include <assert.h>

#include <unordered_set>

#include "CollisionObject.h"
#include "PhysicsMath.h"

class CollidableSphere {
public:
	virtual vec3 getCenter() = 0;
	virtual real getRadius() = 0;
};

class CollisionPair {
public:
	CollisionPair(CollisionObject *ptr1, CollisionObject *ptr2){
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

	CollisionObject *a;
	CollisionObject *b;
};

typedef std::unordered_set<CollisionPair> CollisionSet;

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


