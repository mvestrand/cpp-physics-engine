#pragma once

#include <vector>

#include "PhysicsMath.h"


class CollisionObject
{
public:
	virtual ~CollisionObject() {}
	virtual real getAADiameter() = 0;
	virtual void getBVCells(std::vector<vec3> &cells, vec3 cell_size) = 0;
	virtual real getMinPosx() = 0;
	virtual real getMinPosy() = 0;
	virtual real getMinPosz() = 0;
	virtual real getMaxPosx() = 0;
	virtual real getMaxPosy() = 0;
	virtual real getMaxPosz() = 0;
	virtual bool collisionTest(CollisionObject *obj) = 0;
};

