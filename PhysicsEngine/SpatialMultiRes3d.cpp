#include "stdafx.h"
#include "SpatialMultiRes3d.h"


SpatialMultiRes3d::SpatialMultiRes3d(vec3 base_size)
{
	real x = base_size.x;
	real y = base_size.y;
	real z = base_size.z;
	cell_sizes.push_back(vec3(x, y, z));
}


SpatialMultiRes3d::~SpatialMultiRes3d()
{
}

void SpatialMultiRes3d::updateObjects()
{
	std::vector<vec3> cells;
	std::vector<vec3> old_cells;
	std::vector<vec3> new_cells;

	for (auto it = objects.begin(); it != objects.end(); it++) {
		cells.clear();
		old_cells.clear();
		new_cells.clear();
		it->first->getBVCells(cells, cell_sizes[it->second.layer]);
		getComplements(it->second.t, cells, old_cells, new_cells);

		// Remove from cells it is no longer in
		for (auto i = old_cells.begin(); i != old_cells.end(); i++) {
			int x = i->x;
			int y = i->y;
			int z = i->z;
			CollisionTester *tester = layers[it->second.layer].get(x, y, z);
			assert(tester != NULL);
			tester->removeObject(it->first);
			// Remove empty testers
			if (tester->getObjectCount() == 0) {
				removeTester(x, y, z, it->second.layer, tester);
			}
		}
		// Add to cells it has just entered
		for (auto i = new_cells.begin(); i != new_cells.end(); i++) {
			int x = i->x;
			int y = i->y;
			int z = i->z;
			CollisionTester *tester = layers[it->second.layer].get(x, y, z);
			// Create tester if null
			if (tester == NULL) {
				tester = addTester(x, y, z, it->second.layer);
			}
			tester->addObject(it->first);
		}

		// Update the object container map
		it->second.t.clear();
		for (auto i = cells.begin(); i != cells.end(); i++) {
			it->second.t.push_back(*i);
		}
	}
}

void SpatialMultiRes3d::addLayers(int layer)
{
	if (layers.size() > layer) {
		return;
	}
	for (int i = layers.size(); i <= layer; i++){
		vec3 size = cell_sizes.back();
		size.x *= (real)LAYER_GROWTH;
		size.y *= (real)LAYER_GROWTH;
		size.z *= (real)LAYER_GROWTH;
		cell_sizes.push_back(size);
	}

	layers.resize(layer + 1);
	active.resize(layer + 1);
}

int SpatialMultiRes3d::getObjectLayerIndex(CollisionObject *object)
{
	real diameter = object->getAADiameter();
	real cell_size = cell_sizes[0].x; // Assumes that all axes are the same size
	int layer = 0;
	while (diameter > cell_size * LAYER_OVERFLOW_SIZE) {
		cell_size *= LAYER_GROWTH;
		layer++;
	}
	return layer;
}

CollisionTester *SpatialMultiRes3d::addTester(int x, int y, int z, int layer)
{
	assert(layers[layer].get(x, y, z) == NULL);

	CollisionTester *tester = CollisionTester::newTester();
	layers[layer].set(x, y, z, tester);
	active[layer].emplace(tester, vec3(x, y, z));
	return tester;
}

void SpatialMultiRes3d::removeTester(int x, int y, int z, int layer, CollisionTester *tester)
{
	assert(layers[layer].get(x, y, z) == tester);

	layers[layer].set(x, y, z, NULL);
	active[layer].erase(tester);
	CollisionTester::deleteTester(tester);
}



void SpatialMultiRes3d::getComplements(std::vector<vec3> &v1, std::vector<vec3> &v2,
	std::vector<vec3> &comp1, std::vector<vec3> &comp2) 
{
	// Find vec's in v1 and not v2
	for (auto i = v1.begin(); i != v1.end(); i++) {
		auto j = v2.begin();
		for (; j != v2.end(); j++) {
			if (((int)i->x == (int)j->x) && ((int)i->y == (int)j->y) && ((int)i->z == (int)j->z)) {
				//Found 
				break;
			}
		}
		if (j == v2.end()) { // No match found
			comp1.push_back(*i);
		}
	}

	// Find vec's in v2 and not v1
	for (auto i = v2.begin(); i != v2.end(); i++) {
		auto j = v1.begin();
		for (; j != v1.end(); j++) {
			if (((int)i->x == (int)j->x) && ((int)i->y == (int)j->y) && ((int)i->z == (int)j->z)) {
				//Found 
				break;
			}
		}
		if (j == v1.end()) { // No match found
			comp2.push_back(*i);
		}
	}
}

