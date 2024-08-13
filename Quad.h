#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <type_traits>
#include "Point2D.h"
#include "IMoveable.h"
#define NULL 0
#define OUTLINE_THICKNESS 2
#define ID_UNDEFINED -1

template <typename T>
class Quad
{
	static_assert(std::is_base_of<IMoveable, T>::value, "T must implement IMoveable");
public:
	Quad* TopLeftNode = NULL;
	Quad* TopRightNode = NULL;
	Quad* BottomLeftNode = NULL;
	Quad* BottomRightNode = NULL;
		
	Quad()
		: _topLeft(0, 0)
		, _bottomRight(0, 0)
	{
		_rectangleBounds = sf::RectangleShape(sf::Vector2f(_bottomRight.X - _topLeft.X, _bottomRight.Y - _topLeft.Y));
		_rectangleBounds.setFillColor(sf::Color::Transparent);
		_rectangleBounds.setOutlineColor(sf::Color::Cyan);
		_rectangleBounds.setOutlineThickness(OUTLINE_THICKNESS);
	};

	Quad(Point2D topLeft, Point2D bottomRight)
		: _topLeft(topLeft.X, topLeft.Y)
		, _bottomRight(bottomRight.X, bottomRight.Y)
	{
		if (_topLeft.X > _bottomRight.X || _topLeft.Y > _bottomRight.Y)
		{
			throw "INCORRECT COORDINATES";
			(_topLeft.X, _topLeft.Y) = (float)(0, 0);
			(_bottomRight.X, _bottomRight.Y) = (float)(0, 0);
		}

		_rectangleBounds = sf::RectangleShape(sf::Vector2f(_bottomRight.X - _topLeft.X, _bottomRight.Y - _topLeft.Y));
		_rectangleBounds.setFillColor(sf::Color::Transparent);
		_rectangleBounds.setOutlineColor(sf::Color::Cyan);
		_rectangleBounds.setOutlineThickness(OUTLINE_THICKNESS);
		_rectangleBounds.setPosition(sf::Vector2f(_topLeft.X, _topLeft.Y));
	};

	~Quad()
	{
		delete TopLeftNode, TopRightNode, BottomRightNode, BottomLeftNode, _itemList;
	}

	Point2D GetSize()
	{
		return Point2D(_bottomRight.X - _topLeft.X, _bottomRight.Y - _topLeft.Y, sf::Color::Transparent);
	}

	void Insert(T val)
	{
		//Make sure within bounds of Parent Quad
		if (!WithinBounds(val.GetPosition())) { return; }

		//Insert into ItemList if ID does not exist in list already
		if (_itemList->size() < 4 && !_hasChild)
		{
			if (std::find(_itemList->begin(), _itemList->end(), val) != _itemList->end()) return;
			_itemList->insert(_itemList->end(), val);
		}
		else
		{
			//Push values to _itemList in respective Quad
			for (int i = 0; i < _itemList->size(); i++) InsertIntoChild(_itemList->at(i));
			_itemList->clear();

			InsertIntoChild(val);
		}
	}

	T Find(int id)
	{
		if (_hasChild)
		{
			T result;
			if (TopRightNode != NULL) result = TopRightNode->Find(id);
			if (result.ID != ID_UNDEFINED) return result;

			if (TopLeftNode != NULL) result = TopLeftNode->Find(id);
			if (result.ID != ID_UNDEFINED) return result;

			if (BottomLeftNode != NULL) result = BottomLeftNode->Find(id);
			if (result.ID != ID_UNDEFINED) return result;

			if (BottomRightNode != NULL) result = BottomRightNode->Find(id);
			if (result.ID != ID_UNDEFINED) return result;
		}
		else
		{
			for (int i = 0; i < _itemList->size(); i++)
			{
				T currentItem = _itemList->at(i);
				if (currentItem.ID == id) return currentItem;
			}
		}
		return T();
	}

	bool Contains(T val)
	{
		if (_hasChild) 
		{
			if (val.GetPosition().X >= _topLeft.X + (_bottomRight.X - _topLeft.X) / 2)
			{
				//Indicates Quad 1
				if (val.GetPosition().Y <= _topLeft.Y + (_bottomRight.Y - _topLeft.Y) / 2)
				{
					return TopRightNode->Contains(val);
				}
				//Indicates Quad 4
				else
				{
					return BottomRightNode->Contains(val);
				}
			}
			else
			{
				//Indicates Quad 2
				if (val.GetPosition().Y <= _topLeft.Y + (_bottomRight.Y - _topLeft.Y) / 2)
				{
					return TopLeftNode->Contains(val);
				}
				//Indicates Quad 3
				else
				{
					return BottomLeftNode->Contains(val);
				}
			}
		}
		else
		{
			return (std::find(_itemList->begin(), _itemList->end(), val) != _itemList->end());
		}
	}

