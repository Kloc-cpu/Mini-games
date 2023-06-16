#include "Menu.h"
#include "Gamemenu.h"
#include "tictactoe.h"
#include "maze.h"
#include "pong.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>

using namespace std;
using namespace sf;

int pagenum = 1000, game_selection;

void imageload(GameMenu& gamemenu, Texture& background)
{
    switch (gamemenu.pressed())
    {
    case 0:
        background.loadFromFile("assets/image/tictactoe.jpg");
        break;
    case 1:
        background.loadFromFile("assets/image/random2.jpg");
        break;
    case 2:
        background.loadFromFile("assets/image/random.jpg");
        break;
    }
}

int selectGame(RenderWindow& mainmenu, GameMenu& gamemenu, Texture& background, Sprite& bg)
{
    imageload(gamemenu, background);
    while (mainmenu.isOpen())
    {
        Event event;
        while (mainmenu.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                mainmenu.close();
                break;
            }
            if (event.type == Event::KeyPressed)
            {
                if (event.key.code == Keyboard::Right)
                {
                    gamemenu.MoveRight();
                }
                if (event.key.code == Keyboard::Left)
                {
                    gamemenu.MoveLeft();
                }
                if (event.key.code == Keyboard::Enter)
                {
                    game_selection = gamemenu.pressed();
                    mainmenu.clear();
                    return game_selection;
                }
                imageload(gamemenu, background);
            }

        }
        mainmenu.draw(bg);
        gamemenu.draw(mainmenu);
        mainmenu.display();
    }
    return -1;
}

int main()
{
    RenderWindow mainmenu(VideoMode(1920, 1080), "Game", Style::Default);
    Menu menu(1920, 1080);
    GameMenu gamemenu(1920, 1080);
    Texture background;
    background.loadFromFile("assets/image/background.jpg");
    Sprite bg;
    bg.setTexture(background);


    bool gameSelected = false;  // Flag to track if a game has been selected
    bool exitGame = false;  // Flag to track when to exit the main loop

    while (!exitGame)
    {
        if (pagenum == 1000) //main menu if
        {
            while (mainmenu.isOpen() && !exitGame)
            {
                Event event;
                while (mainmenu.pollEvent(event))
                {
                    if (event.type == Event::Closed)
                    {
                        mainmenu.close();
                        exitGame = true;  // Set the flag to exit the main loop
                        break;
                    }
                    if (event.type == Event::KeyPressed)
                    {
                        if (event.key.code == Keyboard::Up)
                        {
                            menu.MoveUp();
                        }
                        if (event.key.code == Keyboard::Down)
                        {
                            menu.MoveDown();
                        }
                        if (event.key.code == Keyboard::Enter)
                        {
                            if (menu.pressed() == 0)
                                pagenum = 0;
                            else if (menu.pressed() == 1)
                                pagenum = 1;
                            else if (menu.pressed() == 2)
                                pagenum = -1;
                        }
                    }
                }
                if (pagenum != 1000)
                    break;

                mainmenu.clear();
                mainmenu.draw(bg);
                menu.draw(mainmenu);
                mainmenu.display();
            }
            if (pagenum == -1)
            {
                mainmenu.close();
                break;
            }
            if (pagenum == 1)
            {
                RenderWindow window(VideoMode(1920, 1080), "About");
                Font font;
                if (!font.loadFromFile("assets/fonts/arial.ttf"))
                {
                    cerr << "font loading error" << endl;
                }

                Text text;
                text.setFont(font);
                text.setString("Hello,\n as you can see you move through the main menu by using the keys up and down\n by clicking play you will go to a screen where you will pick the game you want to play (navigate by pressing left and/or right)\n tic tac toe works by pressing on the desired tile with your mouse\n maze Runner works by using the arrow keys to navigate\n\n by pressing escape you will go back to the main menu");
                text.setCharacterSize(24);
                text.setPosition(100, 100);
                text.setFillColor(Color::White);

                while (window.isOpen())
                {
                    Event event;
                    while (window.pollEvent(event))
                    {
                        if (event.type == Event::Closed ||  event.key.code == Keyboard::Escape)
                        {
                            window.close();
                            pagenum = 1000;
                        }
                    }

                    window.clear();

                    window.draw(text);

                    window.display();
                }

            }

        }
        if (pagenum == 0 && !gameSelected)
        {
            game_selection = selectGame(mainmenu, gamemenu, background, bg);
            gameSelected = true;

            // Handle the selected game here
            switch (game_selection)
            {
                case 0:
                {
                    GameLogic tictactoe;
                    
                    pagenum = tictactoe.Run();
                    gameSelected = false;
                    break;
                }


                case 1: {
                    Cell maze;
                    pagenum = maze.Run();
                    gameSelected = false;
                    break;
                }

                case 2:
                {
                    PongGame pong;
                    pong.Run();
                    pagenum = 0;
                    gameSelected = false;
                    break;
                }
            }
        }
    }
}
