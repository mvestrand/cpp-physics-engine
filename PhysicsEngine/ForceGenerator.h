#pragma once

#include "PhysicsMath.h"

class RigidBody;

class ForceGenerator
{
public:
	virtual void updateForce(RigidBody *body, real duration) = 0;
};

