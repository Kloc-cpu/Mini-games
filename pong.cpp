#include "pong.h"
#include <thread>
#include <chrono>
#include <iostream>


PongGame::PongGame() :
    window(VideoMode(800, 600), "Pong Game"),
    paddle1(Vector2f(20, 100)),
    paddle2(Vector2f(20, 100)),
    ball(10),
    ballSpeedX(-1.0f),
    ballSpeedY(1.0f),
    leftScore(0),
    rightScore(0),
    isFirstBounce(true)
{
    paddle1.setFillColor(Color::White);
    paddle1.setPosition(50, 250);

    paddle2.setFillColor(Color::White);
    paddle2.setPosition(730, 250);

    ball.setFillColor(Color::White);
    ball.setPosition(400, 300);
}

void PongGame::run()
{
    this_thread::sleep_for(chrono::seconds(1));

    while (window.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void PongGame::processEvents()
{
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
            window.close();
        if (event.key.code == Keyboard::Escape)
        {
            window.close();
            
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::W) && paddle1.getPosition().y > 0)
        paddle1.move(0, -3.5);

    if (Keyboard::isKeyPressed(Keyboard::S) && paddle1.getPosition().y + paddle1.getSize().y < window.getSize().y)
        paddle1.move(0, 3.5);

    if (Keyboard::isKeyPressed(Keyboard::Up) && paddle2.getPosition().y > 0)
        paddle2.move(0, -3.5);

    if (Keyboard::isKeyPressed(Keyboard::Down) && paddle2.getPosition().y + paddle2.getSize().y < window.getSize().y)
        paddle2.move(0, 3.5);
}

void PongGame::update()
{
    ball.move(ballSpeedX, ballSpeedY);

    if (ball.getPosition().y < 0 || ball.getPosition().y + ball.getRadius() * 2 > window.getSize().y)
    {
        ballSpeedY = -ballSpeedY;
        
    }

    if (ball.getGlobalBounds().intersects(paddle1.getGlobalBounds()) || ball.getGlobalBounds().intersects(paddle2.getGlobalBounds()))
    {
        ballSpeedX = -ballSpeedX;
        if (isFirstBounce == true)
        {
            ballSpeedX = -5;
            ballSpeedY = 3.5;
            isFirstBounce = false;
        }
    }
    if (ball.getPosition().x + ball.getRadius() * 2 < 0)
    {
        
        ++rightScore;
        ball.setPosition(400, 300);
        this_thread::sleep_for(chrono::seconds(1));
        ballSpeedX = 1;
        ballSpeedY = 1;
        isFirstBounce = true;
        
    }
    else if (ball.getPosition().x > window.getSize().x)
    {
        
        ++leftScore;
        ball.setPosition(400, 300);
        this_thread::sleep_for(chrono::seconds(1));
        ballSpeedX = 1;
        ballSpeedY = 1;
        isFirstBounce = true;
        
    }
}

void PongGame::render()
{
    window.clear();
    window.draw(paddle1);
    window.draw(paddle2);
    window.draw(ball);

    Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf"))
    {
        cout << "Failed to load font" << endl;
        return;
    }

    Text leftScoreText;
    leftScoreText.setFont(font);
    leftScoreText.setString("Left Player: " + to_string(leftScore));
    leftScoreText.setCharacterSize(24);
    leftScoreText.setFillColor(Color::White);
    leftScoreText.setPosition(20, 20);
    window.draw(leftScoreText);

    Text rightScoreText;
    rightScoreText.setFont(font);
    rightScoreText.setString("Right Player: " + to_string(rightScore));
    rightScoreText.setCharacterSize(24);
    rightScoreText.setFillColor(Color::White);
    rightScoreText.setPosition(window.getSize().x - rightScoreText.getGlobalBounds().width - 20, 20);
    window.draw(rightScoreText);

    Text controlText;
    controlText.setFont(font);
    controlText.setString("Controls:\nLeft Player: W/S\nRight Player: Up/Down");
    controlText.setCharacterSize(24);
    controlText.setFillColor(Color::White);
    controlText.setPosition(window.getSize().x / 2 - controlText.getGlobalBounds().width / 2, 0);
    window.draw(controlText);

    window.display();
}
