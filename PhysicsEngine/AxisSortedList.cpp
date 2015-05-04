#include "stdafx.h"
#include "AxisSortedList.h"


AxisSortedList::AxisSortedList()
{
}


AxisSortedList::~AxisSortedList()
{
}


void AxisSortedList::sort()
{
	/* Merge sort the objects by their starting x-coordinate */
	for (int i = 0; i < axis.size(); i++) {
		for (int j = i; j > 0
			&& axis[j]->getMinPosx()
			< axis[j-1]->getMinPosx(); j--) {

			/* Swap the objects */
			auto tmp = axis[j];
			axis[j] = axis[j - 1];
			axis[j - 1] = tmp;
		}
	}
}

void AxisSortedList::getCollisions(CollisionSet &collisions)
{
	std::vector<CollisionObject *> activeObjects;
	for (auto it = axis.begin(); it != axis.end(); it++) {
		real pos = (*it)->getMinPosx();

		/* Remove inactive objects */
		for (auto j = activeObjects.begin(); j != activeObjects.end();) {
			if (pos > (*j)->getMaxPosx()) {
				j = activeObjects.erase(j);
			}
			else {
				j++;
			}
		}

		/* Test all active objects */
		for (auto obj : activeObjects) {
			if ((*it)->collisionTest(obj)) {
				collisions.insert(CollisionPair(*it, obj));
			}
			//vec3 pp = pos - obj->getCenter();
			//real sqr_dist = pp.x * pp.x + pp.y * pp.y + pp.z * pp.z;
			//real sqr_radius = radius - obj->getRadius();
			//sqr_radius = sqr_radius * sqr_radius;

			//if (sqr_dist < sqr_radius) {
			//	// REMOVED TEMPORARILY FOR COMPILING
			//	//collisions.insert(CollisionPair(*it, obj));
			//}
		}
	}
}

void AxisSortedList::getInterCollisions(CollisionSet &collisions, AxisSortedList *list)
{
	std::vector<CollisionObject *> &axis2 = list->axis;
	int i = 0;
	int j = 0;
	for (; i < axis.size(); i++) {
		// Move forward in a2 until an object could overlap
		while (j < axis2.size()
			&& axis2[j]->getMaxPosx() < axis[i]->getMinPosx()) {

			j++;
		}
		// End of a2, no more overlaps possible for any object
		if (j == axis2.size())
			return;
		// This object can have no more collisions, if a2 doesn't overlap
		if (axis2[j]->getMinPosx() > axis[i]->getMaxPosx())
			continue;
		// Test this object
		if (axis[i]->collisionTest(axis2[j]))
			collisions.insert(CollisionPair(axis[i], axis2[j]));

		// Test any objects that are also overlapping
		int tmp = j;
		j++;
		while (j < axis2.size() 
			&& axis2[j]->getMinPosx() < axis[i]->getMaxPosx()) {

			if (axis[i]->collisionTest(axis2[j]))
				collisions.insert(CollisionPair(axis[i], axis2[j]));
			j++;
		}
		j = tmp;
	}
}

void AxisSortedList::getObjectCollisions(CollisionSet &collisions, CollisionObject *object)
{
	// Binary search the container to find where it would be placed
	real min = object->getMinPosx();
	real max = object->getMaxPosx();
	for (int i = 0; i < axis.size(); i++) {
		if (max < axis[i]->getMinPosx()) { // No more collisions possible
			return;
		}
		if (axis[i]->getMaxPosx() > min) { // Possible collision
			if (object->collisionTest(axis[i])) {
				collisions.insert(CollisionPair(object, axis[i]));
			}
		}
	}
}


void AxisSortedList::addObject(CollisionObject *obj)
{
	assert(std::find(axis.begin(), axis.end(), obj) == axis.end());
	axis.push_back(obj);
}

void AxisSortedList::removeObject(CollisionObject *obj)
{
	for (auto it = axis.begin(); it != axis.end(); it++) {
		if (*it == obj) {
			it = axis.erase(it);
			return;
		}
	}

}

unsigned int AxisSortedList::getObjectCount()
{
	return (unsigned int)axis.size();
}
