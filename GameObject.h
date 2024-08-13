#pragma once
#include "IMoveable.h"
#include <SFML/Graphics.hpp>
#include "Point2D.h"
#define ID_UNDEFINED -1

class GameObject : IMoveable
{
public:
	int ID;
	GameObject()
		: _position(Point2D())
		, _velocity(sf::Vector2f())
		, ID(ID_UNDEFINED)
	{}

	GameObject(int id, Point2D p, sf::Vector2f v)
		: _position(p)
		, _velocity(v)
		, ID(id)
	{}

	Point2D GetPosition() const
	{
		return _position;
	}

	void SetPosition(Point2D p)
	{
		_position = p;
	}

	sf::Vector2f GetVelocity() const
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

bool operator==(const GameObject& obj1, const GameObject& obj2)
{
	return (obj1.ID == obj2.ID);
}