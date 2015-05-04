#pragma once

#include <unordered_set>

#include "PhysicsMath.h"
#include "CollisionPair.h"

class CollisionTester
{
public:
	virtual void addObject(CollisionObject *object) {}
	virtual void removeObject(CollisionObject *object) {}

	// Gets intra collisions only
	virtual void getCollisions(CollisionSet &collisions) {}
	// Gets inter collisions only (objects in the same tester do not collide)
	virtual void getInterCollisions(CollisionSet &collisions, CollisionTester *tester) {}
	// Gets collisions with a single object
	virtual void getObjectCollisions(CollisionSet &collisions, CollisionObject *object) {}

	virtual unsigned int getObjectCount() { return 1; }

	static CollisionTester *newTester() { return new CollisionTester; }
	static void deleteTester(CollisionTester *tester) { delete tester; }
};