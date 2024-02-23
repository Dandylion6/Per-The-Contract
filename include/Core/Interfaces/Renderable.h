#pragma once

#include <SFML/Graphics/RenderTarget.hpp>

// Forward declerations
class Renderer;

/// <summary>
/// An interface for anything that can be rendered.
/// render() will be called by renderer class.
/// </summary>
class Renderable
{
public:
	// Constructors
	Renderable(Renderer& renderer);
	virtual ~Renderable();

	// Getters
	int getZIndex() const;

	// Setters
	void setZIndex(int z_index);
	
	// Functions
	void pushToFront();
	virtual void render(sf::RenderTarget& target) = 0;

private:
	// Reference
	Renderer& renderer;

	// Variables
	int z_index = 0;
};

