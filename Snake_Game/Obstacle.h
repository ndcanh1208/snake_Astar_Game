#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Common.h"

class ObstacleManager {
public:
    std::vector<sf::Vector2i> positions; // Lưu tọa độ của các ô chướng ngại vật

    ObstacleManager() = default;

    // Hàm sinh ngẫu nhiên N chướng ngại vật trên bản đồ
    void generateObstacles(int count, sf::Vector2i snakeHead, sf::Vector2i food) {
        positions.clear();

        while (positions.size() < count) {
            int x = rand() % WIDTH_CELLS;
            int y = rand() % HEIGHT_CELLS;
            sf::Vector2i pos(x, y);

            // ĐIỀU KIỆN TRÁNH: Không sinh vật cản đè lên đầu rắn, đuôi rắn hoặc mồi
            if (pos == snakeHead || pos == food) continue;

            // Tránh sinh vật cản quá sát đầu rắn lúc vừa vào game để rắn không chết nghẹn
            if (std::abs(pos.x - snakeHead.x) <= 2 && std::abs(pos.y - snakeHead.y) <= 2) continue;

            // Kiểm tra xem tọa độ này đã có vật cản nào chưa (tránh trùng)
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

    // Hàm cập nhật các chướng ngại vật vào ma trận bản đồ
    void updateToMap(std::vector<std::vector<int>>& gameMap) {
        for (const auto& pos : positions) {
            gameMap[pos.x][pos.y] = CellType::OBSTACLE;
        }
    }
};

#endif