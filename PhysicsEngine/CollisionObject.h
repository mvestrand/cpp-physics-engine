#pragma once

#include <vector>

#include "PhysicsMath.h"


class CollisionObject
{
public:
	virtual ~CollisionObject() {}
	virtual real getAADiameter() = 0;
	virtual void getBVCells(std::vector<vec3> &cells, vec3 cell_size) = 0;
};

