#pragma once

#include <vector>

#include "PhysicsMath.h"
#include "ForceGenerator.h"

class RigidBody
{
	struct RegisteredForceGenerator {
		bool should_delete;
		ForceGenerator *generator;
	};

public:
	real inverseMass;

	mat3 inverseInertiaTensor;

	real linearDamping;
	real angularDamping;

	vec3 gravity;
	vec3 position;
	vec3 velocity;
	quat orientation;
	vec3 rotation;

	vec3 forceAccumulator;  // in world coordinates
	vec3 torqueAccumulator; // in world coordinates

	//Derived
	mat3 inverseInertiaTensorWorld;
	mat4 worldToLocal;
	mat4 localToWorld;

	std::vector<RegisteredForceGenerator> generators;

	void setInertiaTensor(const mat3 &inertia);

	void addForce(const vec3 &force);
	void addForceAtPoint(const vec3 &force, const vec3 &point);
	void addForceAtBodyPoint(const vec3 &force, const vec3 &point);

	void clearAccumulators();
	void updateForces(real duration);
	void integrate(real duration);


	void addForceGenerator(ForceGenerator *generator);
	void removeForceGenerator(ForceGenerator *generator);
	void clearForceGenerators();

	void calculateDerivedValues();
};

