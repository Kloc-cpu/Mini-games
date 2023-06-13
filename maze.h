#pragma once

#include <SFML/Graphics.hpp>
#include <stack>
#include <iostream>
#include <random>

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


    Cell();
    Cell(int, int);
    void draw(sf::RenderWindow* window);
};





void resetMaze(Cell* maze, int size) {
    for (int i = 0; i < size * size; i++) {
        for (int j = 0; j < 4; j++) {
            maze[i].walls[j] = true;
            maze[i].visited = false;
            maze[i].isActive = false;
        }
    }

}

void removeWallsBetween(Cell* maze, Cell* current, Cell* chosen, int size) {
    // top
    if (current->pos - size == chosen->pos) {
        current->walls[0] = false;
        chosen->walls[2] = false;
        // right
    }
    else if (current->pos + 1 == chosen->pos) {
        current->walls[1] = false;
        chosen->walls[3] = false;
        // bottom 
    }
    else if (current->pos + size == chosen->pos) {
        current->walls[2] = false;
        chosen->walls[0] = false;
        // left 
    }
    else if (current->pos - 1 == chosen->pos) {
        current->walls[3] = false;
        chosen->walls[1] = false;
    }
}

void makeMaze(Cell* maze, int size) {
    resetMaze(maze, size);
    stack<Cell> stack;
    maze[0].visited = true;
    stack.push(maze[0]);

    while (!stack.empty()) {
        Cell current = stack.top();
        stack.pop();
        int pos = current.pos;
        vector<int> neighbours;

        if ((pos) % (size) != 0 && pos > 0) {
            Cell left = maze[pos - 1];
            if (!left.visited) {
                neighbours.push_back(pos - 1);
            }
        }
        if ((pos + 1) % (size) != 0 && pos < size * size) {
            Cell right = maze[pos + 1];
            if (!right.visited) {
                neighbours.push_back(pos + 1);
            }

        }
        if ((pos + size) < size * size) {
            Cell bottom = maze[pos + size];
            if (!bottom.visited) {
                neighbours.push_back(pos + size);
            }
        }

        if ((pos - size) > 0) {
            Cell top = maze[pos - size];
            if (!top.visited) {
                neighbours.push_back(pos - size);
            }
        }

        if (neighbours.size() > 0) {
            // generate a random array index for selecting a neighbour
            random_device dev;
            mt19937 rng(dev());
            uniform_int_distribution<mt19937::result_type> dist6(0, neighbours.size() - 1);
            int randneighbourpos = dist6(rng);

            Cell* chosen = &maze[neighbours[randneighbourpos]];

            stack.push(current);

            removeWallsBetween(maze, &maze[current.pos], chosen, size);

            chosen->visited = true;
            stack.push(*chosen);
        }

    }
}
void handleMove(sf::Event event, Cell* maze, int* currentPos, int size) {
    if (
        event.key.code == sf::Keyboard::Left ||
        event.key.code == sf::Keyboard::H
        ) {
        if (!maze[*currentPos].walls[3] && !maze[*currentPos - 1].walls[1]) {
            *currentPos = *currentPos - 1;
            maze[*currentPos].isActive = true;

        }
    }
    else if (
        event.key.code == sf::Keyboard::Right ||
        event.key.code == sf::Keyboard::L
        ) {
        if (!maze[*currentPos].walls[1] && !maze[*currentPos + 1].walls[3]) {
            *currentPos = *currentPos + 1;
            maze[*currentPos].isActive = true;
        }
    }
    else if (
        event.key.code == sf::Keyboard::Up ||
        event.key.code == sf::Keyboard::K
        ) {
        if ((*currentPos - size) < 0) {
            return;
        }
        if (!maze[*currentPos].walls[0] && !maze[*currentPos - size].walls[2]) {
            *currentPos = *currentPos - size;
            maze[*currentPos].isActive = true;
        }
    }
    else if (
        event.key.code == sf::Keyboard::Down ||
        event.key.code == sf::Keyboard::J
        ) {
        if ((*currentPos + size) > size * size) {
            return;
        }
        if (!maze[*currentPos].walls[2] && !maze[*currentPos + size].walls[0]) {
            *currentPos = *currentPos + size;
            maze[*currentPos].isActive = true;
        }
    }
}


Cell::Cell() {}
Cell::Cell(int _x, int _y) {
    x = _x;
    y = _y;
}

void Cell::draw(sf::RenderWindow* window) {
    sf::RectangleShape rect;

    if (isActive) {
        rect.setFillColor(sf::Color(247, 23, 53));
        rect.setSize(sf::Vector2f(size, size));
        rect.setPosition(x, y);
        window->draw(rect);
    }
    rect.setFillColor(sf::Color(223, 243, 228));

    if (walls[0]) {
        rect.setSize(sf::Vector2f(size, thickness));
        rect.setPosition(x, y);
        window->draw(rect);
    }
    // right
    if (walls[1]) {
        rect.setSize(sf::Vector2f(thickness, size));
        rect.setPosition(x + size, y);
        window->draw(rect);
    }
    // bottom
    if (walls[2]) {
        rect.setSize(sf::Vector2f(size + thickness, thickness));
        rect.setPosition(x, y + size);
        window->draw(rect);
    }
    // left
    if (walls[3]) {
        rect.setSize(sf::Vector2f(thickness, size));
        rect.setPosition(x, y);
        window->draw(rect);
    }


}