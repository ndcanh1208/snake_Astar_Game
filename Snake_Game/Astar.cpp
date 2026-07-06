#include "AStar.h"
#include <queue>
#include <algorithm>
#include <unordered_set>

Node::Node(int _x, int _y, Node* _parent) :
    x(_x), y(_y), g(0), h(0), f(0), parent(_parent), weight(1.0f) {
}

bool Node::operator>(const Node& other) const {
    return f > other.f;
}

bool isValid(int x, int y, const std::vector<std::vector<int>>& gameMap) {
    if (x < 0 || x >= WIDTH_CELLS || y < 0 || y >= HEIGHT_CELLS) return false;
    if (gameMap[x][y] == CellType::OBSTACLE ||
        gameMap[x][y] == CellType::SNAKE_BODY ||
        gameMap[x][y] == CellType::SNAKE_HEAD) return false;
    return true;
}

// Hàm A* cơ bản
std::vector<sf::Vector2i> findAStarPath(
    sf::Vector2i start,
    sf::Vector2i target,
    const std::vector<std::vector<int>>& gameMap
) {
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> openList;
    std::vector<std::vector<bool>> closedList(WIDTH_CELLS, std::vector<bool>(HEIGHT_CELLS, false));
    std::vector<std::vector<Node*>> allNodes(WIDTH_CELLS, std::vector<Node*>(HEIGHT_CELLS, nullptr));

    Node* startNode = new Node(start.x, start.y);
    startNode->h = std::abs(start.x - target.x) + std::abs(start.y - target.y);
    startNode->f = startNode->g + startNode->h;

    openList.push(*startNode);
    allNodes[start.x][start.y] = startNode;

    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };
    bool foundTarget = false;
    Node* targetNodeRef = nullptr;

    while (!openList.empty()) {
        Node currentNode = openList.top();
        openList.pop();

        int cx = currentNode.x;
        int cy = currentNode.y;

        if (closedList[cx][cy]) continue;
        closedList[cx][cy] = true;

        if (cx == target.x && cy == target.y) {
            foundTarget = true;
            targetNodeRef = allNodes[cx][cy];
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (isValid(nx, ny, gameMap) && !closedList[nx][ny]) {
                int newG = currentNode.g + 1;

                if (allNodes[nx][ny] == nullptr) {
                    Node* neighbor = new Node(nx, ny, allNodes[cx][cy]);
                    neighbor->g = newG;
                    neighbor->h = std::abs(nx - target.x) + std::abs(ny - target.y);
                    neighbor->f = neighbor->g + neighbor->h;
                    allNodes[nx][ny] = neighbor;
                    openList.push(*neighbor);
                }
                else if (newG < allNodes[nx][ny]->g) {
                    allNodes[nx][ny]->g = newG;
                    allNodes[nx][ny]->f = newG + allNodes[nx][ny]->h;
                    allNodes[nx][ny]->parent = allNodes[cx][cy];
                    openList.push(*allNodes[nx][ny]);
                }
            }
        }
    }

    std::vector<sf::Vector2i> path;
    if (foundTarget && targetNodeRef != nullptr) {
        Node* curr = targetNodeRef;
        while (curr != nullptr) {
            path.push_back(sf::Vector2i(curr->x, curr->y));
            curr = curr->parent;
        }
        std::reverse(path.begin(), path.end());
    }

    for (int x = 0; x < WIDTH_CELLS; x++) {
        for (int y = 0; y < HEIGHT_CELLS; y++) {
            if (allNodes[x][y] != nullptr) delete allNodes[x][y];
        }
    }

    return path;
}

// Hàm kiểm tra xem còn đủ không gian để sống không
bool hasEnoughSpace(sf::Vector2i start,
    const std::vector<std::vector<int>>& gameMap,
    const Snake& snake) {

    std::queue<sf::Vector2i> q;
    std::vector<std::vector<bool>> visited(WIDTH_CELLS, std::vector<bool>(HEIGHT_CELLS, false));

    q.push(start);
    visited[start.x][start.y] = true;

    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };
    int count = 0;

    while (!q.empty()) {
        sf::Vector2i pos = q.front();
        q.pop();
        count++;

        for (int i = 0; i < 4; i++) {
            int nx = pos.x + dx[i];
            int ny = pos.y + dy[i];

            if (nx >= 0 && nx < WIDTH_CELLS && ny >= 0 && ny < HEIGHT_CELLS &&
                !visited[nx][ny] && gameMap[nx][ny] == CellType::EMPTY) {
                visited[nx][ny] = true;
                q.push(sf::Vector2i(nx, ny));
            }
        }
    }

    return count >= 5;
}

// Kiểm tra ô có an toàn không
bool isSafeCell(int x, int y,
    const std::vector<std::vector<int>>& gameMap,
    const Snake& snake) {

    auto body = snake.body;
    for (const auto& seg : body) {
        if (seg.x == x && seg.y == y) return false;
    }

    if (x < 0 || x >= WIDTH_CELLS || y < 0 || y >= HEIGHT_CELLS) return false;
    if (gameMap[x][y] == CellType::OBSTACLE) return false;

    return hasEnoughSpace(sf::Vector2i(x, y), gameMap, snake);
}

// Kiểm tra đường đi có an toàn không
bool isPathSafe(const std::vector<sf::Vector2i>& path,
    const std::vector<std::vector<int>>& gameMap,
    const Snake& snake) {

    if (path.size() < 2) return false;

    auto tempMap = gameMap;
    auto tempSnake = snake;

    for (size_t i = 1; i < path.size(); i++) {
        sf::Vector2i nextPos = path[i];
        bool isFood = (nextPos == sf::Vector2i(path[path.size() - 1].x, path[path.size() - 1].y));

        for (const auto& seg : tempSnake.body) {
            if (seg.x == nextPos.x && seg.y == nextPos.y && !isFood) {
                return false;
            }
        }

        tempSnake.body.insert(tempSnake.body.begin(), nextPos);
        if (!isFood) {
            tempSnake.body.pop_back();
        }

        if (!hasEnoughSpace(tempSnake.body[0], tempMap, tempSnake)) {
            return false;
        }
    }

    return true;
}

