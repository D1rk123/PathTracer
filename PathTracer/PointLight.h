#pragma once

#include <glm/glm.hpp>

struct PointLight
{
	const glm::vec3 pos;
	const glm::vec3 color;
	const float power;

	PointLight(const glm::vec3 pos, const glm::vec3 color, const float power)
		: pos(pos), color(color), power(power)
	{
	}
};