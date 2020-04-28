#pragma once

#include "IntersectionResult.h"
#include "Ray.h"

class SceneObject
{
public:
	virtual IntersectionResult testIntersection(const Ray ray) = 0;
};