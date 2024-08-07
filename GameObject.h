#pragma once
#include "IMoveable.h"
#include <SFML/Graphics.hpp>
#include "Point2D.h"

class GameObject : IMoveable
{
public:
	GameObject()
		: _position(Point2D())
		, _velocity(sf::Vector2f())
	{}

	GameObject(Point2D p, sf::Vector2f v)
		: _position(p)
		, _velocity(v)
	{}

	Point2D GetPosition()
	{
		return _position;
	}

	void SetPosition(Point2D p)
	{
		_position = p;
	}

	sf::Vector2f GetVelocity()
	{
		return _velocity;
	}

	void SetVelocity(sf::Vector2f v)
	{
		_velocity = v;
	}

	//sf::Texture GetTexture() {}

private:
	Point2D _position;
	sf::Vector2f _velocity;
	//sf::Texture _texture;
};