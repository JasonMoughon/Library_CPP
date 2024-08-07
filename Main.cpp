#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Quad.h"
#include "Point2D.h"
#include "GlobalPositions.h"
#include "GameData.h"
#define DELTA_TIME 0.166

void SeedQuad(GameData* d, std::vector<std::pair<int, Point2D>>* a)
{
	if (d == NULL || a == NULL) return;
	//Insert all desired values into the quad
	for (int i = 0; i < a->size(); i++)
	{
		d->GameSpace.Insert(a->at(i).first, a->at(i).second);
		d->GamePositions.AddPoint(a->at(i).second);
	}
}

void EmptyQuad(GameData* d, std::vector<std::pair<int, Point2D>>* a)
{
	if (d == NULL || a == NULL) return;
	//Delete all desired values from the quad
	for (int i = 0; i < a->size(); i++)
	{
		d->GameSpace.Delete(a->at(i).first, a->at(i).second);
		d->GamePositions.DeletePoint(i);
	}
}

void UpdatePosition(int i, GameData* d)
{
	bool isColliding = false;
	Point2D newPosition = d->GamePositions.GetPosition(i);
	sf::Vector2f newVelocity = d->GamePositions.GetVelocity(i);
	
	//Delete from Quad
	d->GameSpace.Delete(i, newPosition);

	//Calculate new position
	newPosition.X += d->GamePositions.GetVelocity(i).x * DELTA_TIME;
	newPosition.Y += d->GamePositions.GetVelocity(i).y * DELTA_TIME;

	//Check for collision
	if (newPosition.Y + newPosition.CircleBounds.getRadius() >= d->GameSpace.GetBottomRight().Y)
	{
		//Collided with bottom of quad
		isColliding = true;
		newVelocity.y = -newVelocity.y;
		d->GamePositions.SetVelocity(i, newVelocity);
	}
	if (newPosition.Y - newPosition.CircleBounds.getRadius() <= d->GameSpace.GetTopLeft().Y)
	{
		//Collided with top of quad
		isColliding = true;
		newVelocity.y = -newVelocity.y;
		d->GamePositions.SetVelocity(i, newVelocity);
	}
	if (newPosition.X + newPosition.CircleBounds.getRadius() >= d->GameSpace.GetBottomRight().X)
	{
		//Collided with right of quad
		isColliding = true;
		newVelocity.x = -newVelocity.x;
		d->GamePositions.SetVelocity(i, newVelocity);
	}
	if (newPosition.X - newPosition.CircleBounds.getRadius() <= d->GameSpace.GetTopLeft().X)
	{
		//Collided with left of quad
		isColliding = true;
		newVelocity.x = -newVelocity.x;
		d->GamePositions.SetVelocity(i, newVelocity);
	}

	//Rewrite position
	if (!isColliding)
	{
		d->GamePositions.SetPosition(i, newPosition);
	}

	//Insert new position into quad
	d->GameSpace.Insert(i, newPosition);
}

int SFML(GameData* d)
{
	sf::RenderWindow window(sf::VideoMode((int)d->GameSpace.GetSize().X + 5, (int)d->GameSpace.GetSize().Y + 5), "Quad Demo");

	while (window.isOpen())
	{
		//Update Assets
		for (int i = 0; i < d->GamePositions.Size(); i++)
		{
			UpdatePosition(i, d);
		}

		//Draw Stuff
		window.clear();
		d->GameSpace.Draw(&window);
		window.display();


		//Close Window Event
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	return 0;
}

int main()
{
	std::vector<std::pair<int, Point2D>> FirstRound = { {0, Point2D(100,100)},{1, Point2D(300,100)},{2, Point2D(100,900)},{3, Point2D(700,800)},{4, Point2D(900,500)}
													   ,{5, Point2D(300,300)},{6, Point2D(100,300)},{7, Point2D(900,100)},{8, Point2D(800,700)},{9, Point2D(500,900)}
													   ,{10, Point2D(500,300)},{11, Point2D(800,100)},{12, Point2D(900,300)},{13, Point2D(200,200)},{14, Point2D(600,400)}
													   ,{15, Point2D(627, 444)},{16, Point2D(555,555)},{17, Point2D(10, 4)},{18, Point2D(10, 900)}, {19, Point2D(900, 10)} };
	GameData Data = GameData(Point2D(0, 0), Point2D(1000, 1000));
	SeedQuad(&Data, &FirstRound);
	std::cout << Data.GameSpace.ToString() + "\n\n";

	return SFML(&Data);

	EmptyQuad(&Data, &FirstRound);
	std::cout << Data.GameSpace.ToString() + "\n\n";
}
