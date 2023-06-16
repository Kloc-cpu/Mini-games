#include "tictactoe.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

std::string getPlayerNameFromSFML()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Enter Your Name");
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf"))
    {
        std::cerr << "Font loading error" << std::endl;
    }

    sf::Text promptText;
    promptText.setFont(font);
    promptText.setString("Enter your name:");
    promptText.setCharacterSize(30);
    promptText.setFillColor(sf::Color::White);
    promptText.setPosition(sf::Vector2f(10.f, 10.f));

    sf::Text nameText;
    nameText.setFont(font);
    nameText.setCharacterSize(30);
    nameText.setFillColor(sf::Color::White);
    nameText.setPosition(sf::Vector2f(10.f, 60.f));

    std::string playerName;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return "";  // Return an empty string if the window is closed
            }
            else if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b' && playerName.size() > 0)
                {
                    // Handle backspace
                    playerName.pop_back();
                }
                else if (event.text.unicode == '\r' || event.text.unicode == '\n')
                {
                    // Handle Enter or Return key
                    window.close();
                }
                else if (event.text.unicode < 128 && playerName.size() < 20)
                {
                    // Append entered character to the name
                    playerName += static_cast<char>(event.text.unicode);
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(promptText);
        nameText.setString(playerName);
        window.draw(nameText);
        window.display();
    }

    return playerName;
}

void saveScores(const string& playerName, int playerScore, int botScore, int tieScore) {
    ofstream scoresFile("scores.txt", ios::app);
    if (!scoresFile.is_open()) {
        cout << "Error creating/opening scores file." << endl;
        return;
    }

    scoresFile << playerName << "," << playerScore << "," << botScore << "," << tieScore << endl;
    scoresFile.close();
}


CircleShape construct_circle_at(int i) {
    int row = i / 3;
    int col = i % 3;
    CircleShape circle(90.f);
    circle.setFillColor(Color::Black);
    circle.setOutlineColor(Color::Black);
    circle.setOutlineThickness(10.f);
    circle.setPosition(Vector2f(row * 235.f + 600.f + 27.f, col * 235.f + 180.f + 27.f));
    return circle;
}


CircleShape construct_circle_at_bot(int i) {
    int row = i / 3;
    int col = i % 3;
    CircleShape circle(90.f);
    circle.setFillColor(Color::Transparent);
    circle.setOutlineColor(Color::Black);
    circle.setOutlineThickness(10.f);
    circle.setPosition(Vector2f(row * 235.f + 600.f + 27.f, col * 235.f + 180.f + 27.f));
    return circle;
}


GameLogic::GameLogic() {
    Reset_game();
    playerScore = botScore = tieScore = 0;
}
void GameLogic::Reset_game() {
    winner = -1;
    circles.clear();
    verts.clear();
    for (int i = 0; i < 9; ++i) board[i / 3][i % 3] = -1;
}
bool GameLogic::Finished_game()
{
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != -1 && board[i][0] == board[i][1] && board[i][1] == board[i][2]) winner = board[i][0];
        if (board[0][i] != -1 && board[0][i] == board[1][i] && board[1][i] == board[2][i]) winner = board[0][i];
    }
    if (board[1][1] != -1) {
        if (board[1][1] == board[0][0] && board[1][1] == board[2][2]) winner = board[1][1];
        if (board[1][1] == board[2][0] && board[1][1] == board[0][2]) winner = board[1][1];
    }
    bool is_finished = true;
    for (int i = 0; i < 9; ++i) {
        if (board[i / 3][i % 3] == -1) is_finished = false;
    }
    if (is_finished == true) winner = -2;
    return winner != -1 || is_finished;
}
bool GameLogic::Apply_player_move(int i)
{
    if (board[i / 3][i % 3] != -1) {
        // Tile is already occupied
        return false;
    }
    board[i / 3][i % 3] = 1;
    circles.push_back(construct_circle_at(i));
    return Finished_game();
}
bool GameLogic::Apply_bot_move()
{
    pair<int, int> move = maximizer_move(this->board);
    int tile = move.second;
    board[tile / 3][tile % 3] = 0;
    circles.push_back(construct_circle_at_bot(tile));
    return Finished_game();
}
bool GameLogic::Perform_game_turn(int i)
{
    if (Apply_player_move(i)) return true;

    if (Apply_bot_move()) return true;

    return false;
}

int position_of_tile(int x, int y) {
    int tileSize = 230;  // Adjust this value based on your tile size
    int spacing = 5;     // Adjust this value based on your desired spacing between tiles
    int boardOffsetX = 600;  // Adjust this value based on your game board offset on the X-axis
    int boardOffsetY = 180;  // Adjust this value based on your game board offset on the Y-axis

    int adjustedX = x - boardOffsetX;
    int adjustedY = y - boardOffsetY;

    if (adjustedX < 0 || adjustedY < 0)
        return -1;  // Mouse position is outside the game board

    int col = adjustedY / (tileSize + spacing);
    int row = adjustedX / (tileSize + spacing);

    if (row >= 3 || col >= 3)
        return -1;  // Mouse position is outside the game board

    return row * 3 + col;
}

