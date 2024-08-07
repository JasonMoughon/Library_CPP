#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#define NULL 0
#define CIRCLE_RADIUS 2.0f

//2-Dimensional point on a plane
class Point2D
{
public:
	float X = 0;
	float Y = 0;
	sf::CircleShape CircleBounds;

	Point2D()
	{
		CircleBounds = sf::CircleShape(CIRCLE_RADIUS);
		CircleBounds.setPosition(sf::Vector2f(X - CIRCLE_RADIUS / 2, Y - CIRCLE_RADIUS / 2));
		CircleBounds.setFillColor(sf::Color::Red);
	}

	Point2D(float X, float Y)
		: X(X)
		, Y(Y)
	{
		CircleBounds = sf::CircleShape(CIRCLE_RADIUS);
		CircleBounds.setPosition(sf::Vector2f(X - CIRCLE_RADIUS / 2, Y - CIRCLE_RADIUS / 2));
		CircleBounds.setFillColor(sf::Color::Red);
	}

	void Draw(sf::RenderWindow* w)
	{
		//Do nothing if passed NULL pointer
		if (w == NULL) return;

		w->draw(CircleBounds);
	}

	void Update()
	{
		CircleBounds.setPosition(sf::Vector2f(X - CIRCLE_RADIUS / 2, Y - CIRCLE_RADIUS / 2));
	}

	void MoveUp()
	{
		Y += 1;
		CircleBounds.setPosition(sf::Vector2f(X - CIRCLE_RADIUS / 2, Y - CIRCLE_RADIUS / 2));
	}

	void MoveDown()
	{
		Y -= 1;
		CircleBounds.setPosition(sf::Vector2f(X - CIRCLE_RADIUS / 2, Y - CIRCLE_RADIUS / 2));
	}

	void MoveLeft()
	{
		X -= 1;
		CircleBounds.setPosition(sf::Vector2f(X - CIRCLE_RADIUS / 2, Y - CIRCLE_RADIUS / 2));
	}

	void MoveRight()
	{
		X += 1;
		CircleBounds.setPosition(sf::Vector2f(X - CIRCLE_RADIUS / 2, Y - CIRCLE_RADIUS / 2));
	}

	bool Equals(Point2D& refPoint)
	{
		if (X == refPoint.X && Y == refPoint.Y) return true;
		return false;
	}

	std::string ToString()
	{
		return "{X: " + std::to_string(X) + ", Y: " + std::to_string(Y) + "}";
	}
};

bool operator==(const Point2D& p1, const Point2D& p2)
{
	if (p1.X == p2.X && p1.Y == p2.Y) return true;
	return false;
}