	bool Delete(T val)
	{
		bool result = false;
		if (_hasChild)
		{
			if (val.GetPosition().X >= _topLeft.X + (_bottomRight.X - _topLeft.X) / 2)
			{
				//Indicates Quad 1
				if (val.GetPosition().Y <= _topLeft.Y + (_bottomRight.Y - _topLeft.Y) / 2)
				{
					if (TopRightNode != NULL) result = TopRightNode->Delete(val);
					CleanUp();
				}
				//Indicates Quad 4
				else
				{
					if (BottomRightNode != NULL) result = BottomRightNode->Delete(val);
					CleanUp();
				}
			}
			else
			{
				//Indicates Quad 2
				if (val.GetPosition().Y <= _topLeft.Y + (_bottomRight.Y - _topLeft.Y) / 2)
				{
					if (TopLeftNode != NULL) result = TopLeftNode->Delete(val);
					CleanUp();
				}
				//Indicates Quad 3
				else
				{
					if (BottomLeftNode != NULL) result = BottomLeftNode->Delete(val);
					CleanUp();
				}
			}
		}
		else
		{
			auto it = std::find(_itemList->begin(), _itemList->end(), val);

			// If element is found, erase it 
			if (it != _itemList->end()) 
			{
				_itemList->erase(it);
				result = true;
			}
		}
		return result;
	}

	const std::vector<T>* GetItems(T val)
	{
		if (_hasChild)
		{
			//Indicates Top Right or Quadrant 1
			if (val.GetPosition().X > (_bottomRight.X - _topLeft.X) / 2 && val.GetPosition().Y <= (_bottomRight.Y - _topLeft.Y) / 2)
			{
				if (TopRightNode != NULL) return TopRightNode->GetItems(val);
				else return NULL;
			}

			//Indicates Top Left or Quadrant 2
			if (val.GetPosition().X <= (_bottomRight.X - _topLeft.X) / 2 && val.GetPosition().Y < (_bottomRight.Y - _topLeft.Y) / 2)
			{
				if (TopLeftNode != NULL) return TopLeftNode->GetItems(val);
				else return NULL;
			}

			//Indicates Bottom Left or Quadrant 3
			if (val.GetPosition().X < (_bottomRight.X - _topLeft.X) / 2 && val.GetPosition().Y >= (_bottomRight.Y - _topLeft.Y) / 2)
			{
				if (BottomLeftNode != NULL) return BottomLeftNode->GetItems(val);
				else return NULL;
			}

			//Indicates Bottom Right or Quadrant 4
			if (val.GetPosition().X >= (_bottomRight.X - _topLeft.X) / 2 && val.GetPosition().Y > (_bottomRight.Y - _topLeft.Y) / 2)
			{
				if (BottomRightNode != NULL) return BottomRightNode->GetItems(val);
				else return NULL;
			}
		}
		else
		{
			if (std::find(_itemList->begin(), _itemList->end(), val) != _itemList->end()) return _itemList;
		}

		//Return Empty Map if ID is not found
		return NULL;
	}

	void Draw(sf::RenderWindow* w)
	{
		//Do nothing if passed NULL pointer
		if (w == NULL) return;

		w->draw(_rectangleBounds);

		if (_itemList->size() > 0)
		{
			for (int i = 0; i < _itemList->size(); i++)
			{
				//Pass RenderWindow to all Points to Draw
				_itemList->at(i).GetPosition().Draw(w);
			}
		}

		if (TopRightNode != NULL) TopRightNode->Draw(w);
		if (TopLeftNode != NULL) TopLeftNode->Draw(w);
		if (BottomLeftNode != NULL) BottomLeftNode->Draw(w);
		if (BottomRightNode != NULL) BottomRightNode->Draw(w);
	}

	void CleanUp()
	{
		if (_hasChild)
		{
			//Check if children are empty
			int emptyCount = 0;
			if (TopLeftNode != NULL)
			{
				if (TopLeftNode->_itemList->size() == 0 && !TopLeftNode->_hasChild)
				{
					delete TopLeftNode->_itemList;
					delete TopLeftNode;
					TopLeftNode = NULL;
					emptyCount += 1;
				}
			}
			else emptyCount += 1;
			if (TopRightNode != NULL)
			{
				if (TopRightNode->_itemList->size() == 0 && !TopRightNode->_hasChild)
				{
					delete TopRightNode->_itemList;
					delete TopRightNode;
					TopRightNode = NULL;
					emptyCount += 1;
				}
			}
			else emptyCount += 1;
			if (BottomLeftNode != NULL)
			{
				if (BottomLeftNode->_itemList->size() == 0 && !BottomLeftNode->_hasChild)
				{
					delete BottomLeftNode->_itemList;
					delete BottomLeftNode;
					BottomLeftNode = NULL;
					emptyCount += 1;
				}
			}
			else emptyCount += 1;
			if (BottomRightNode != NULL)
			{
				if (BottomRightNode->_itemList->size() == 0 && !BottomRightNode->_hasChild)
				{
					delete BottomRightNode->_itemList;
					delete BottomRightNode;
					BottomRightNode = NULL;
					emptyCount += 1;
				}
			}
			else emptyCount += 1;

			//If all four quadrants are empty
			if (emptyCount >= 4) _hasChild = false;
		}
	}

