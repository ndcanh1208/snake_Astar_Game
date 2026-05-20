#include "Snake.h"

Snake::Snake(sf::Vector2i startPos) {
    body.push_back(startPos);      // Đầu rắn
    body.push_back(startPos + sf::Vector2i(0, 1)); // Thân rắn thứ 1
    body.push_back(startPos + sf::Vector2i(0, 2)); // Thân rắn thứ 2
}

sf::Vector2i Snake::getHeadPosition() {
    return body[0];
}

// Hàm di chuyển rắn sang ô tiếp theo
void Snake::move(sf::Vector2i nextStep, bool grow) {
    // Thêm đầu mới vào vị trí bước đi tiếp theo
    body.insert(body.begin(), nextStep);

    // Nếu không ăn mồi (không grow) thì cắt bớt đốt đuôi cũ đi
    if (!grow) {
        body.pop_back();
    }
}

// Hàm cập nhật vị trí rắn vào ma trận bản đồ
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