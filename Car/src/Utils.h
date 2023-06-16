#pragma once

#include <vectors.h>

constexpr int gridUnit = 20;

template <typename T>
Vector2d<float> toVecFloat(Vector2d<T> a)
{
	return Vector2d<float>(a.x, a.y);
}