void SpatialMultiRes3d::getIntersectingCells(vec3 cell, int layer,
	std::unordered_set<Vec4i> &intersecting)
{
	// Add this cell
	intersecting.insert(Vec4i(cell.x, cell.y, cell.z, layer));

	// Add cells below this cell
	int x = cell.x;
	int y = cell.y;
	int z = cell.z;
	// !!! MISSING CODE !!!

	// Add cells above this cell
	x = cell.x;
	y = cell.y;
	z = cell.z;
	for (int i = layer + 1; i < active.size(); i++) {
		x = (int)floor(x * LAYER_GROWTH_INVERSE);
		y = (int)floor(y * LAYER_GROWTH_INVERSE);
		z = (int)floor(z * LAYER_GROWTH_INVERSE);
		intersecting.insert(Vec4i(x, y, z, i));
	}
}


bool SpatialMultiRes3d::hasObject(CollisionObject *obj)
{
	auto it = objects.find(obj);
	return (it != objects.end());
}

void SpatialMultiRes3d::addObject(CollisionObject *obj)
{
	assert(!hasObject(obj));
	int layer = getObjectLayerIndex(obj);
	addLayers(layer);
	std::vector<vec3> cells;
	obj->getBVCells(cells, cell_sizes[layer]);

	// Add to cells it has just entered
	for (auto i = cells.begin(); i != cells.end(); i++) {
		int x = i->x;
		int y = i->y;
		int z = i->z;
		CollisionTester *tester = layers[layer].get(x, y, z);
		// Create tester if null
		if (tester == NULL) {
			tester = addTester(x, y, z, layer);
		}
		tester->addObject(obj);
	}
	// Add to object map
	objects.emplace(obj, Cells(layer, cells));
}

void SpatialMultiRes3d::removeObject(CollisionObject *obj)
{
	assert(hasObject(obj));

	// Remove from all object containers
	auto mapped_obj = objects.find(obj);
	for (auto i = mapped_obj->second.t.begin(); i != mapped_obj->second.t.end(); i++) {
		int x = i->x;
		int y = i->y;
		int z = i->z;
		int layer = mapped_obj->second.layer;
		CollisionTester *tester = layers[layer].get(x, y, z);
		tester->removeObject(obj);
		// Remove empty testers
		if (tester->getObjectCount() == 0) {
			removeTester(x, y, z, layer, tester);
		}
	}

	// Remove from object map
	objects.erase(obj);
}

void SpatialMultiRes3d::updateObject(CollisionObject *obj)
{
	assert(false);
}

void SpatialMultiRes3d::getCollisions(CollisionSet &collisions)
{
	for (int layer = 0; layer < active.size(); layer++) {
		for (auto active_cell : active[layer]) {
			// Get intra cell collisions
			active_cell.first->getCollisions(collisions);

			// Collide with parent cells
			int x = active_cell.second.x;
			int y = active_cell.second.y;
			int z = active_cell.second.z;
			for (int i = layer+1; i < active.size(); i++) {
				x = (int)floor(x * LAYER_GROWTH_INVERSE);
				y = (int)floor(y * LAYER_GROWTH_INVERSE);
				z = (int)floor(z * LAYER_GROWTH_INVERSE);
				CollisionTester *tester = layers[i].get(x, y, z);
				if (tester != NULL) {
					tester->getInterCollisions(collisions, active_cell.first);
				}
			}
		}
	}
}

void SpatialMultiRes3d::getCollisionsWithObject(CollisionSet &collisions, CollisionObject *obj)
{
	int layer = getObjectLayerIndex(obj);
	std::vector<vec3> cells;
	obj->getBVCells(cells, cell_sizes[layer]);
	std::unordered_set<Vec4i> intersecting;
	for (auto cell : cells) {
		getIntersectingCells(cell, layer, intersecting);
	}
	for (auto it = intersecting.begin(); it != intersecting.end(); it++) {
		int x = it->x;
		int y = it->y;
		int z = it->z;
		int l = it->w;
		CollisionTester *tester = layers[l].get(x, y, z);
		if (tester != NULL) {
			tester->getObjectCollisions(collisions, obj);
		}
	}
}