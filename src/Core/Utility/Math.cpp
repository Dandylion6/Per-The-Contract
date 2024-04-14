#include <stdint.h>

#include "Core/Utility/Math.h"

float utils::clamp01(const float value) {
	return clamp(value, 0.f, 1.f);
}

float utils::clamp(
	const float value, const float min, const float max
) {
	return value > max ? max : (value < min ? min : value);
}

float utils::clamp(const uint16_t value, const uint16_t min, const uint16_t max) {
	return value > max ? max : (value < min ? min : value);
}

float utils::lerp(const float start, const float end, float t) {
	return start + (end - start) * t;
}

uint16_t utils::lerp(const uint16_t start, const uint16_t end, float t) {
	return static_cast<uint16_t>(start + (end - start) * t);
}

float utils::outQuad(const float start, const float end, float t) {
	return start + (end - start) * (1.f - (1.f - t) * (1.f - t));
}
