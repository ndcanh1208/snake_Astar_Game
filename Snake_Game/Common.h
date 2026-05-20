#ifndef COMMON_H
#define COMMON_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <queue>
#include <algorithm>

// Cấu hình kích thước ô lưới
const int CELL_SIZE = 30;
const int WIDTH_CELLS = 25;
const int HEIGHT_CELLS = 20;

// Các loại ô trên bản đồ
enum CellType {
    EMPTY = 0,
    WALL = 1,
    SNAKE_HEAD = 2,
    SNAKE_BODY = 3,
    FOOD = 4,
    OBSTACLE = 5
};

#endif