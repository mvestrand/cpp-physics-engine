#include "stdafx.h"
#include "SpatialMultiRes3d.h"


SpatialMultiRes3d::SpatialMultiRes3d()
{
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
			tester->removeObject(it->first);
		}
		// Add to cells it has just entered
		for (auto i = new_cells.begin(); i != new_cells.end(); i++) {
			int x = i->x;
			int y = i->y;
			int z = i->z;
			CollisionTester *tester = layers[it->second.layer].get(x, y, z);
			tester->addObject(it->first);
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

int SpatialMultiRes3d::getObjectsLayer(CollisionObject *object)
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