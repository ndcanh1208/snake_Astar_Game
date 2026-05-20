#ifndef SNAKE_H
#define SNAKE_H

#include "Common.h"

class Snake {
public:
    std::vector<sf::Vector2i> body; // Đốt đầu tiên body[0] là đầu rắn

    Snake(sf::Vector2i startPos);
    sf::Vector2i getHeadPosition();
    void move(sf::Vector2i nextStep, bool grow);
    void updateToMap(std::vector<std::vector<int>>& gameMap);
};

#endif