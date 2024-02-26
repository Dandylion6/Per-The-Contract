#include "Core/Utility/Math.h"

float utils::clamp01(const float value) {
	return clamp(value, 0.f, 1.f);
}

float utils::clamp(
	const float value, const float min, const float max
) {
	return value > max ? max : (value < min ? min : value);
}

float utils::lerp(const float start, const float end, float t) {
	return start + (end - start) * t;
}

float utils::outQuad(const float start, const float end, float t) {
	return start + (end - start) * (1.f - (1.f - t) * (1.f - t));
}
