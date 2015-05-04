#pragma once

#include <unordered_set>

#include "PhysicsMath.h"
#include "CollisionPair.h"

class CollisionTester
{
public:
	virtual void addObject(CollisionObject *object) = 0;
	virtual void removeObject(CollisionObject *object) = 0;

	// Gets intra collisions only
	virtual void getCollisions(CollisionSet &collisions) = 0;
	// Gets inter collisions only (objects in the same tester do not collide)
	virtual void getInterCollisions(CollisionSet &collisions, CollisionTester *tester) = 0;
	// Gets collisions with a single object
	virtual void getObjectCollisions(CollisionSet &collisions, CollisionObject *object) = 0;

	virtual unsigned int getObjectCount() = 0;

	static CollisionTester *newInstance() { return NULL; }
	static void deleteInstance(CollisionTester *tester) {  }
};