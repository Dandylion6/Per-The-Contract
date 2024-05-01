#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdint>

#include "SFML/Graphics/Color.hpp"

#include "Components/Renderers/OutlineRenderer.h"
#include "Components/Renderers/Renderer.h"
#include "Core/Managers/Game.h"
#include "Core/Object.h"
#include "Core/Utility/Vector2.h"


//_______________
// Constructors

OutlineRenderer::OutlineRenderer(
	Game& game, Object& object, Vector2 size
) :
	Renderer(game, object), size(size)
{
	outline.setFillColor(sf::Color::Transparent);
	outline.setOutlineThickness(thickness);
}

OutlineRenderer::~OutlineRenderer() {
}


//___________________
// Public functions

void OutlineRenderer::update(float delta_time) {
	float sine = sinf(flash_speed * 2.f * M_PI * time);
	uint8_t alpha = (1.5f + sine) * (255.f / 2.5f);
	float sizing = 0.5f + (0.5f * sine);

	outline.setPosition(object.getPosition());
	outline.setOutlineColor(sf::Color(255u, 255u, 255u, alpha));

	Vector2 new_size = size + Vector2::scale(size_offset * sizing);
	outline.setSize(new_size);
	outline.setOrigin(new_size * 0.5f);
	target.draw(outline);

	time += delta_time;
	float period = 2.f / flash_speed;
	if (time >= period) time -= period;
}
