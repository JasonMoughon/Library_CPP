#pragma once
#include <SFML/Graphics.hpp>
#include "Point2D.h"

class IMoveable
{
public:
	virtual Point2D GetPosition() = 0;
	virtual void SetPosition(Point2D p) = 0;
	virtual sf::Vector2f GetVelocity() = 0;
	virtual void SetVelocity(sf::Vector2f v) = 0;
};