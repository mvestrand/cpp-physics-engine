#include "stdafx.h"
#include "RigidBody.h"

/** Set the inertia tensor.
 * @param inertia - The inertia tensor of the object.
 */
void RigidBody::setInertiaTensor(const mat3 &inertia)
{
	inverseInertiaTensor = glm::inverse(inertia);
}

/** Applies a force to the object at the center of mass, ignoring rotation.
 * @param force - The force to apply. In world space.
 */
void RigidBody::addForce(const vec3 &force)
{
	forceAccumulator += force;
}

/** Applies a force at a point in world space.
* @param force - The force to add. In world space.
* @param point - The point to apply the force at. In world space.
*/
void RigidBody::addForceAtPoint(const vec3 &force, const vec3 &point)
{
	torqueAccumulator += glm::cross(point - position, force);
	forceAccumulator += force;
}


/** Applies a force at a point in local space.
 * @param force - The force to add. In world space.
 * @param point - The point to apply the force at. In local space.
 */
void RigidBody::addForceAtBodyPoint(const vec3 &force, const vec3 &point)
{
	vec4 p = localToWorld * vec4(point.x, point.y, point.z, (real)1);
	addForceAtPoint(force, vec3(p.x, p.y, p.z));
}

/** Clears the force and torque accumulators.
 */
void RigidBody::clearAccumulators()
{
	forceAccumulator.x = (real)0;
	forceAccumulator.y = (real)0;
	forceAccumulator.z = (real)0;

	torqueAccumulator.x = (real)0;
	torqueAccumulator.y = (real)0;
	torqueAccumulator.z = (real)0;
}

/** Accumulates the forces from the attached force generators
 * @param duration - The time step.
 */
void RigidBody::updateForces(real duration)
{
	calculateDerivedValues();

	for (auto it = generators.begin(); it != generators.end(); it++) {
		if (it->should_delete) {
			it = generators.erase(it);
			continue;
		}
		it->generator->updateForce(this, duration);
	}
}

void RigidBody::integrate(real duration)
{
	vec3 acceleration = inverseMass * forceAccumulator;
	vec3 angularAcceleration = inverseInertiaTensorWorld * torqueAccumulator;

	velocity += duration * acceleration;
	rotation += duration * angularAcceleration;

	velocity *= (real)pow(linearDamping, duration);
	rotation *= (real)pow(angularDamping, duration);

	position += duration * velocity;
	quatAddVector(orientation, duration * rotation);

	clearAccumulators();
}

void RigidBody::addForceGenerator(ForceGenerator *generator)
{
	RegisteredForceGenerator entry;
	entry.should_delete = false;
	entry.generator = generator;
	generators.push_back(entry);
}

void RigidBody::removeForceGenerator(ForceGenerator *generator)
{
	for (auto it = generators.begin(); it != generators.end(); it++) {
		if (it->generator == generator) {
			it->should_delete = true;
		}
	}
}

void RigidBody::clearForceGenerators()
{
	for (auto gen : generators) {
		gen.should_delete = true;
	}
}

void RigidBody::calculateDerivedValues()
{
	
}