#pragma once

#include <vectors.h>

struct Particle
{
public:
	Particle(Vector3d<float> position, Vector3d<float> velocity, float timeToLive);

	// Returns if more updates are required
	bool update(float deltaTime);
	Vector3d<float> const& getPosition() const { return position; }

private:
	Vector3d<float> position;
	Vector3d<float> velocity;
	float restitution;

	float timeToLive;
};