#include "stdafx.h"
#include "PhysicsWorld.h"


void PhysicsWorld::update(real duration)
{
	updateForces(duration);
	integrate(duration);
}

void PhysicsWorld::updateForces(real duration)
{
	for (auto body : rigidBodies) {
		body->updateForces(duration);
	}
}

void PhysicsWorld::integrate(real duration)
{
	for (auto body : rigidBodies) {
		body->integrate(duration);
	}
}

