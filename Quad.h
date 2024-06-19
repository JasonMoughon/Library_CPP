#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <Windows.h>
#include "Point2D.h"
#define NULL 0

class Quad
{
public:
	Quad* TopLeftNode = NULL;
	Quad* TopRightNode = NULL;
	Quad* BottomLeftNode = NULL;
	Quad* BottomRightNode = NULL;
		
	Quad()
		: _topLeft(0, 0)
		, _bottomRight(0, 0)
	{};

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
	};

	~Quad()
	{
		if (TopLeftNode != NULL) delete TopLeftNode;
		if (TopRightNode != NULL) delete TopRightNode;
		if (BottomLeftNode != NULL) delete BottomLeftNode;
		if (BottomRightNode != NULL) delete BottomRightNode;
		if (_itemList != NULL) delete _itemList;
	}

	void CleanUp()
	{
		//Clean up all quadrants if they are initialized
		if (TopLeftNode != NULL) TopLeftNode->CleanUp();
		if (TopRightNode != NULL) TopRightNode->CleanUp();
		if (BottomLeftNode != NULL) BottomLeftNode->CleanUp();
		if (BottomRightNode != NULL) BottomRightNode->CleanUp();

		if (this->_hasChild)
		{
			//Check if children are empty
			int emptyCount = 0;
			if (TopLeftNode != NULL)
			{
				if (TopLeftNode->_itemList->size() == 0 && !TopLeftNode->_hasChild)
				{
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

	void Insert(int id, Point2D position)
	{
		//Make sure within bounds of Parent Quad
		if (!WithinBounds(position)) { return; }

		//Insert into ItemList if ID does not exist in list already
		if (_itemList->size() < 4 && !_hasChild)
		{
			if (std::find(_itemList->begin(), _itemList->end(), std::pair<int, Point2D>(id, position)) != _itemList->end()) return;
			_itemList->insert(_itemList->end(), std::pair<int, Point2D>(id, position));
			std::cout << "ITEMLIST UPDATED\n";
		}
		else
		{
			//Push values to _itemList in respective Quad
			for (int i = 0; i < _itemList->size(); i++) InsertIntoChild(_itemList->at(i).first, _itemList->at(i).second);
			_itemList->clear();

			InsertIntoChild(id, position);
		}
	}

	bool Contains(int id, Point2D position)
	{
		if (this->_hasChild)
		{
			//Indicates Top Right or Quadrant 1
			if (position.X > (_bottomRight.X - _topLeft.X) / 2 && position.Y <= (_bottomRight.Y - _topLeft.Y) / 2)
			{
				return this->TopRightNode->Contains(id, position);
			}

			//Indicates Top Left or Quadrant 2
			if (position.X <= (_bottomRight.X - _topLeft.X) / 2 && position.Y < (_bottomRight.Y - _topLeft.Y) / 2)
			{
				return this->TopLeftNode->Contains(id, position);
			}

			//Indicates Bottom Left or Quadrant 3
			if (position.X < (_bottomRight.X - _topLeft.X) / 2 && position.Y >= (_bottomRight.Y - _topLeft.Y) / 2)
			{
				return this->BottomLeftNode->Contains(id, position);
			}

			//Indicates Bottom Right or Quadrant 4
			if (position.X >= (_bottomRight.X - _topLeft.X) / 2 && position.Y > (_bottomRight.Y - _topLeft.Y) / 2)
			{
				return this->BottomRightNode->Contains(id, position);
			}
		}
		else
			return (std::find(_itemList->begin(), _itemList->end(), std::pair<int, Point2D>(id, position)) != _itemList->end());
	}

	void Delete(int id, Point2D position)
	{
		if (this->_hasChild)
		{
			//Indicates Top Right or Quadrant 1
			if (position.X > (_bottomRight.X - _topLeft.X) / 2 && position.Y <= (_bottomRight.Y - _topLeft.Y) / 2)
			{
				this->TopRightNode->Delete(id, position);
			}

			//Indicates Top Left or Quadrant 2
			if (position.X <= (_bottomRight.X - _topLeft.X) / 2 && position.Y < (_bottomRight.Y - _topLeft.Y) / 2)
			{
				this->TopLeftNode->Delete(id, position);
			}

			//Indicates Bottom Left or Quadrant 3
			if (position.X < (_bottomRight.X - _topLeft.X) / 2 && position.Y >= (_bottomRight.Y - _topLeft.Y) / 2)
			{
				this->BottomLeftNode->Delete(id, position);
			}

			//Indicates Bottom Right or Quadrant 4
			if (position.X >= (_bottomRight.X - _topLeft.X) / 2 && position.Y > (_bottomRight.Y - _topLeft.Y) / 2)
			{
				this->BottomRightNode->Delete(id, position);
			}
		}
		else
		{
			auto it = std::find(_itemList->begin(), _itemList->end(),
				std::pair<int, Point2D>(id, position));

			// If element is found found, erase it 
			if (it != _itemList->end()) {
				_itemList->erase(it);
			}
		}
	}

	const std::vector<std::pair<int, Point2D>>* GetItems(int id, Point2D position)
	{
		if (this->_hasChild)
		{
			//Indicates Top Right or Quadrant 1
			if (position.X > (_bottomRight.X - _topLeft.X) / 2 && position.Y <= (_bottomRight.Y - _topLeft.Y) / 2)
			{
				return this->TopRightNode->GetItems(id, position);
			}

			//Indicates Top Left or Quadrant 2
			if (position.X <= (_bottomRight.X - _topLeft.X) / 2 && position.Y < (_bottomRight.Y - _topLeft.Y) / 2)
			{
				return this->TopLeftNode->GetItems(id, position);
			}

			//Indicates Bottom Left or Quadrant 3
			if (position.X < (_bottomRight.X - _topLeft.X) / 2 && position.Y >= (_bottomRight.Y - _topLeft.Y) / 2)
			{
				return this->BottomLeftNode->GetItems(id, position);
			}

			//Indicates Bottom Right or Quadrant 4
			if (position.X >= (_bottomRight.X - _topLeft.X) / 2 && position.Y > (_bottomRight.Y - _topLeft.Y) / 2)
			{
				return this->BottomRightNode->GetItems(id, position);
			}
		}
		else
		{
			if (std::find(_itemList->begin(), _itemList->end(), std::pair<int, Point2D>(id, position)) != _itemList->end()) return _itemList;
		}

		//Return Empty Map if ID is not found
		return new std::vector<std::pair<int, Point2D>>();
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
		if (this->_topLeft.Equals(q->_topLeft) && this->_bottomRight.Equals(q->_bottomRight))
		{
			if (!RecurseQuads(q)) return false;
		}
		return true;
	}

private:
	Point2D _topLeft;
	Point2D _bottomRight;
	std::vector<std::pair<int, Point2D>>* _itemList = new std::vector<std::pair<int, Point2D>>();
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
	
	void InsertIntoChild(int id, Point2D position)
	{
		//Indicates Top Right or Quadrant 1
		if (position.X > (_bottomRight.X - _topLeft.X) / 2 && position.Y <= (_bottomRight.Y - _topLeft.Y) / 2)
		{
			if (TopRightNode == NULL)
			{
				this->_hasChild = true;
				this->TopRightNode = new Quad(Point2D((_bottomRight.X - _topLeft.X) / 2, _topLeft.Y), Point2D(_bottomRight.X, (_bottomRight.Y - _topLeft.Y) / 2));
			}
			this->TopRightNode->Insert(id, position);
		}

		//Indicates Top Left or Quadrant 2
		if (position.X <= (_bottomRight.X - _topLeft.X) / 2 && position.Y < (_bottomRight.Y - _topLeft.Y) / 2)
		{
			if (TopLeftNode == NULL)
			{
				this->_hasChild = true;
				this->TopLeftNode = new Quad(_topLeft, Point2D((_bottomRight.X - _topLeft.X) / 2, (_bottomRight.Y - _topLeft.Y) / 2));
			}
			this->TopLeftNode->Insert(id, position);
		}

		//Indicates Bottom Left or Quadrant 3
		if (position.X < (_bottomRight.X - _topLeft.X) / 2 && position.Y >= (_bottomRight.Y - _topLeft.Y) / 2)
		{
			if (BottomLeftNode == NULL)
			{
				this->_hasChild = true;
				this->BottomLeftNode = new Quad(Point2D(_topLeft.X, (_bottomRight.Y - _topLeft.Y) / 2), Point2D((_bottomRight.X - _topLeft.X) / 2, _bottomRight.Y));
			}
			this->BottomLeftNode->Insert(id, position);
		}

		//Indicates Bottom Right or Quadrant 4
		if (position.X >= (_bottomRight.X - _topLeft.X) / 2 && position.Y > (_bottomRight.Y - _topLeft.Y) / 2)
		{
			if (BottomRightNode == NULL)
			{
				std::cout << "\nInitiating Quad 4\n";
				this->_hasChild - true;
				this->BottomRightNode = new Quad(Point2D((_bottomRight.X - _topLeft.X) / 2, (_bottomRight.Y - _topLeft.Y) / 2), _bottomRight);
			}
			std::cout << "\nInserting to Quad 4\n";
			this->BottomRightNode->Insert(id, position);
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
