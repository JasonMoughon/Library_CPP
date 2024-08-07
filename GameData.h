#pragma once
#include "Quad.h"
#include "GlobalPositions.h"

struct GameData
{
/// <summary>
/// Structure for all common game data passed between functions in MAIN.CPP
/// </summary>
public:
	Quad GameSpace;

	GameData()
		: GameSpace(Quad())
	{}

	GameData(Point2D topLeft, Point2D bottomRight)
		: GameSpace(Quad(topLeft, bottomRight))
	{}
};