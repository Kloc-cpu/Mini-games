#pragma once
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class PongGame {
public:
    PongGame();
    void run();
    bool isFirstBounce;

private:
    void processEvents();
    void update();
    void render();

    RenderWindow window;
    RectangleShape paddle1;
    RectangleShape paddle2;
    CircleShape ball;

    float ballSpeedX;
    float ballSpeedY;

    int leftScore;
    int rightScore;
};