int GameLogic::Run()
{
    FloatRect bounds;
    Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf"))
    {
        cerr << "font loading error" << endl;
    }
    string playerName = getPlayerNameFromSFML();
    RenderWindow window(VideoMode(1920, 1080), "SFML TicTacToe", Style::Titlebar | Style::Close);

    Text text1;
    text1.setFont(font);
    text1.setString("Winner");
    text1.setCharacterSize(30);
    text1.setFillColor(Color::White);

    Text win_lose_text;
    win_lose_text.setFont(font);
    win_lose_text.setCharacterSize(30);
    win_lose_text.setFillColor(Color::White);

    vector<RectangleShape> rects;
    for (int i = 0; i < 9; ++i) {
        int row = i / 3;
        int col = i % 3;

        RectangleShape r(Vector2f(230.f, 230.f));
        r.setFillColor(Color::White);
        r.setOutlineColor(Color::Black);
        r.setOutlineThickness(2.f);
        r.setPosition(Vector2f(col * 235.f + 600.f, row * 235.f + 180.f));

        rects.push_back(r);
    }




    RectangleShape replay_button(Vector2f(100, 50));
    replay_button.setFillColor(Color::White);
    replay_button.setOutlineColor(Color::Blue);
    bounds = replay_button.getLocalBounds();
    replay_button.setPosition(Vector2f(300.f + (150.f - bounds.width) / 2.f, 200.f));

    Text replay_text;
    replay_text.setFont(font);
    replay_text.setCharacterSize(25);
    replay_text.setString("REPLAY");
    bounds = replay_text.getLocalBounds();
    replay_text.setPosition(Vector2f(300.f + (150.f - bounds.width) / 2.f, 190.f + (50.f - bounds.height) / 2.f));

    GameLogic game;
    int mouse_x = 0;
    int mouse_y = 0;

    

    while (window.isOpen())
    {
        bool mouse_pressed = false;
        bool restart_hovered = false;
        bool restart_pressed = false;
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed) {
                mouse_pressed = true;
            }

            if (event.type == Event::MouseMoved) {
                Vector2i mouse_pos = Mouse::getPosition(window);
                mouse_x = mouse_pos.x;
                mouse_y = mouse_pos.y;
            }
            if (event.key.code == Keyboard::Escape)
            {
                window.close();
                ofstream scoresFile("scores.txt", ios::app);
                if (!scoresFile.is_open()) {
                    cout << "Error creating/opening scores file." << endl;
                }

                scoresFile << playerName << ":   Wins: " << playerScore << ", Loses: " << botScore << ", Ties: " << tieScore << endl;
                scoresFile.close();
                return 0;
            }
        }

        if (mouse_pressed && game.winner == -1) {
            int tile = position_of_tile(mouse_x, mouse_y);
            if (tile >= 0 && tile < 9 && game.board[tile / 3][tile % 3] == -1) {
                game.Perform_game_turn(tile);
                cout << tile << endl;
            }


        }

        if (mouse_pressed && mouse_x > (300.f + (150.f - bounds.width) / 2.f) && mouse_x < (300.f + (150.f + bounds.width) / 2.f) && mouse_y > 200 && mouse_y < 250) {
            game.Reset_game();
        }

        if (mouse_x > (300.f + (150.f - bounds.width) / 2.f) && mouse_x < (300.f + (150.f + bounds.width) / 2.f) && mouse_y > 200 && mouse_y < 250) {
            replay_button.setOutlineThickness(2);
            replay_text.setFillColor(Color::Cyan);
        }
        else {
            replay_button.setOutlineThickness(0);
            replay_text.setFillColor(Color::Black);
        }

        window.clear(Color::Black);
        window.draw(replay_button);
        window.draw(replay_text);
        for (RectangleShape r : rects) window.draw(r);
        for (CircleShape c : game.circles) window.draw(c);
        for (VertexArray v : game.verts) window.draw(v);
        bounds = text1.getLocalBounds();
        text1.setPosition(Vector2f(1350.f, 180.f));
        window.draw(text1);
        if (game.winner == BOT_ID && win_lose_text.getString() != "BOT")
        {
            win_lose_text.setString("BOT");
            botScore++;
        }
        else if (game.winner == PLAYER_ID && win_lose_text.getString() != "YOU")
        {
            win_lose_text.setString("YOU");
            playerScore++;
        }
        else if (game.winner == -2 && win_lose_text.getString() != "TIE")
        {
            win_lose_text.setString("TIE");
            tieScore++;
        }
        else if(game.winner == -1)win_lose_text.setString("PLAYING");
        bounds = win_lose_text.getLocalBounds();
        win_lose_text.setPosition(Vector2f(1350.f, 220.f));
        window.draw(win_lose_text);
        window.display();

    }
}