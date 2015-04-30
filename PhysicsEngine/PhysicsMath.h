#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

typedef float real;
using glm::vec3;
using glm::vec4;
using glm::quat;
using glm::mat3;
using glm::mat4;
using glm::mat3x4;


inline void quatAddVector(quat &q, const vec3 &v)
{
	real len = glm::length(v);
	if (len > 0) {
		q = glm::angleAxis(len, v) * q;
	}
}