// Tìm hướng di chuyển an toàn nhất
sf::Vector2i findSafeMove(sf::Vector2i start,
    const std::vector<std::vector<int>>& gameMap,
    const Snake& snake) {

    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };
    int priority[4] = { 1, 0, 3, 2 }; // Ưu tiên: xuống, lên, phải, trái

    for (int i = 0; i < 4; i++) {
        int idx = priority[i];
        int nx = start.x + dx[idx];
        int ny = start.y + dy[idx];

        if (isValid(nx, ny, gameMap) && isSafeCell(nx, ny, gameMap, snake)) {
            return sf::Vector2i(nx, ny);
        }
    }

    return sf::Vector2i(-1, -1);
}

// Hàm tạo bản đồ tạm sau khi rắn di chuyển
std::vector<std::vector<int>> createTempMap(
    const std::vector<std::vector<int>>& gameMap,
    const Snake& snake,
    const sf::Vector2i& newHead,
    bool isEating
) {
    auto tempMap = gameMap;
    auto tempSnake = snake;

    // Cập nhật rắn tạm
    tempSnake.body.insert(tempSnake.body.begin(), newHead);
    if (!isEating) {
        tempSnake.body.pop_back();
    }

    // Xóa dấu rắn cũ trên map tạm
    for (int x = 0; x < WIDTH_CELLS; x++) {
        for (int y = 0; y < HEIGHT_CELLS; y++) {
            if (tempMap[x][y] == CellType::SNAKE_HEAD ||
                tempMap[x][y] == CellType::SNAKE_BODY) {
                tempMap[x][y] = CellType::EMPTY;
            }
        }
    }

    // Đánh dấu rắn mới
    for (size_t i = 0; i < tempSnake.body.size(); i++) {
        if (i == 0) {
            tempMap[tempSnake.body[i].x][tempSnake.body[i].y] = CellType::SNAKE_HEAD;
        }
        else {
            tempMap[tempSnake.body[i].x][tempSnake.body[i].y] = CellType::SNAKE_BODY;
        }
    }

    return tempMap;
}

// Hàm tìm đường thông minh: đi đến đuôi và kiểm tra từng bước
std::vector<sf::Vector2i> findPathWithTailCheck(
    sf::Vector2i start,
    sf::Vector2i food,
    const std::vector<std::vector<int>>& gameMap,
    const Snake& snake
) {
    // ===== BƯỚC 1: Thử tìm đường đến thức ăn =====
    auto pathToFood = findAStarPath(start, food, gameMap);
    if (!pathToFood.empty() && pathToFood.size() > 1) {
        std::cout << "co duong den thuc an\n";
        return pathToFood;
    }

    // ===== BƯỚC 2: Không có đường, tìm đường đến đuôi =====
    if (snake.body.size() <= 1) {
        std::cout << "khong co duoi\n";
        return std::vector<sf::Vector2i>();
    }

    sf::Vector2i tail = snake.body[snake.body.size() - 1];
    auto pathToTail = findAStarPath(start, tail, gameMap);

    if (pathToTail.empty() || pathToTail.size() <= 1) {
        std::cout << "khong tim duocc duong den duoi\n";
        return std::vector<sf::Vector2i>();
    }

    std::cout << "dang di den duoi, kiem tra tung buoc...\n";

    // ===== BƯỚC 3: Mô phỏng từng bước đến đuôi =====
    auto tempSnake = snake;
    auto tempMap = gameMap;

    for (size_t i = 1; i < pathToTail.size(); i++) {
        sf::Vector2i nextPos = pathToTail[i];
        bool isEating = (nextPos == food);

        // Mô phỏng di chuyển
        tempSnake.body.insert(tempSnake.body.begin(), nextPos);
        if (!isEating) {
            tempSnake.body.pop_back();
        }

        // Cập nhật map tạm
        tempMap = createTempMap(gameMap, snake, nextPos, isEating);

        // 🔥 KIỂM TRA: Từ vị trí mới có thể đến thức ăn không?
        auto checkPathToFood = findAStarPath(nextPos, food, tempMap);

        if (!checkPathToFood.empty() && checkPathToFood.size() > 1) {
            std::cout << "  tim thay thuc an sau " << i << " buoc!\n";

            // Ghép đường đi: path đến vị trí hiện tại + đường đến food
            std::vector<sf::Vector2i> finalPath;
            for (size_t j = 0; j <= i; j++) {
                finalPath.push_back(pathToTail[j]);
            }
            // Thêm đường đến food (bỏ qua vị trí đầu trùng)
            for (size_t j = 1; j < checkPathToFood.size(); j++) {
                finalPath.push_back(checkPathToFood[j]);
            }
            return finalPath;
        }

        // Kiểm tra xem còn đủ không gian sống không
        if (!hasEnoughSpace(nextPos, tempMap, tempSnake)) {
            std::cout << "  het khong gian tai buoc " << i << "\n";
            break;
        }
    }

    // ===== BƯỚC 4: Không tìm thấy đường đến food, chỉ đi 1 bước về đuôi =====
    std::cout << "  khong tim thay duong den thuc an, di toi duoi\n";
    std::vector<sf::Vector2i> result;
    result.push_back(start);
    result.push_back(pathToTail[1]);
    return result;
}