#include "Snake.h"

Snake::Snake(sf::Vector2i startPos) {
    body.push_back(startPos);
    body.push_back(startPos + sf::Vector2i(0, 1));
    body.push_back(startPos + sf::Vector2i(0, 2));
}

sf::Vector2i Snake::getHeadPosition() {
    return body[0];
}

void Snake::move(sf::Vector2i nextStep, bool grow) {
    body.insert(body.begin(), nextStep);
    if (!grow) {
        body.pop_back();
    }
}

void Snake::updateToMap(std::vector<std::vector<int>>& gameMap) {
    for (size_t i = 0; i < body.size(); i++) {
        if (i == 0) {
            gameMap[body[i].x][body[i].y] = CellType::SNAKE_HEAD;
        }
        else {
            gameMap[body[i].x][body[i].y] = CellType::SNAKE_BODY;
        }
    }
}