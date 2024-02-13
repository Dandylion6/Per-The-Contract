#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

/// <summary>
/// An interface for anything that can be rendered.
/// render() will be called by renderer class.
/// </summary>
class Renderable
{
public: virtual void render(sf::RenderTarget& target) = 0;
};

