#ifndef ASTAR_H
#define ASTAR_H

#include "Common.h"
#include "Snake.h"

struct Node {
    int x, y;
    int g, h, f;
    Node* parent;
    float weight;

    Node(int _x, int _y, Node* _parent = nullptr);
    bool operator>(const Node& other) const;
};

// Hàm A* cơ bản
std::vector<sf::Vector2i> findAStarPath(
    sf::Vector2i start,
    sf::Vector2i target,
    const std::vector<std::vector<int>>& gameMap
);

// Hàm tìm đường thông minh: đi đến đuôi và kiểm tra từng bước
std::vector<sf::Vector2i> findPathWithTailCheck(
    sf::Vector2i start,
    sf::Vector2i food,
    const std::vector<std::vector<int>>& gameMap,
    const Snake& snake
);

// Hàm tìm hướng di chuyển an toàn nhất
sf::Vector2i findSafeMove(
    sf::Vector2i start,
    const std::vector<std::vector<int>>& gameMap,
    const Snake& snake
);

// Kiểm tra ô có an toàn không
bool isSafeCell(
    int x, int y,
    const std::vector<std::vector<int>>& gameMap,
    const Snake& snake
);

// Kiểm tra đường đi có an toàn không
bool isPathSafe(
    const std::vector<sf::Vector2i>& path,
    const std::vector<std::vector<int>>& gameMap,
    const Snake& snake
);

// Hàm kiểm tra xem còn đủ không gian để sống không
bool hasEnoughSpace(
    sf::Vector2i start,
    const std::vector<std::vector<int>>& gameMap,
    const Snake& snake
);

#endif