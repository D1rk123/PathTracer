#pragma once

#include "IntersectionResult.h"
#include "Ray.h"
#include "MaterialType.h"

class SceneObject
{
public:
	MaterialType materialType;

	SceneObject(MaterialType materialType)
		: materialType(materialType)
	{

	}
	virtual IntersectionResult testIntersection(const Ray& ray) = 0;
};