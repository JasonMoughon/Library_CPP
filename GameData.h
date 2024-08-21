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
	std::unordered_map<int, T> ItemList;

	GameData()
		: Space2D(Quad<T>())
		, ItemList(std::vector<std::pair<int, T>>())
	{}

	GameData(Point2D topLeft, Point2D bottomRight)
		: Space2D(Quad<T>(topLeft, bottomRight))
		, ItemList(std::unordered_map<int, T>())
	{}

	void Insert(T val)
	{
		ItemList.insert_or_assign(val.ID, val);
		Space2D.Insert(val);
	}

	bool Delete(T val)
	{
		ItemList.erase(val.ID);
		return Space2D.Delete(val);
	}

	T GetItem(int id)
	{
		return ItemList[id];
	}
};