	Point2D GetBottomRight()
	{
		return _bottomRight;
	}

	Point2D GetTopLeft()
	{
		return _topLeft;
	}

	std::string ToString()
	{
		std::string message = "--Top Left: (" + std::to_string(_topLeft.X) + "," + std::to_string(_topLeft.Y) + "), Bottom Right: (" + std::to_string(_bottomRight.X) + "," + std::to_string(_bottomRight.Y) + ") " + StringifyChildren() + "\n";
		//Quadrant 1
		if (TopRightNode != NULL) { message += "--" + TopRightNode->ToStringAddPrefix("--"); }
		//Quadrant 2
		if (TopLeftNode != NULL) { message += "--" + TopLeftNode->ToStringAddPrefix("--"); }
		//Quadrant 3
		if (BottomLeftNode != NULL) { message += "--" + BottomLeftNode->ToStringAddPrefix("--"); }
		//Quadrant 4
		if (BottomRightNode != NULL) { message += "--" + BottomRightNode->ToStringAddPrefix("--"); }

		return message;
	}

	bool operator==(Quad* q)
	{
		//Do not compare NULL pointer
		if (q == NULL) return false;
		
		//Check dimensions
		if (_topLeft.Equals(q->_topLeft) && _bottomRight.Equals(q->_bottomRight))
		{
			if (!RecurseQuads(q)) return false;
		}
		return true;
	}

private:
	Point2D _topLeft;
	Point2D _bottomRight;
	std::vector<T>* _itemList = new std::vector<T>();
	sf::RectangleShape _rectangleBounds;
	bool _hasChild = false;

	std::string StringifyChildren()
	{
		if (_hasChild) return "Has Children.";
		else return "Has " + std::to_string(_itemList->size()) + " Items.";
	}

	std::string ToStringAddPrefix(std::string prefix)
	{
		std::string message = prefix + "Top Left: (" + std::to_string(_topLeft.X) + "," + std::to_string(_topLeft.Y) + "), Bottom Right: (" + std::to_string(_bottomRight.X) + "," + std::to_string(_bottomRight.Y) + ") " + StringifyChildren() + "\n";
		//Quadrant 1
		if (TopRightNode != NULL) { message += prefix + "--" + TopRightNode->ToStringAddPrefix("--"); }
		//Quadrant 2
		if (TopLeftNode != NULL) { message += prefix + "--" + TopLeftNode->ToStringAddPrefix("--"); }
		//Quadrant 3
		if (BottomLeftNode != NULL) { message += prefix + "--" + BottomLeftNode->ToStringAddPrefix("--"); }
		//Quadrant 4
		if (BottomRightNode != NULL) { message += prefix + "--" + BottomRightNode->ToStringAddPrefix("--"); }

		return message;

	}
	
	void InsertIntoChild(T val)
	{
		if (val.GetPosition().X >= _topLeft.X + (_bottomRight.X - _topLeft.X) / 2)
		{
			//Indicates Quad 1
			if (val.GetPosition().Y <= _topLeft.Y + (_bottomRight.Y - _topLeft.Y) / 2)
			{
				if (TopRightNode == NULL)
				{
					_hasChild = true;
					TopRightNode = new Quad(Point2D(_topLeft.X + (_bottomRight.X - _topLeft.X) / 2, _topLeft.Y), Point2D(_bottomRight.X, _topLeft.Y + (_bottomRight.Y - _topLeft.Y) / 2));
				}
				TopRightNode->Insert(val);
			}
			//Indicates Quad 4
			else
			{
				if (BottomRightNode == NULL)
				{
					_hasChild = true;
					BottomRightNode = new Quad(Point2D(_topLeft.X + (_bottomRight.X - _topLeft.X) / 2, _topLeft.Y + (_bottomRight.Y - _topLeft.Y) / 2), _bottomRight);
				}
				BottomRightNode->Insert(val);
			}
		}
		else
		{
			//Indicates Quad 2
			if (val.GetPosition().Y <= _topLeft.Y + (_bottomRight.Y - _topLeft.Y) / 2)
			{
				if (TopLeftNode == NULL)
				{
					_hasChild = true;
					TopLeftNode = new Quad(_topLeft, Point2D(_topLeft.X + (_bottomRight.X - _topLeft.X) / 2, _topLeft.Y + (_bottomRight.Y - _topLeft.Y) / 2));
				}
				TopLeftNode->Insert(val);
			}
			//Indicates Quad 3
			else
			{
				if (BottomLeftNode == NULL)
				{
					_hasChild = true;
					BottomLeftNode = new Quad(Point2D(_topLeft.X, _topLeft.Y + (_bottomRight.Y - _topLeft.Y) / 2), Point2D(_topLeft.X + (_bottomRight.X - _topLeft.X) / 2, _bottomRight.Y));
				}
				BottomLeftNode->Insert(val);
			}
		}
	}

