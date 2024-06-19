#include <iostream>
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
	for (int i = 0; i < a->size(); i++) q->Delete(a->at(i).first, a->at(i).second);
}

int main()
{
	std::vector<std::pair<int, Point2D>> FirstRound = { {1, Point2D(-1,-1)},{2, Point2D(-3,-1)},{3, Point2D(-1,-9)},{4, Point2D(-7,-8)},{5, Point2D(-9,-5)} };
	/*std::vector<std::pair<int, Point2D>> FirstRound = {{1, Point2D(-1,-1)},{2, Point2D(-3,-1)},{3, Point2D(-1,-9)},{4, Point2D(-7,-8)},{5, Point2D(-9,-5)}
													   ,{6, Point2D(-3,-3)},{7, Point2D(-1,-3)},{8, Point2D(-9,-1)},{9, Point2D(-8,-7)},{10, Point2D(-5,-9)} 
													   ,{11, Point2D(-5,-3)},{12, Point2D(-8,-1)},{13, Point2D(-9,-3)},{14, Point2D(-2,-2)},{15, Point2D(-6,-4)} };*/
	Quad MyQuad(Point2D(-10,-10),Point2D(10,10));
	SeedQuad(&MyQuad, &FirstRound);
	std::cout << MyQuad.ToString();
	//EmptyQuad(&MyQuad, &FirstRound);
	//MyQuad.CleanUp();
	//std::cout << MyQuad.ToString();

	return 0;
}
