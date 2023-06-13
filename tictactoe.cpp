#include "tictactoe.h"
#include <SFML/Graphics.hpp>
#include <iostream>



sf::CircleShape construct_circle_at(int i) {
    int row = i / 3;
    int col = i % 3;
    sf::CircleShape circle(90.f);
    circle.setFillColor(sf::Color::Black);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(10.f);
    circle.setPosition(sf::Vector2f(row * 235.f + 600.f + 27.f, col * 235.f + 180.f + 27.f));
    return circle;
}


sf::CircleShape construct_circle_at_bot(int i) {
    int row = i / 3;
    int col = i % 3;
    sf::CircleShape circle(90.f);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Black);
    circle.setOutlineThickness(10.f);
    circle.setPosition(sf::Vector2f(row * 235.f + 600.f + 27.f, col * 235.f + 180.f + 27.f));
    return circle;
}


GameLogic::GameLogic() {
    reset_game();
}
void GameLogic::reset_game() {
    winner = -1;
    circles.clear();
    verts.clear();
    for (int i = 0; i < 9; ++i) board[i / 3][i % 3] = -1;
}
bool GameLogic::finished_game()
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
bool GameLogic::apply_player_move(int i)
{
    if (board[i / 3][i % 3] != -1) {
        // Tile is already occupied
        return false;
    }
    board[i / 3][i % 3] = 1;
    circles.push_back(construct_circle_at(i));
    return finished_game();
}
bool GameLogic::apply_bot_move()
{
    pair<int, int> move = maximizer_move(this->board);
    int tile = move.second;
    board[tile / 3][tile % 3] = 0;
    circles.push_back(construct_circle_at_bot(tile));
    return finished_game();
}
bool GameLogic::perform_game_turn(int i)
{
    if (apply_player_move(i)) return true;
    
    if (apply_bot_move()) return true;
    
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