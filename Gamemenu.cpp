#include "Gamemenu.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

GameMenu::GameMenu(float width, float height)
{
	font.loadFromFile("assets/fonts/Gatrich.otf");
	font2.loadFromFile("assets/fonts/Arial.ttf");
	//game1
	selectionmenu[0].setFont(font);
	selectionmenu[0].setFillColor(Color::Black);
	selectionmenu[0].setString("Tic-Tac-Toe");
	selectionmenu[0].setCharacterSize(40);
	selectionmenu[0].setPosition(145, 310);
	//game2
	selectionmenu[1].setFont(font);
	selectionmenu[1].setFillColor(Color::White);
	selectionmenu[1].setString("Maze Runner");
	selectionmenu[1].setCharacterSize(40);
	selectionmenu[1].setPosition(500, 310);
	//game3
	selectionmenu[2].setFont(font);

	selectionmenu[2].setFillColor(Color::White);
	selectionmenu[2].setString("Pong");
	selectionmenu[2].setCharacterSize(40);
	selectionmenu[2].setPosition(855, 310);

	//game5
	selectionmenu[3].setFont(font2);
	selectionmenu[3].setFillColor(Color::White);
	selectionmenu[3].setString("When in game press escape or close the window to come back to this menu");
	selectionmenu[3].setCharacterSize(40);
	selectionmenu[3].setPosition(250, 900);

	selectionmenu[4].setFont(font);
	selectionmenu[4].setFillColor(Color::White);
	selectionmenu[4].setString("Pick a path");
	selectionmenu[4].setCharacterSize(70);
	selectionmenu[4].setPosition(120, 120);

	selected = 0;
}

GameMenu::~GameMenu() {}

void GameMenu::setSelected(int n)
{
	selected = n;
}

void GameMenu::draw(RenderWindow& window)
{
	for (int i = 0; i < 5; i++)
	{
		window.draw(selectionmenu[i]);
	}
}

void GameMenu::MoveRight()
{
	if (selected + 1 <= 3)
	{
		selectionmenu[selected].setFillColor(Color::White);
		selected++;
		if (selected == 3)
		{
			selected = 0;
		}
		selectionmenu[selected].setFillColor(Color::Black);
	}
}

void GameMenu::MoveLeft()
{
	if (selected - 1 >= -1)
	{
		selectionmenu[selected].setFillColor(Color::White);
		selected--;
		if (selected == -1)
		{
			selected = 2;
		}
		selectionmenu[selected].setFillColor(Color::Black);



	}
}