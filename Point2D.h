#pragma once
#include <iostream>
#include <random>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#define NULL 0
#define CIRCLE_RADIUS 4.0f
#define RAND_MAX 5
#define PI 3.14159f

//2-Dimensional point on a plane
class Point2D
{
public:
	float X = 0;
	float Y = 0;
	sf::CircleShape CircleBounds;

	Point2D()
		: X(0)
		, Y(0)
		, _status(0x00)
	{
		CircleBounds = sf::CircleShape(CIRCLE_RADIUS);
		CircleBounds.setPosition(sf::Vector2f(X - CIRCLE_RADIUS / 2, Y - CIRCLE_RADIUS / 2));
		CircleBounds.setFillColor(sf::Color::Red);
	}

	Point2D(float X, float Y)
		: X(X)
		, Y(Y)
		, _status(0x00)
	{
		CircleBounds = sf::CircleShape(CIRCLE_RADIUS);
		CircleBounds.setPosition(sf::Vector2f(X - CIRCLE_RADIUS / 2, Y - CIRCLE_RADIUS / 2));
		CircleBounds.setFillColor(sf::Color::Transparent);
	}

	Point2D(float X, float Y, sf::Color C)
		: X(X)
		, Y(Y)
		, _status(0x01)
	{
		CircleBounds = sf::CircleShape(CIRCLE_RADIUS);
		CircleBounds.setPosition(sf::Vector2f(X - CIRCLE_RADIUS / 2, Y - CIRCLE_RADIUS / 2));
		CircleBounds.setFillColor(C);
	}

	void Draw(sf::RenderWindow* w)
	{
		//Do nothing if passed NULL pointer
		if (w == NULL) return;

		//Draw only if Draw is Enabled
		if (GetBit(_status, 0))
		{
			w->draw(CircleBounds);
		}
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

	Point2D GetMidRight()
	{
		return SetQuadrant(0);
	}

	Point2D GetTopRight()
	{
		return SetQuadrant(PI / 4);
	}

	Point2D GetMidTop()
	{
		return SetQuadrant(PI / 2);
	}

	Point2D GetTopLeft()
	{
		return SetQuadrant(PI * 3 / 4);
	}

	Point2D GetMidLeft()
	{
		return SetQuadrant(PI);
	}

	Point2D GetBottomLeft()
	{
		return SetQuadrant(PI * 5 / 4);
	}

	Point2D GetMidBottom()
	{
		return SetQuadrant(PI * 3 / 2);
	}

	Point2D GetBottomRight()
	{
		return SetQuadrant(PI * 7 / 4);
	}

private:
	char _status = 0x00; //Byte status: 0-bit=DrawEnabled, bits 1-7 unused

	Point2D SetQuadrant(float radians)
	{
		return Point2D(X + std::cos(radians) * CIRCLE_RADIUS, Y - std::sin(radians) * CIRCLE_RADIUS);
	}

	bool GetBit(char status, unsigned int shift)
	{
		return (status >> shift) & 1;
	}

	void SetBit(char status, unsigned int shift)
	{
		status |= 1 << shift;
	}

	void UnsetBit(char status, unsigned int shift)
	{
		status &= ~(1 << shift);
	}

	void ToggleBit(char status, unsigned int shift)
	{
		status ^= 1 << shift;
	}
};

bool operator==(const Point2D& p1, const Point2D& p2)
{
	if (p1.X == p2.X && p1.Y == p2.Y) return true;
	return false;
}

Point2D operator-(const Point2D& p1, const Point2D& p2)
{
	return Point2D(p1.X - p2.X, p1.Y - p2.Y);
}