#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
using namespace std;
using namespace sf;

constexpr auto BOT_ID = 0;
constexpr auto PLAYER_ID = 1;



class GameLogic {
private:
    int tieScore;
    int botScore;
    int playerScore;


    pair<int, bool> game_state(int board[3][3]) {
        int winner = -1;
        for (int i = 0; i < 3; ++i) {
            if (board[i][0] != -1 && board[i][0] == board[i][1] && board[i][1] == board[i][2]) winner = board[i][0];
            if (board[0][i] != -1 && board[0][i] == board[1][i] && board[1][i] == board[2][i]) winner = board[0][i];
        }
        if (board[1][1] != -1) {
            if (board[1][1] == board[0][0] && board[1][1] == board[2][2]) winner = board[1][1];
            if (board[1][1] == board[2][0] && board[1][1] == board[0][2]) winner = board[1][1];
        }
        bool is_finished = winner != -1;
        if (!is_finished) {
            for (int i = 0; i < 9; ++i) {
                if (board[i / 3][i % 3] == -1) is_finished = false;
            }
        }
        if (winner == PLAYER_ID) return make_pair(-1, is_finished);
        else if (winner == BOT_ID) return make_pair(1, is_finished);
        return make_pair(0, is_finished);
    }
    pair<int, int> minimizer_move(int board[3][3]) {
        pair<int, bool> gs = game_state(board);
        if (gs.second) return make_pair(gs.first, -1);
        vector<pair<int, int>> possible_moves;
        for (int i = 0; i < 9; ++i) {
            if (board[i / 3][i % 3] != -1) continue;
            int b[3][3];
            for (int j = 0; j < 9; ++j) {
                if (i != j) b[j / 3][j % 3] = board[j / 3][j % 3];
                else b[j / 3][j % 3] = PLAYER_ID;
            }
            pair<int, int> move = maximizer_move(b);
            possible_moves.push_back(make_pair(move.first, i));
        }
        if (possible_moves.size() == 0) return make_pair(0, -1);
        sort(possible_moves.begin(), possible_moves.end(), [](pair<int, int> p1, pair<int, int> p2) {
            return p1.first < p2.first;
            });
        return possible_moves[0];
    }
    pair<int, int> maximizer_move(int board[3][3]) {
        pair<int, bool> gs = game_state(board);
        if (gs.second) return make_pair(gs.first, -1);
        vector<pair<int, int>> possible_moves;
        for (int i = 0; i < 9; ++i) {
            if (board[i / 3][i % 3] != -1) continue;
            int b[3][3];
            for (int j = 0; j < 9; ++j) {
                if (i != j) b[j / 3][j % 3] = board[j / 3][j % 3];
                else b[j / 3][j % 3] = BOT_ID;
            }
            pair<int, int> move = minimizer_move(b);
            possible_moves.push_back(make_pair(move.first, i));
        }

        if (possible_moves.size() == 0) return make_pair(0, -1);
        sort(possible_moves.begin(), possible_moves.end(), [](pair<int, int> p1, pair<int, int> p2) {
            return p1.first > p2.first;
            });
        return possible_moves[0];

        
    }
    
public:
    int board[3][3];
    int winner;
    vector<CircleShape> circles;
    vector<VertexArray> verts;
    string playerName;
    
    int Run();

    GameLogic();

    void Reset_game();

    bool Finished_game();

    bool Apply_player_move(int i);

    bool Apply_bot_move();

    bool Perform_game_turn(int i);

};

int position_of_tile(int x, int y);
