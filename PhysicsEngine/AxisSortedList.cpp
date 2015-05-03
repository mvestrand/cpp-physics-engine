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
			&& (axis[j]->getCenter().SORT_AXIS - axis[j]->getRadius()) 
			< (axis[j-1]->getCenter().SORT_AXIS - axis[j-1]->getRadius()); j--) {

			/* Swap the objects */
			auto tmp = axis[j];
			axis[j] = axis[j - 1];
			axis[j - 1] = tmp;
		}
	}
}

void AxisSortedList::generateCollisions(std::unordered_set<CollisionPair> &collisions)
{
	std::vector<CollidableSphere *> activeObjects;
	for (auto it = axis.begin(); it != axis.end(); it++) {
		real pos = (*it)->getCenter().SORT_AXIS;
		real radius = (*it)->getRadius();

		/* Remove inactive objects */
		for (auto j = activeObjects.begin(); j != activeObjects.end();) {
			if (pos - radius > (*j)->getCenter().SORT_AXIS + (*j)->getRadius()) {
				j = activeObjects.erase(j);
			}
			else {
				j++;
			}
		}

		/* Test all active objects */
		for (auto obj : activeObjects) {
			vec3 pp = pos - obj->getCenter();
			real sqr_dist = pp.x * pp.x + pp.y * pp.y + pp.z * pp.z;
			real sqr_radius = radius - obj->getRadius();
			sqr_radius = sqr_radius * sqr_radius;

			if (sqr_dist < sqr_radius) {
				collisions.insert(CollisionPair(*it, obj));
			}
		}
	}
}

void AxisSortedList::insert(CollidableSphere *obj)
{
	assert(std::find(axis.begin(), axis.end(), obj) == axis.end());
	axis.push_back(obj);
}

void AxisSortedList::remove(CollidableSphere *obj)
{
	for (auto it = axis.begin(); it != axis.end(); it++) {
		if (*it == obj) {
			it = axis.erase(it);
			return;
		}
	}

}