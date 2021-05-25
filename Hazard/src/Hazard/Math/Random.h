#pragma once


namespace Hazard 
{
	class Random {
	public:
		static float Range(float min, float max);
	private:
		static std::mt19937_64& Random::Engine() {
			static std::random_device rd;
			static std::mt19937_64 engine(rd());
			return engine;
		}
	};
}