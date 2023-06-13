#include "Menu.h"
#include "Gamemenu.h"
#include "tictactoe.h"
#include "maze.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>

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
        background.loadFromFile("assets/image/maze-runner.jpg");
        break;
    case 2:
        background.loadFromFile("assets/image/random.jpg");
        break;
    case 3:
        background.loadFromFile("assets/image/random2.jpg");
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

void game_maze()  //function that handles running the game maze runner
{
    int currentPos = 0;

    sf::RenderWindow window(sf::VideoMode(CELL_WIDTH * SIZE + 60, CELL_WIDTH * SIZE + 60), "maze");
    Cell* maze = new Cell[SIZE * SIZE];
    window.setFramerateLimit(30);
    // window.setVerticalSyncEnabled(true);

    for (int i = 30, k = 0; i < CELL_WIDTH * SIZE + 30; i += CELL_WIDTH) {
        for (int j = 30; j < CELL_WIDTH * SIZE + 30; j += CELL_WIDTH, k++) {
            maze[k].y = i;
            maze[k].x = j;
            maze[k].size = CELL_WIDTH;
            maze[k].pos = k;
        }
    }

    makeMaze(maze, SIZE);
    maze[currentPos].isActive = true;

    sf::RectangleShape currentPosRect;
    currentPosRect.setFillColor(sf::Color(166, 207, 213));
    currentPosRect.setSize(sf::Vector2f(CELL_WIDTH, CELL_WIDTH));

    sf::RectangleShape finishRect;
    finishRect.setFillColor(sf::Color(0, 128, 0));
    finishRect.setSize(sf::Vector2f(CELL_WIDTH, CELL_WIDTH));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                handleMove(event, maze, &currentPos, SIZE);
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                    pagenum = 0;
                }
                break;
            default:
                break;
            }
        }

        if (currentPos == (SIZE * SIZE - 1)) {
            makeMaze(maze, SIZE);
            currentPos = 0;
            maze[currentPos].isActive = true;
        }

        window.clear(sf::Color(13, 2, 33));

        for (int i = 0; i < SIZE * SIZE; i++) {
            maze[i].draw(&window);
        }

        currentPosRect.setPosition(maze[currentPos].x, maze[currentPos].y);
        window.draw(currentPosRect);
        finishRect.setPosition(maze[SIZE * SIZE - 1].x, maze[SIZE * SIZE - 1].y);
        window.draw(finishRect);

        window.display();
    }

    delete[] maze;
}

