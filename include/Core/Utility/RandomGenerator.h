#pragma once

#include <random>

namespace utils {
	class Random
	{
	public:
		// Functions
		static int generateInt(int min, int max);
		static int randomIndex(int size);
		static int randomIndex(size_t size);
		static float generateFloat(float min, float max);

	private:
		// Variables
		static std::mt19937 rng;
	};
}
