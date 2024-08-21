#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Quad.h"
#include "Point2D.h"
#include "GameObject.h"
#include "GameData.h"
#define SPEED 20.0f
#define COLLISION_BIAS 10.0F
#define DELTA_TIME 0.166f

sf::Vector2f GetDifference(Point2D base, Point2D head)
{
	//Returns the normal Vector between two points
	return sf::Vector2f(head.X - base.X, head.Y - base.Y);
}

float GetAngle(Point2D base, Point2D head)
{
	//Returns the angle, in radians, between the two points
	return std::atan2(head.Y - base.Y, head.X - base.X);
}

float GetLength(sf::Vector2f v) { return std::sqrt(v.x * v.x + v.y * v.y); }

sf::Vector2f Normalize(sf::Vector2f v)
{
	//Returns the normalized vector (magnitude of 1)
	float len = GetLength(v);
	return sf::Vector2f(v.x / len, v.y / len);
}


void SeedQuad(GameData<GameObject>* d, std::vector<std::pair<int, Point2D>>* a)
{
	if (d == NULL || a == NULL) return;
	//Insert all desired values into the quad
	for (int i = 0; i < a->size(); i++)
	{
		d->Insert(GameObject(a->at(i).first, a->at(i).second, sf::Vector2f(SPEED, SPEED)));
		//d->Space2D.Insert(GameObject(a->at(i).first, a->at(i).second, sf::Vector2f(SPEED, SPEED)));
	}
}

void EmptyQuad(GameData<GameObject>* d, std::vector<std::pair<int, Point2D>>* a)
{
	if (d == NULL || a == NULL) return;
	//Delete all desired values from the quad
	for (int i = 0; i < a->size(); i++)
	{
		//GameObject iObject = GameObject(a->at(i).first, a->at(i).second, a->at(i).second.GetVelocity());

		//d->Space2D.Delete(iObject);
		{
			//d->Space2D.Delete(GameObject(iObject.ID, iObject.GetPosition().GetMidRight(), currentPosition.GetVelocity()));
			//d->Space2D.Delete(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidTop(), currentPosition.GetVelocity()));
			//d->Space2D.Delete(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidBottom(), currentPosition.GetVelocity()));
			//d->Space2D.Delete(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidLeft(), currentPosition.GetVelocity()));
		}
	}
}

void aKeyAction(int i, GameData<GameObject>* d)
{
	GameObject currentPosition = d->Space2D.Find(i);
	d->Space2D.Delete(currentPosition);
	currentPosition.SetVelocity(sf::Vector2f(-SPEED, 0));
	d->Space2D.Insert(currentPosition);
}

void sKeyAction(int i, GameData<GameObject>* d)
{
	GameObject currentPosition = d->Space2D.Find(i);
	d->Space2D.Delete(currentPosition);
	currentPosition.SetVelocity(sf::Vector2f(0, SPEED));
	d->Space2D.Insert(currentPosition);
}

void dKeyAction(int i, GameData<GameObject>* d)
{
	GameObject currentPosition = d->Space2D.Find(i);
	d->Space2D.Delete(currentPosition);
	currentPosition.SetVelocity(sf::Vector2f(SPEED, 0));
	d->Space2D.Insert(currentPosition);
}

void wKeyAction(int i, GameData<GameObject>* d)
{
	GameObject currentPosition = d->Space2D.Find(i);
	d->Space2D.Delete(currentPosition);
	currentPosition.SetVelocity(sf::Vector2f(0, -SPEED));
	d->Space2D.Insert(currentPosition);
}

void escKeyAction(sf::RenderWindow* w)
{
	w->close();
}

void leftKeyAction(sf::View* v)
{
	sf::Vector2f previousCenter= v->getCenter();
	v->setCenter(previousCenter.x - 10, previousCenter.y);
}

void rightKeyAction(sf::View* v)
{
	sf::Vector2f previousCenter = v->getCenter();
	v->setCenter(previousCenter.x + 10, previousCenter.y);
}

void UpdatePosition(int i, GameData<GameObject>* d)
{
	bool isColliding = false;
	GameObject currentPosition = d->Space2D.Find(i);
	GameObject newPosition = currentPosition;
	const std::vector<GameObject>* collidables = d->Space2D.GetItems(currentPosition);
	
	//Delete from Quad
	//d->Space2D.Delete(currentPosition);
	d->Delete(currentPosition);

	//Calculate new position
	newPosition.SetPosition(Point2D(
		newPosition.GetPosition().X + newPosition.GetVelocity().x * DELTA_TIME,
		newPosition.GetPosition().Y + newPosition.GetVelocity().y * DELTA_TIME,
		newPosition.GetPosition().CircleBounds.getFillColor()));

	//Check for collision with other objects
	if (collidables != NULL)
	{
		for (int i = 0; i < collidables->size(); i++)
		{
			//Snapshot of current collidable item
			GameObject currentCollidable = collidables->at(i);

			//Don't check for collision with oneself
			if (currentCollidable.ID != currentPosition.ID)
			{

				sf::Vector2f differenceVector = GetDifference(currentPosition.GetPosition(), currentCollidable.GetPosition());
				float radiiSum = currentCollidable.GetPosition().CircleBounds.getRadius() + currentPosition.GetPosition().CircleBounds.getRadius();
				if (GetLength(differenceVector) <= radiiSum + COLLISION_BIAS)
				{
					sf::Vector2f normalDiff = Normalize(differenceVector);
					newPosition.SetVelocity(sf::Vector2f(-(SPEED * normalDiff.x), -(SPEED * normalDiff.y)));
				}
			}
		}
	}

	//Check for collision with exterior window
	if (newPosition.GetPosition().Y + newPosition.GetPosition().CircleBounds.getRadius() >= d->Space2D.GetBottomRight().Y)
	{
		//Collided with bottom of quad
		isColliding = true;
		currentPosition.SetVelocity(sf::Vector2f(newPosition.GetVelocity().x, -newPosition.GetVelocity().y));
	}
	if (newPosition.GetPosition().Y - newPosition.GetPosition().CircleBounds.getRadius() <= d->Space2D.GetTopLeft().Y)
	{
		//Collided with top of quad
		isColliding = true;
		currentPosition.SetVelocity(sf::Vector2f(newPosition.GetVelocity().x, -newPosition.GetVelocity().y));
	}
	if (newPosition.GetPosition().X + newPosition.GetPosition().CircleBounds.getRadius() >= d->Space2D.GetBottomRight().X)
	{
		//Collided with right of quad
		isColliding = true;
		currentPosition.SetVelocity(sf::Vector2f(-newPosition.GetVelocity().x, newPosition.GetVelocity().y));
	}
	if (newPosition.GetPosition().X - newPosition.GetPosition().CircleBounds.getRadius() <= d->Space2D.GetTopLeft().X)
	{
		//Collided with left of quad
		isColliding = true;
		currentPosition.SetVelocity(sf::Vector2f(-newPosition.GetVelocity().x, newPosition.GetVelocity().y));
	}

	//Insert current position back into Quad if motion would collide with border, otherwise insert new position
	if (isColliding)
	{
		d->Insert(currentPosition);
		//d->Space2D.Insert(currentPosition);
	}
	else
	{
		d->Insert(newPosition);
		//d->Space2D.Insert(newPosition);
	}
}

