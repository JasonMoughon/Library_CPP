#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Quad.h"
#include "Point2D.h"
#include "GameObject.h"
#include "GameData.h"
#define SPEED 10.0f
#define DELTA_TIME 0.166

void SeedQuad(GameData<GameObject>* d, std::vector<std::pair<int, Point2D>>* a)
{
	if (d == NULL || a == NULL) return;
	//Insert all desired values into the quad
	for (int i = 0; i < a->size(); i++)
	{
		d->Space2D.Insert(GameObject(a->at(i).first, a->at(i).second, sf::Vector2f(SPEED, SPEED)));
	}
}

void EmptyQuad(GameData<GameObject>* d, std::vector<std::pair<int, Point2D>>* a)
{
	if (d == NULL || a == NULL) return;
	//Delete all desired values from the quad
	for (int i = 0; i < a->size(); i++)
	{
		d->Space2D.Delete(GameObject(a->at(i).first, a->at(i).second, sf::Vector2f(SPEED, SPEED)));
	}
}

void UpdatePosition(int i, GameData<GameObject>* d)
{
	bool isColliding = false;
	GameObject newPosition = d->Space2D.Find(i);
	//sf::Vector2f newVelocity = d->GamePositions.GetVelocity(i);
	
	//Delete from Quad
	d->Space2D.Delete(newPosition);

	//Calculate new position
	newPosition.SetPosition(Point2D(
		newPosition.GetPosition().X + newPosition.GetVelocity().x * DELTA_TIME ,
		newPosition.GetPosition().Y + newPosition.GetVelocity().y * DELTA_TIME));

	//Check for collision
	if (newPosition.GetPosition().Y + newPosition.GetPosition().CircleBounds.getRadius() >= d->Space2D.GetBottomRight().Y)
	{
		//Collided with bottom of quad
		isColliding = true;
		newPosition.SetVelocity(sf::Vector2f(newPosition.GetVelocity().x, -newPosition.GetVelocity().y));
	}
	if (newPosition.GetPosition().Y - newPosition.GetPosition().CircleBounds.getRadius() <= d->Space2D.GetTopLeft().Y)
	{
		//Collided with top of quad
		isColliding = true;
		newPosition.SetVelocity(sf::Vector2f(newPosition.GetVelocity().x, -newPosition.GetVelocity().y));
	}
	if (newPosition.GetPosition().X + newPosition.GetPosition().CircleBounds.getRadius() >= d->Space2D.GetBottomRight().X)
	{
		//Collided with right of quad
		isColliding = true;
		newPosition.SetVelocity(sf::Vector2f(-newPosition.GetVelocity().x, newPosition.GetVelocity().y));
	}
	if (newPosition.GetPosition().X - newPosition.GetPosition().CircleBounds.getRadius() <= d->Space2D.GetTopLeft().X)
	{
		//Collided with left of quad
		isColliding = true;
		newPosition.SetVelocity(sf::Vector2f(-newPosition.GetVelocity().x, newPosition.GetVelocity().y));
	}

	//Insert new position into quad
	d->Space2D.Insert(newPosition);
}

void SFML(GameData<GameObject>* d, std::vector<std::pair<int, Point2D>> v)
{
	sf::RenderWindow window(sf::VideoMode((int)d->Space2D.GetSize().X + 5, (int)d->Space2D.GetSize().Y + 5), "Quad Demo");

	while (window.isOpen())
	{
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
	}
}

int main()
{
	std::vector<std::pair<int, Point2D>> FirstRound = { {0, Point2D(100,100)},{1, Point2D(300,100)},{2, Point2D(100,900)},{3, Point2D(700,800)},{4, Point2D(900,500)}
													   ,{5, Point2D(300,300)},{6, Point2D(100,300)},{7, Point2D(900,100)},{8, Point2D(800,700)},{9, Point2D(500,900)}
													   ,{10, Point2D(500,300)},{11, Point2D(800,100)},{12, Point2D(900,300)},{13, Point2D(200,200)},{14, Point2D(600,400)}
													   ,{15, Point2D(627, 444)},{16, Point2D(555,555)},{17, Point2D(10, 4)},{18, Point2D(10, 900)}, {19, Point2D(900, 10)} };
	
	GameData<GameObject> Data = GameData<GameObject>(Point2D(0, 0), Point2D(1000, 1000));
	SeedQuad(&Data, &FirstRound);

	SFML(&Data, FirstRound);

	return 0;
}
