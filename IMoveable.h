#pragma once
#include <SFML/Graphics.hpp>
#include "Point2D.h"

class IMoveable
{
public:
	int ID = 0;
	virtual Point2D GetPosition() const = 0;
	virtual void SetPosition(Point2D p) = 0;
	virtual sf::Vector2f GetVelocity() const = 0;
	virtual void SetVelocity(sf::Vector2f v) = 0;
};