void SFML(GameData<GameObject>* d, std::vector<std::pair<int, Point2D>> v)
{
	sf::RenderWindow window(sf::VideoMode((int)d->Space2D.GetSize().X + 10, (int)d->Space2D.GetSize().Y + 10), "Quad Demo");
	sf::View myView = window.getView();

	unsigned short aState = 0x00, aStateLast = 0x00, dState = 0x00, dStateLast = 0x00, sState = 0x00, sStateLast = 0x00,
		wState = 0x00, wStateLast = 0x00, escState = 0x00, escStateLast = 0x00, leftState = 0x00, leftStateLast = 0x00,
		rightState = 0x00, rightStateLast = 0x00;

	while (window.isOpen())
	{
		//Window view changed
		//GameObject item0 = d->Space2D.Find(0);
		//window.setView(sf::View(sf::Vector2f(item0.GetPosition().X, item0.GetPosition().Y), window.getView().getSize()));

		aState = GetKeyState(0x41); // 'A' key - >= 0xff80 is ON, otherwise OFF
		dState = GetKeyState(0x44); // 'D' key
		sState = GetKeyState(0x53); // 'S' key
		wState = GetKeyState(0x57); // 'W' key
		escState = GetKeyState(VK_ESCAPE); // 'esc' key
		leftState = GetKeyState(VK_LEFT); // 'left arrow' key
		rightState = GetKeyState(VK_RIGHT); // 'right arrow' key

		if (aState >= 0xff80 && aState != aStateLast) aKeyAction(0, d);
		if (dState >= 0xff80 && dState != dStateLast) dKeyAction(0, d);
		if (sState >= 0xff80 && sState != sStateLast) sKeyAction(0, d);
		if (wState >= 0xff80 && wState != wStateLast) wKeyAction(0, d);
		if (escState >= 0xff80 && escState != escStateLast) escKeyAction(&window);
		if (leftState >= 0xff80 && leftState != leftStateLast) leftKeyAction(&myView);
		if (rightState >= 0xff80 && rightState != rightStateLast) rightKeyAction(&myView);

		//Update Assets
		for (int i = 0; i < v.size(); i++)
		{
			UpdatePosition(i, d);
		}

		//Draw Stuff
		window.clear();
		d->Space2D.Draw(&window);
		window.display();


		//Close Window Event
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		aStateLast = aState;
		dStateLast = dState;
		sStateLast = sState;
		wStateLast = wState;
		escStateLast = escState;
		leftStateLast = leftState;
		rightStateLast = rightState;
	}
}

int main()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 5); //Total number of colors listed in COLOR_MAP
	std::uniform_int_distribution<std::mt19937::result_type> dist999(6, 999); //Ensure number distribution stays within bounds of Quad.

	std::unordered_map<int, sf::Color> COLOR_MAP = { {0, sf::Color::Yellow}
												, {1, sf::Color::Red}
												, {2, sf::Color::Green}
												, {3, sf::Color::Blue}
												, {4, sf::Color::Magenta}
												, {5, sf::Color::White} };


	//Ask user for total number of points to insert
	std::cout << std::endl << "Please enter total number of points (1-100) to simulate in Quad" << std::endl;
	int total = 0;
	bool inputInvalid = true;
	
	while (inputInvalid)
	{
		std::cin >> total;
		if (total > 0 && total <= 100) inputInvalid = false;
		else std::cout << "INPUT INVALID. Please enter an integer value between 1-100." << std::endl;
	}


	std::vector<std::pair<int, Point2D>> FirstRound;
	
	for (int i = 0; i < total; i++)
	{
		FirstRound.insert(FirstRound.end(), std::pair<int, Point2D>(i, Point2D((float)dist999(rng), (float)dist999(rng), COLOR_MAP.at(dist6(rng)))));
	}
	
	GameData<GameObject> Data = GameData<GameObject>(Point2D(3, 3, sf::Color::Transparent), Point2D(1000, 1000, sf::Color::Transparent));
	SeedQuad(&Data, &FirstRound);

	Point2D init = Point2D(10, 10);
	Point2D fin = Point2D(1, 5);
	sf::Vector2f diff = GetDifference(init, fin);

	SFML(&Data, FirstRound);

	return 0;
}