	bool WithinBounds(Point2D position)
	{
		return position.X >= _topLeft.X && position.X <= _bottomRight.X
			&& position.Y >= _topLeft.Y && position.Y <= _bottomRight.Y;
	}

	bool SearchItemList(int id)
	{
		for (int i = 0; i < _itemList->size(); i++) if (_itemList->at(i).first == id) return true;
		return false;
	}

	bool RecurseQuads(Quad* q)
	{
		//Recursively check for differences between quads.
		//Return false if any difference is found.

		//Check Top Right Quad
		if (this->TopRightNode != NULL && q->TopRightNode != NULL)
		{
			if (this->TopRightNode->_hasChild && q->TopRightNode->_hasChild)
			{
				if (!this->TopRightNode->RecurseQuads(q)) return false; //difference found
			}
			else if (this->TopRightNode->_hasChild && !q->TopRightNode->_hasChild || !this->TopRightNode->_hasChild && q->TopRightNode->_hasChild)
			{
				return false; //difference found
			}
			else
			{
				if (this->TopRightNode->_itemList != q->TopRightNode->_itemList) return false; //difference found
			}
		}
		else if (this->TopRightNode == NULL && q->TopRightNode != NULL || this->TopRightNode != NULL && q->TopRightNode == NULL)
			return false; //difference found

		//Check Top Left Quad
		if (this->TopLeftNode != NULL && q->TopLeftNode != NULL)
		{
			if (this->TopLeftNode->_hasChild && q->TopLeftNode->_hasChild)
			{
				if (!this->TopLeftNode->RecurseQuads(q)) return false; //difference found
			}
			else if (this->TopLeftNode->_hasChild && !q->TopLeftNode->_hasChild || !this->TopLeftNode->_hasChild && q->TopLeftNode->_hasChild)
			{
				return false; //difference found
			}
			else
			{
				if (this->TopLeftNode->_itemList != q->TopLeftNode->_itemList) return false; //difference found
			}
		}
		else if (this->TopLeftNode == NULL && q->TopLeftNode != NULL || this->TopLeftNode != NULL && q->TopLeftNode == NULL)
			return false; //difference found

		//Check Bottom Left Quad
		if (this->BottomLeftNode != NULL && q->BottomLeftNode != NULL)
		{
			if (this->BottomLeftNode->_hasChild && q->BottomLeftNode->_hasChild)
			{
				if (!this->BottomLeftNode->RecurseQuads(q)) return false; //difference found
			}
			else if (this->BottomLeftNode->_hasChild && !q->BottomLeftNode->_hasChild || !this->BottomLeftNode->_hasChild && q->BottomLeftNode->_hasChild)
			{
				return false; //difference found
			}
			else
			{
				if (this->BottomLeftNode->_itemList != q->BottomLeftNode->_itemList) return false; //difference found
			}
		}
		else if (this->BottomLeftNode == NULL && q->BottomLeftNode != NULL || this->BottomLeftNode != NULL && q->BottomLeftNode == NULL)
			return false; //difference found

		//Check Bottom Right Quad
		if (this->BottomRightNode != NULL && q->BottomRightNode != NULL)
		{
			if (this->BottomRightNode->_hasChild && q->BottomRightNode->_hasChild)
			{
				if (!this->BottomRightNode->RecurseQuads(q)) return false; //difference found
			}
			else if (this->BottomRightNode->_hasChild && !q->BottomRightNode->_hasChild || !this->BottomRightNode->_hasChild && q->BottomRightNode->_hasChild)
			{
				return false; //difference found
			}
			else
			{
				if (this->BottomRightNode->_itemList != q->BottomRightNode->_itemList) return false; //difference found
			}
		}
		else if (this->BottomRightNode == NULL && q->BottomRightNode != NULL || this->BottomRightNode != NULL && q->BottomRightNode == NULL)
			return false; //difference found

		//No difference was found
		return true;
	}
};
