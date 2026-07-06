#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Common.h"

class ObstacleManager {
public:
    std::vector<sf::Vector2i> positions;

    ObstacleManager() = default;

    void generateObstacles(int count, sf::Vector2i snakeHead, sf::Vector2i food) {
        positions.clear();

        while (positions.size() < count) {
            int x = rand() % WIDTH_CELLS;
            int y = rand() % HEIGHT_CELLS;
            sf::Vector2i pos(x, y);

            if (pos == snakeHead || pos == food) continue;

            if (std::abs(pos.x - snakeHead.x) <= 2 && std::abs(pos.y - snakeHead.y) <= 2) continue;

            bool duplicated = false;
            for (const auto& p : positions) {
                if (p == pos) {
                    duplicated = true;
                    break;
                }
            }

            if (!duplicated) {
                positions.push_back(pos);
            }
        }
    }

    void updateToMap(std::vector<std::vector<int>>& gameMap) {
        for (const auto& pos : positions) {
            gameMap[pos.x][pos.y] = CellType::OBSTACLE;
        }
    }
};

#endif