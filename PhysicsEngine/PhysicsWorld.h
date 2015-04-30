#pragma once

#include "PhysicsMath.h"
#include "RigidBody.h"

class PhysicsWorld
{
public:

	std::vector<RigidBody*> rigidBodies;

	void update(real duration);

	void updateForces(real duration);
	void integrate(real duration);

	void addRigidBody(RigidBody *body);
	void removeRigidBody(RigidBody *body);
};

