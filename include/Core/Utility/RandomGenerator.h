#pragma once

#include <random>

#include "Core/Utility/Vector2.h"

namespace utils {
	class Random
	{
	public:
		// Functions
		static int generateInt(int min, int max);
		static int randomIndex(int size);
		static int randomIndex(size_t size);
		static Vector2 randomRadius(float radius);
		static float generateFloat(float min, float max);

	private:
		// Variables
		static std::mt19937 rng;
	};
}
