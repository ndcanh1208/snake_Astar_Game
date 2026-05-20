#include "AStar.h"

Node::Node(int _x, int _y, Node* _parent) : x(_x), y(_y), g(0), h(0), f(0), parent(_parent) {}

bool Node::operator>(const Node& other) const {
    return f > other.f;
}

bool isValid(int x, int y, const std::vector<std::vector<int>>& gameMap) {
    if (x < 0 || x >= WIDTH_CELLS || y < 0 || y >= HEIGHT_CELLS) return false;
    if (gameMap[x][y] == CellType::OBSTACLE || gameMap[x][y] == CellType::SNAKE_BODY) return false;
    return true;
}

std::vector<sf::Vector2i> findAStarPath(sf::Vector2i start, sf::Vector2i target, const std::vector<std::vector<int>>& gameMap) {
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