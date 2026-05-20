#ifndef ASTAR_H
#define ASTAR_H

#include "Common.h"

struct Node {
    int x, y;
    int g, h, f;
    Node* parent;

    Node(int _x, int _y, Node* _parent = nullptr);
    bool operator>(const Node& other) const;
};

// Hàm tìm đường A*
std::vector<sf::Vector2i> findAStarPath(
    sf::Vector2i start,
    sf::Vector2i target,
    const std::vector<std::vector<int>>& gameMap
);

#endif