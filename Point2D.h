#pragma once

//2-Dimensional point on a plane
class Point2D
{
public:
	float X = 0;
	float Y = 0;

	Point2D()
	{}
	Point2D(float X, float Y)
		: X(X)
		, Y(Y)
	{}

	void MoveUp()
	{
		Y += 1;
	}
	void MoveDown()
	{
		Y -= 1;
	}
	void MoveLeft()
	{
		X -= 1;
	}
	void MoveRight()
	{
		X += 1;
	}
	bool Equals(Point2D& refPoint)
	{
		if (X == refPoint.X && Y == refPoint.Y) return true;
		return false;
	}
};

bool operator==(const Point2D& p1, const Point2D& p2)
{
	if (p1.X == p2.X && p1.Y == p2.Y) return true;
	return false;
}