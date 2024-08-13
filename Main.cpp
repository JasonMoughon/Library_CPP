#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Quad.h"
#include "Point2D.h"
#include "GameObject.h"
#include "GameData.h"
#define SPEED 20.0f
#define COLLISION_BIAS 1.0F
#define TOTAL_POINTS 20
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
		d->Space2D.Insert(GameObject(a->at(i).first, a->at(i).second, sf::Vector2f(SPEED, SPEED)));
		//d->Space2D.Insert(GameObject(a->at(i).first, a->at(i).second.GetMidRight(), sf::Vector2f(SPEED, SPEED)));
		//d->Space2D.Insert(GameObject(a->at(i).first, a->at(i).second.GetMidTop(), sf::Vector2f(SPEED, SPEED)));
		//d->Space2D.Insert(GameObject(a->at(i).first, a->at(i).second.GetMidLeft(), sf::Vector2f(SPEED, SPEED)));
		//d->Space2D.Insert(GameObject(a->at(i).first, a->at(i).second.GetMidBottom(), sf::Vector2f(SPEED, SPEED)));
	}
}

void EmptyQuad(GameData<GameObject>* d, std::vector<std::pair<int, Point2D>>* a)
{
	if (d == NULL || a == NULL) return;
	//Delete all desired values from the quad
	for (int i = 0; i < a->size(); i++)
	{
		//GameObject iObject = GameObject(a->at(i).first, a->at(i).second, a->at(i).second.GetVelocity());

		//if (d->Space2D.Delete(iObject))
		{
			//d->Space2D.Delete(GameObject(iObject.ID, iObject.GetPosition().GetMidRight(), currentPosition.GetVelocity()));
			//d->Space2D.Delete(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidTop(), currentPosition.GetVelocity()));
			//d->Space2D.Delete(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidBottom(), currentPosition.GetVelocity()));
			//d->Space2D.Delete(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidLeft(), currentPosition.GetVelocity()));
		}
	}
}

void UpdatePosition(int i, GameData<GameObject>* d)
{
	bool isColliding = false;
	GameObject currentPosition = d->Space2D.Find(i);
	GameObject newPosition = currentPosition;
	const std::vector<GameObject>* collidables = d->Space2D.GetItems(currentPosition);
	
	//Delete from Quad
	d->Space2D.Delete(currentPosition);
	//if ()
	{
		//d->Space2D.Delete(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidRight(), currentPosition.GetVelocity()));
		//d->Space2D.Delete(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidTop(), currentPosition.GetVelocity()));
		//d->Space2D.Delete(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidBottom(), currentPosition.GetVelocity()));
		//d->Space2D.Delete(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidLeft(), currentPosition.GetVelocity()));
	}

	//Calculate new position
	newPosition.SetPosition(Point2D(
		newPosition.GetPosition().X + newPosition.GetVelocity().x * DELTA_TIME ,
		newPosition.GetPosition().Y + newPosition.GetVelocity().y * DELTA_TIME ,
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
				float radiiSum = currentCollidable.GetPosition().CircleBounds.getRadius() + currentPosition.GetPosition().CircleBounds.getRadius() + COLLISION_BIAS;
				if (GetLength(differenceVector) <= radiiSum)
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
		d->Space2D.Insert(currentPosition);
		//d->Space2D.Insert(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidRight(), currentPosition.GetVelocity()));
		//d->Space2D.Insert(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidTop(), currentPosition.GetVelocity()));
		//d->Space2D.Insert(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidBottom(), currentPosition.GetVelocity()));
		//d->Space2D.Insert(GameObject(currentPosition.ID, currentPosition.GetPosition().GetMidLeft(), currentPosition.GetVelocity()));
	}
	else
	{
		d->Space2D.Insert(newPosition);
		//d->Space2D.Insert(GameObject(newPosition.ID, newPosition.GetPosition().GetMidRight(), newPosition.GetVelocity()));
		//d->Space2D.Insert(GameObject(newPosition.ID, newPosition.GetPosition().GetMidTop(), newPosition.GetVelocity()));
		//d->Space2D.Insert(GameObject(newPosition.ID, newPosition.GetPosition().GetMidBottom(), newPosition.GetVelocity()));
		//d->Space2D.Insert(GameObject(newPosition.ID, newPosition.GetPosition().GetMidLeft(), newPosition.GetVelocity()));
	}
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
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 5);
	std::uniform_int_distribution<std::mt19937::result_type> dist999(1, 999);

	std::unordered_map<int, sf::Color> COLOR_MAP = { {0, sf::Color::Yellow}
												, {1, sf::Color::Red}
												, {2, sf::Color::Green}
												, {3, sf::Color::Blue}
												, {4, sf::Color::Magenta}
												, {5, sf::Color::White} };


	std::vector<std::pair<int, Point2D>> FirstRound;
	/*FirstRound = {{0, Point2D(100, 100, COLOR_MAP.at(dist6(rng)))}
													   ,{1, Point2D(300, 100, COLOR_MAP.at(dist6(rng)))}
													   ,{2, Point2D(100, 900, COLOR_MAP.at(dist6(rng)))}
													   ,{3, Point2D(700, 800, COLOR_MAP.at(dist6(rng)))}
													   ,{4, Point2D(900, 500, COLOR_MAP.at(dist6(rng)))}
													   ,{5, Point2D(300, 300, COLOR_MAP.at(dist6(rng)))}
													   ,{6, Point2D(100, 300, COLOR_MAP.at(dist6(rng)))}
													   ,{7, Point2D(900, 100, COLOR_MAP.at(dist6(rng)))}
													   ,{8, Point2D(800, 700, COLOR_MAP.at(dist6(rng)))}
													   ,{9, Point2D(500, 900, COLOR_MAP.at(dist6(rng)))}
													   ,{10, Point2D(500, 300, COLOR_MAP.at(dist6(rng)))}
													   ,{11, Point2D(800, 100, COLOR_MAP.at(dist6(rng)))}
													   ,{12, Point2D(900, 300, COLOR_MAP.at(dist6(rng)))}
													   ,{13, Point2D(200, 200, COLOR_MAP.at(dist6(rng)))}
													   ,{14, Point2D(600, 400, COLOR_MAP.at(dist6(rng)))}
													   ,{15, Point2D(627, 444, COLOR_MAP.at(dist6(rng)))}
													   ,{16, Point2D(555, 555, COLOR_MAP.at(dist6(rng)))}
													   ,{17, Point2D(10, 4, COLOR_MAP.at(dist6(rng)))}
													   ,{18, Point2D(10, 900, COLOR_MAP.at(dist6(rng)))}
													   ,{19, Point2D(900, 10, COLOR_MAP.at(dist6(rng)))} };
													   */
	
	for (int i = 0; i < TOTAL_POINTS; i++)
	{
		FirstRound.insert(FirstRound.end(), std::pair<int, Point2D>(i, Point2D((float)dist999(rng), (float)dist999(rng), COLOR_MAP.at(dist6(rng)))));
	}
	
	GameData<GameObject> Data = GameData<GameObject>(Point2D(0, 0, sf::Color::Transparent), Point2D(1000, 1000, sf::Color::Transparent));
	SeedQuad(&Data, &FirstRound);

	Point2D init = Point2D(10, 10);
	Point2D fin = Point2D(1, 5);
	sf::Vector2f diff = GetDifference(init, fin);
	std::cout << init.ToString() << std::endl << fin.ToString() << std::endl << "{X:" + std::to_string(diff.x) + ", Y:" + std::to_string(diff.y) + "}" << std::endl;

	SFML(&Data, FirstRound);

	return 0;
}
