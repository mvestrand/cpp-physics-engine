#pragma once

#include <unordered_set>
#include <unordered_map>

#include "CollisionPair.h"
#include "AxisSortedList.h"
#include "PointerGrid3d.h"


class SpatialGrid3d
{
	typedef std::vector<AxisSortedList*> Buckets;

	void getBuckets(Buckets *b);

public:


	SpatialGrid3d();
	~SpatialGrid3d();

	void updateBuckets();
	void generateCollisions(std::unordered_set<CollisionPair> &collisions);

	PointerGrid3d<AxisSortedList*> buckets;
	std::unordered_set<AxisSortedList*> active;
	std::unordered_map<CollidableSphere *, Buckets> obj_buckets;
};
