#pragma once

#include <random>

class Random 
{
public:

	static float Range(float min, float max)
	{
		std::uniform_real_distribution<float> distrib(min, max);
		auto& mt = Engine();
		return distrib(mt);
	}

private:
	static std::mt19937_64& Engine() 
	{
		static std::random_device rd;
		static std::mt19937_64 engine(rd());
		return engine;
	}
};
