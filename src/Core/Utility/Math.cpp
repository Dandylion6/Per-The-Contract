#include <stdint.h>
#include <cmath>

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
	float start_float = static_cast<float>(start);
	float end_float = static_cast<float>(end);
	return static_cast<uint16_t>(start_float + (end_float - start_float) * t);
}

float utils::outQuad(const float start, const float end, float t) {
	return start + (end - start) * (1.f - (1.f - t) * (1.f - t));
}

float utils::outExpo(const float start, const float end, float t) {
	return (t == 1.f) ? end : start + (end - start) * (-powf(2.f, -10.f * t) + 1.f);
}
