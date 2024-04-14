#include <random>

#include "Core/Utility/RandomGenerator.h"

namespace utils {
	std::mt19937 Random::rng = std::mt19937(std::random_device{}());

	int Random::generateInt(int min, int max) {
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(rng);
	}

	int Random::randomIndex(int size) {
		std::uniform_int_distribution<int> distribution(0, size - 1);
		return distribution(rng);
	}

	int Random::randomIndex(size_t size) {
		std::uniform_int_distribution<int> distribution(
			0, static_cast<int>(size) - 1
		);
		return distribution(rng);
	}

	Vector2 Random::randomRadius(float radius) {
		return Vector2(
			generateFloat(-radius, radius),
			generateFloat(-radius, radius)
		);
	}

	float Random::generateFloat(float min, float max) {
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(rng);
	}
}
