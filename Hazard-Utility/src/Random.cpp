
#include "Random.h"
#include <random>

float Random::Range(float min, float max)
{
	std::uniform_real_distribution<float> distrib(min, max);
	auto& mt = Engine();
	return distrib(mt);
}
