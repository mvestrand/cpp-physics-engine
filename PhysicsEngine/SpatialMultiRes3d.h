#pragma once

#include <unordered_set>
#include <unordered_map>

#include "CollisionObject.h"
#include "CollisionPair.h"
#include "CollisionTester.h"
#include "PointerGrid3d.h"

#define LAYER_GROWTH 2
#define LAYER_GROWTH_INVERSE (1.0 / LAYER_GROWTH)
#define LAYER_OVERFLOW_SIZE 0.5

class Vec4i {
public:
	int x;
	int y;
	int z;
	int w;
	Vec4i(int a, int b, int c, int d) {
		x = a;
		y = b;
		z = c;
		w = d;
	}

	inline bool operator==(const Vec4i &other) const {
		return (x == other.x && y == other.y && z == other.z && w == other.w);
	}
};

template <class Bucket>
class SpatialMultiRes3d
{
public:
private:
	class Cells {
	public:
		unsigned int layer;
		std::vector<vec3> t;
		Cells(int l, std::vector<vec3> &cells) {
			t = cells;
			layer = l;
		}
		Cells(int l) {
			layer = l;
		}
	};

	typedef std::unordered_map<Bucket*, vec3> TesterMap;
	typedef std::unordered_map<CollisionObject*, Cells> CellMap;
	typedef PointerGrid3d<Bucket*> GridLayer;

	void updateObjects();
	void addLayers(int layer);
	int getObjectLayerIndex(CollisionObject *object);
	Bucket *addTester(int x, int y, int z, int layer);
	void removeTester(int x, int y, int z, int layer, Bucket *tester);
	// Comp1 - in v1 not v2 //Comp2 - in v2 not v1
	void getComplements(std::vector<vec3> &v1, std::vector<vec3> &v2, 
		std::vector<vec3> &comp1, std::vector<vec3> &comp2);
	void getIntersectingCells(vec3 cell, int layer, std::unordered_set<Vec4i> &intersecting);
	
public:

	SpatialMultiRes3d(vec3 base_size);
	~SpatialMultiRes3d();

	void getCollisions(CollisionSet &collisions);
	void getCollisionsWithObject(CollisionSet &collisions, CollisionObject *object);

	void addObject(CollisionObject *obj);
	void removeObject(CollisionObject *obj);
	bool hasObject(CollisionObject *obj);

	// Update an object after it has been moved or resized
	void updateObject(CollisionObject *obj);

	std::vector<GridLayer> layers;
	std::vector<TesterMap> active;
	CellMap objects;
	std::vector<vec3> cell_sizes;
};

namespace std
{
	template <>
	struct hash < Vec4i >
	{
		size_t operator() (const Vec4i& k) const
		{
			return (((hash<int>()(k.x)) ^ (hash<int>()(k.y) << 1) >> 1)
				^ ((hash<int>()(k.z)) ^ (hash<int>()(k.w) << 2) >> 2));
		}
	};
}

template <class Bucket>
SpatialMultiRes3d<Bucket>::SpatialMultiRes3d(vec3 base_size)
{
	real x = base_size.x;
	real y = base_size.y;
	real z = base_size.z;
	cell_sizes.push_back(vec3(x, y, z));
}

template <class Bucket>
SpatialMultiRes3d<Bucket>::~SpatialMultiRes3d()
{
}

template <class Bucket>
void SpatialMultiRes3d<Bucket>::updateObjects()
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
			Bucket *tester = layers[it->second.layer].get(x, y, z);
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
			Bucket *tester = layers[it->second.layer].get(x, y, z);
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

template <class Bucket>
void SpatialMultiRes3d<Bucket>::addLayers(int layer)
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

template <class Bucket>
int SpatialMultiRes3d<Bucket>::getObjectLayerIndex(CollisionObject *object)
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

template <class Bucket>
Bucket *SpatialMultiRes3d<Bucket>::addTester(int x, int y, int z, int layer)
{
	assert(layers[layer].get(x, y, z) == NULL);

	Bucket *tester = Bucket::newInstance();
	layers[layer].set(x, y, z, tester);
	active[layer].emplace(tester, vec3(x, y, z));
	return tester;
}

template <class Bucket>
void SpatialMultiRes3d<Bucket>::removeTester(int x, int y, int z, int layer, Bucket *tester)
{
	assert(layers[layer].get(x, y, z) == tester);

	layers[layer].set(x, y, z, NULL);
	active[layer].erase(tester);
	Bucket::deleteInstance(tester);
}



template <class Bucket>
void SpatialMultiRes3d<Bucket>::getComplements(std::vector<vec3> &v1, std::vector<vec3> &v2,
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

template <class Bucket>
void SpatialMultiRes3d<Bucket>::getIntersectingCells(vec3 cell, int layer,
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


template <class Bucket>
bool SpatialMultiRes3d<Bucket>::hasObject(CollisionObject *obj)
{
	auto it = objects.find(obj);
	return (it != objects.end());
}

template <class Bucket>
void SpatialMultiRes3d<Bucket>::addObject(CollisionObject *obj)
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
		Bucket *tester = layers[layer].get(x, y, z);
		// Create tester if null
		if (tester == NULL) {
			tester = addTester(x, y, z, layer);
		}
		tester->addObject(obj);
	}
	// Add to object map
	objects.emplace(obj, Cells(layer, cells));
}

template <class Bucket>
void SpatialMultiRes3d<Bucket>::removeObject(CollisionObject *obj)
{
	assert(hasObject(obj));

	// Remove from all object containers
	auto mapped_obj = objects.find(obj);
	for (auto i = mapped_obj->second.t.begin(); i != mapped_obj->second.t.end(); i++) {
		int x = i->x;
		int y = i->y;
		int z = i->z;
		int layer = mapped_obj->second.layer;
		Bucket *tester = layers[layer].get(x, y, z);
		tester->removeObject(obj);
		// Remove empty testers
		if (tester->getObjectCount() == 0) {
			removeTester(x, y, z, layer, tester);
		}
	}

	// Remove from object map
	objects.erase(obj);
}

template <class Bucket>
void SpatialMultiRes3d<Bucket>::updateObject(CollisionObject *obj)
{
	assert(false);
}

template <class Bucket>
void SpatialMultiRes3d<Bucket>::getCollisions(CollisionSet &collisions)
{
	for (int layer = 0; layer < active.size(); layer++) {
		for (auto active_cell : active[layer]) {
			// Get intra cell collisions
			active_cell.first->getCollisions(collisions);

			// Collide with parent cells
			int x = active_cell.second.x;
			int y = active_cell.second.y;
			int z = active_cell.second.z;
			for (int i = layer + 1; i < active.size(); i++) {
				x = (int)floor(x * LAYER_GROWTH_INVERSE);
				y = (int)floor(y * LAYER_GROWTH_INVERSE);
				z = (int)floor(z * LAYER_GROWTH_INVERSE);
				Bucket *tester = layers[i].get(x, y, z);
				if (tester != NULL) {
					tester->getInterCollisions(collisions, active_cell.first);
				}
			}
		}
	}
}

template <class Bucket>
void SpatialMultiRes3d<Bucket>::getCollisionsWithObject(CollisionSet &collisions, CollisionObject *obj)
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
		Bucket *tester = layers[l].get(x, y, z);
		if (tester != NULL) {
			tester->getObjectCollisions(collisions, obj);
		}
	}
}