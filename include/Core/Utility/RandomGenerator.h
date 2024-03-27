#pragma once

#include <random>

namespace utils {
	class RandomGenerator
	{
	public:
		// Functions
		static int generateInt(int min, int max);
		static int randomIndex(int size);
		static float generateFloat(float min, float max);

	private:
		// Variables
		static std::mt19937 rng;
	};
}
