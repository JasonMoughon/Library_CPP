#pragma once
#include "Quad.h"

template <typename T>
struct GameData
{
	static_assert(std::is_base_of<IMoveable, T>::value, "T must implement IMoveable");
/// <summary>
/// Structure for all common game data passed between functions in MAIN.CPP
/// </summary>
public:
	Quad<T> Space2D;

	GameData()
		: Space2D(Quad<T>())
	{}

	GameData(Point2D topLeft, Point2D bottomRight)
		: Space2D(Quad<T>(topLeft, bottomRight))
	{}
};