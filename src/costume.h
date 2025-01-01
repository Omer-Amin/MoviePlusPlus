#pragma once

#include <vector>
#include "vec2.h"

typedef std::vector<Vec2> shape;

struct Costume
{
	static inline shape rectangle(float x, float y, float width, float height)
	{
		const float halfWidth = width / 2.0f;
		const float halfHeight = height / 2.0f;
		return {
			Vec2(x - halfWidth, y - halfHeight),
			Vec2(x + halfWidth, y - halfHeight),
			Vec2(x + halfWidth, y + halfHeight),
			Vec2(x - halfWidth, y + halfHeight)
		};
	}
};
