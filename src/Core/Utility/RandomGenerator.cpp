#include <random>

#include "Core/Utility/RandomGenerator.h"

namespace utils {
	std::mt19937 RandomGenerator::rng = std::mt19937(std::random_device{}());

	int RandomGenerator::generateInt(int min, int max) {
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(rng);
	}

	int RandomGenerator::randomIndex(int size) {
		std::uniform_int_distribution<int> distribution(0, size - 1);
		return distribution(rng);
	}

	float RandomGenerator::generateFloat(float min, float max) {
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(rng);
	}
}
