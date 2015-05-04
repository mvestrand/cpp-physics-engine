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

class SpatialMultiRes3d
{
	class Cells {
	public:
		unsigned int layer;
		std::vector<vec3> t;
	};
	typedef std::unordered_map<CollisionTester*, vec3> TesterMap;
	typedef std::unordered_map<CollisionObject*, Cells> CellMap;
	typedef PointerGrid3d<CollisionTester*> GridLayer;

	void updateObjects();
	void addLayers(int layer);
	int getObjectLayerIndex(CollisionObject *object);
	CollisionTester *addTester(int x, int y, int z, int layer);
	void removeTester(int x, int y, int z, int layer, CollisionTester *tester);
	// Comp1 - in v1 not v2 //Comp2 - in v2 not v1
	void getComplements(std::vector<vec3> &v1, std::vector<vec3> &v2, 
		std::vector<vec3> &comp1, std::vector<vec3> &comp2);
	void getIntersectingCells(vec3 cell, int layer, std::unordered_set<vec4> &intersecting);
	
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
