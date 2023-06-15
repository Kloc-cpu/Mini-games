#pragma once

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class GameMenu
{
public:
	Text selectionmenu[5];
	GameMenu(float width, float height);
	void draw(RenderWindow& window);
	void MoveLeft();
	void MoveRight();
	void setSelected(int n);
	int pressed()
	{
		return selected;
	}
	~GameMenu();
	
private:
	int selected;
	Font font;
	Font font2;
};