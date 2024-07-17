#include <iostream>
#include <SFML/Graphics.hpp>
#include "Quad.h"
#include "Point2D.h"

void SeedQuad(Quad* q, std::vector<std::pair<int, Point2D>>* a)
{
	//Insert all desired values into the quad
	for (int i = 0; i < a->size(); i++) q->Insert(a->at(i).first, a->at(i).second);
}

void EmptyQuad(Quad* q, std::vector<std::pair<int, Point2D>>* a)
{
	//Delete all desired values from the quad
	for (int i = 0; i < a->size(); i++)
	{
		q->Delete(a->at(i).first, a->at(i).second);
	}
}

int SFML(Quad* q)
{
	sf::RenderWindow window(sf::VideoMode(q->GetSize().X, q->GetSize().Y), "SFML works!");
	sf::CircleShape shape(100.f);
	sf::RectangleShape rect(sf::Vector2f((float)window.getSize().x - 10, (float)window.getSize().y - 10));
	rect.setOutlineColor(sf::Color::Magenta);
	rect.setOutlineThickness(5);
	rect.setFillColor(sf::Color::Black);
	rect.setPosition(sf::Vector2f(5,5));
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(rect);
		window.draw(shape);
		window.display();
	}

	return 0;
}

int main()
{
	std::vector<std::pair<int, Point2D>> FirstRound = { {1, Point2D(1,1)},{2, Point2D(3,1)},{3, Point2D(1,9)},{4, Point2D(7,8)},{5, Point2D(9,5)}
													   ,{6, Point2D(3,3)},{7, Point2D(1,3)},{8, Point2D(9,1)},{9, Point2D(8,7)},{10, Point2D(5,9)}
													   ,{11, Point2D(5,3)},{12, Point2D(8,1)},{13, Point2D(9,3)},{14, Point2D(2,2)},{15, Point2D(6,4)} };
	Quad MyQuad(Point2D(0,0),Point2D(1000,1000));
	SeedQuad(&MyQuad, &FirstRound);
	std::cout << MyQuad.ToString() + "\n\n";

	SFML(&MyQuad);

	EmptyQuad(&MyQuad, &FirstRound);
	std::cout << MyQuad.ToString() + "\n\n";

	return 0;
}
