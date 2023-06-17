#include <raylib.h>

#include "Particle.h"

float gravity = 9.2; // m/s
float restitutionDistribution = .4f;

Particle::Particle(Vector3d<float> position, Vector3d<float> velocity, float timeToLive)
	: position(position)
	, velocity(velocity)
	, timeToLive(timeToLive)
{
	restitution = 1.f - (float(GetRandomValue(0, 100)) / 100.f)
		* restitutionDistribution;
}

bool
Particle::update(float deltaTime)
{
	velocity.z -= deltaTime * gravity;
	position += velocity * deltaTime;
	if (position.z < 0)
	{
		velocity.z = -velocity.z * restitution;
	}
	timeToLive -= deltaTime;

	return timeToLive > .0f;
}