void tictactoe_game() //handles running the game tic tac toe
{
    sf::FloatRect bounds;
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf"))
    {
        std::cerr << "font loading error" << std::endl;
    }

    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML TicTacToe", sf::Style::Titlebar | sf::Style::Close);

    sf::Text text1;
    text1.setFont(font);
    text1.setString("Winner");
    text1.setCharacterSize(30);
    text1.setFillColor(sf::Color::White);

    sf::Text win_lose_text;
    win_lose_text.setFont(font);
    win_lose_text.setCharacterSize(30);
    win_lose_text.setFillColor(sf::Color::White);

    std::vector<sf::RectangleShape> rects;
    for (int i = 0; i < 9; ++i) {
        int row = i / 3;
        int col = i % 3;

        sf::RectangleShape r(sf::Vector2f(230.f, 230.f));
        r.setFillColor(sf::Color::White);
        r.setOutlineColor(sf::Color::Black);
        r.setOutlineThickness(2.f);
        r.setPosition(sf::Vector2f(col * 235.f + 600.f, row * 235.f + 180.f));

        rects.push_back(r);
    }




    sf::RectangleShape replay_button(sf::Vector2f(100, 50));
    replay_button.setFillColor(sf::Color::White);
    replay_button.setOutlineColor(sf::Color::Blue);
    bounds = replay_button.getLocalBounds();
    replay_button.setPosition(sf::Vector2f(300.f + (150.f - bounds.width) / 2.f, 200.f));

    sf::Text replay_text;
    replay_text.setFont(font);
    replay_text.setCharacterSize(25);
    replay_text.setString("REPLAY");
    bounds = replay_text.getLocalBounds();
    replay_text.setPosition(sf::Vector2f(300.f + (150.f - bounds.width) / 2.f, 190.f + (50.f - bounds.height) / 2.f));

    GameLogic game;
    int mouse_x = 0;
    int mouse_y = 0;

    while (window.isOpen())
    {
        bool mouse_pressed = false;
        bool restart_hovered = false;
        bool restart_pressed = false;
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                mouse_pressed = true;
            }

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                mouse_x = mouse_pos.x;
                mouse_y = mouse_pos.y;
            }
            if (event.key.code == sf::Keyboard::Escape)
            {
                window.close();
                pagenum = 0;
            }
        }

        if (mouse_pressed && game.winner == -1) {
            int tile = position_of_tile(mouse_x, mouse_y);
            if (tile >= 0 && tile < 9 && game.board[tile / 3][tile % 3] == -1) {
                game.perform_game_turn(tile);
                std::cout << tile << std::endl;
            }


        }

        if (mouse_pressed && mouse_x > (300.f + (150.f - bounds.width) / 2.f) && mouse_x < (300.f + (150.f + bounds.width) / 2.f) && mouse_y > 200 && mouse_y < 250) {
            game.reset_game();
        }

        if (mouse_x > (300.f + (150.f - bounds.width) / 2.f) && mouse_x < (300.f + (150.f + bounds.width) / 2.f) && mouse_y > 200 && mouse_y < 250) {
            replay_button.setOutlineThickness(2);
            replay_text.setFillColor(sf::Color::Cyan);
        }
        else {
            replay_button.setOutlineThickness(0);
            replay_text.setFillColor(sf::Color::Black);
        }

        window.clear(sf::Color::Black);
        window.draw(replay_button);
        window.draw(replay_text);
        for (sf::RectangleShape r : rects) window.draw(r);
        for (sf::CircleShape c : game.circles) window.draw(c);
        for (sf::VertexArray v : game.verts) window.draw(v);
        bounds = text1.getLocalBounds();
        text1.setPosition(sf::Vector2f(1350.f, 180.f));
        window.draw(text1);
        if (game.winner == BOT_ID) win_lose_text.setString("BOT");
        else if (game.winner == PLAYER_ID) win_lose_text.setString("YOU");
        else if (game.winner == -2) win_lose_text.setString("TIE");
        else win_lose_text.setString("PLAYING");
        bounds = win_lose_text.getLocalBounds();
        win_lose_text.setPosition(sf::Vector2f(1350.f, 220.f));
        window.draw(win_lose_text);
        window.display();
    }
    std::cout << game.winner << std::endl;
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
                sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Window");
                Font font;
                if (!font.loadFromFile("assets/fonts/arial.ttf"))
                {
                    std::cerr << "font loading error" << std::endl;
                }

                sf::Text text;
                text.setFont(font);
                text.setString("Hello,\n as you can see you move through the main menu by using the keys up and down\n by clicking play you will go to a screen where you will pick the game you want to play (navigate by pressing left and/or right)\n tic tac toe works by pressing on the desired tile with your mouse\n maze runner works by using the arrow keys to navigate\n\n by pressing escape you will go back to the main menu");
                text.setCharacterSize(24);
                text.setPosition(100, 100);
                text.setFillColor(sf::Color::White);

                while (window.isOpen())
                {
                    sf::Event event;
                    while (window.pollEvent(event))
                    {
                        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
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
                tictactoe_game();
                pagenum == 0;
                gameSelected = false;
                break;
            }


            case 1: {
                game_maze();
                pagenum == 0;
                gameSelected = false;
                break;
            }

            case 2:
                // Game 3: Another game
                // Add your code to start and handle the third game here
                break;
            case 3:
                // Game 4: Another game
                // Add your code to start and handle the fourth game here
                break;
            case 4:
                // Game 5: Another game
                // Add your code to start and handle the fifth game here
                break;
            
            }


        }
    }
}
