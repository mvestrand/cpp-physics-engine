#pragma once

#include <unordered_set>

#include "PhysicsMath.h"
#include "CollisionPair.h"

class CollisionTester
{
public:
	virtual void addObject(CollisionObject *object) {};
	virtual void removeObject(CollisionObject *object) {};

	// Gets intra collisions only
	virtual void getCollisions(std::unordered_set<CollisionPair> &collisions) {};
	// Gets inter collisions only (objects in the same tester do not collide)
	virtual void getInterCollisions(std::unordered_set<CollisionPair> &collisions, CollisionTester *tester) {};
	// Gets collisions with a single object
	virtual void getObjectCollisions(std::unordered_set<CollisionPair> &collisions, CollisionObject *object) {};

	static CollisionTester *newTester();
	static void deleteTester(CollisionTester *tester);
};