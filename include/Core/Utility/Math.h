#pragma once

namespace utils {
	float clamp01(const float value);
	float clamp(
		const float value, const float min, const float max
	);

	float lerp(const float start, const float end, float t);
	float outQuad(const float start, const float end, float t);
}