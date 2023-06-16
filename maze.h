#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include <iostream>
#include <random>
using namespace std;
using namespace sf;

#define SIZE 30
#define CELL_WIDTH 20

class Cell
{
public:
    int x, y;
    int pos;
    float size = 30.f;
    float thickness = 2.f;
    bool walls[4] = { true,true,true,true };
    bool visited = false;
    bool isActive = false;
    
    int Run();
    Cell();
    Cell(int, int);
    void draw(RenderWindow* window);
};





void resetMaze(Cell* maze, int size);

void removeWallsBetween(Cell* maze, Cell* current, Cell* chosen, int size);

void makeMaze(Cell* maze, int size);

void handleMove(Event event, Cell* maze, int* currentPos, int size);
    
