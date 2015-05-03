#pragma once

#include <unordered_set>
#include <unordered_map>

#include "CollisionObject.h"
#include "CollisionPair.h"
#include "CollisionTester.h"
#include "PointerGrid3d.h"

#define LAYER_GROWTH 2
#define LAYER_OVERFLOW_SIZE 0.5

class SpatialMultiRes3d
{
	class Cells {
	public:
		unsigned int layer;
		std::vector<vec3> t;
	};
	typedef std::unordered_set<CollisionTester*> TesterSet;
	typedef std::unordered_map<CollisionObject*, Cells> CellMap;
	typedef PointerGrid3d<CollisionTester*> GridLayer;

	void updateObjects();
	void addLayers(int layer);
	int getObjectsLayer(CollisionObject *object);

	// Comp1 - in v1 not v2 //Comp2 - in v2 not v1
	void getComplements(std::vector<vec3> &v1, std::vector<vec3> &v2, 
		std::vector<vec3> &comp1, std::vector<vec3> &comp2);
public:

	SpatialMultiRes3d();
	~SpatialMultiRes3d();

	void getCollisions(std::unordered_set<CollisionPair> &collisions);
	void getCollisionsWithObject(std::unordered_set<CollisionPair> &collisions, CollisionObject *object);

	void addObject(CollisionObject *obj);
	void removeObject(CollisionObject *obj);

	// Update an object after it has been moved or resized
	void updateObject(CollisionObject *obj);

	std::vector<GridLayer> layers;
	std::vector<TesterSet> active;
	CellMap objects;
	std::vector<vec3> cell_sizes;
};
