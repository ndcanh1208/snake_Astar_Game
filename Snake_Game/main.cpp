#include "Common.h"
#include "AStar.h"
#include "Snake.h"
#include "Obstacle.h" // Khai báo thêm file quản lý vật cản
#include <ctime>

int main() {
    // Khởi tạo bộ sinh số ngẫu nhiên theo thời gian thực
    srand(static_cast<unsigned int>(time(0)));

    sf::RenderWindow window(sf::VideoMode(WIDTH_CELLS * CELL_SIZE, HEIGHT_CELLS * CELL_SIZE), "A* Snake Game - Super Obstacles");
    window.setFramerateLimit(60);

    // Khởi tạo map, rắn, mồi và trình quản lý vật cản
    std::vector<std::vector<int>> gameMap(WIDTH_CELLS, std::vector<int>(HEIGHT_CELLS, CellType::EMPTY));
    Snake snake(sf::Vector2i(3, 3)); // Cho rắn xuất phát ở góc trên bên trái
    sf::Vector2i food(12, 10);

    ObstacleManager obstacleMgr;

    // Cấu hình SỐ LƯỢNG CHƯỚNG NGẠI VẬT ở đây (Ví dụ: 50 ô vật cản)
    int obstacleCount = 100;
    obstacleMgr.generateObstacles(obstacleCount, snake.getHeadPosition(), food);

    sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
    sf::Clock clock;
    float timer = 0, delay = 0.12f; // Tăng tốc độ game lên một chút cho mượt

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // --- UPDATE LOGIC GAME ---
        if (timer > delay) {
            timer = 0;

            // Bước 1: Xóa trắng map để nạp lại trạng thái mới
            for (int x = 0; x < WIDTH_CELLS; x++) {
                for (int y = 0; y < HEIGHT_CELLS; y++) {
                    gameMap[x][y] = CellType::EMPTY;
                }
            }

            // Bước 2: Nạp các vật thể tĩnh và động lên ma trận
            gameMap[food.x][food.y] = CellType::FOOD;
            obstacleMgr.updateToMap(gameMap); // Đóng dấu các ô vật cản màu xanh dương
            snake.updateToMap(gameMap);       // Đóng dấu các ô thân rắn

            // Bước 3: Gọi A* đi tìm đường xuyên qua "mê cung"
            std::vector<sf::Vector2i> path = findAStarPath(snake.getHeadPosition(), food, gameMap);

            // Bước 4: Xử lý di chuyển
            if (path.size() > 1) {
                sf::Vector2i nextStep = path[1]; // Ô tiếp theo cần bước tới

                bool grow = (nextStep == food);
                snake.move(nextStep, grow);

                // Nếu đớp được mồi
                if (grow) {
                    // Tìm một vị trí đặt mồi mới sao cho không đè lên vật cản hay thân rắn
                    do {
                        food = sf::Vector2i(rand() % WIDTH_CELLS, rand() % HEIGHT_CELLS);
                    } while (gameMap[food.x][food.y] != CellType::EMPTY);

                    // THÚ VỊ: Mỗi lần ăn mồi, ta đổi luôn map mới (sinh lại các chướng ngại vật ngẫu nhiên)
                    obstacleMgr.generateObstacles(obstacleCount, snake.getHeadPosition(), food);
                }
            }
            else {
                // Nếu A* trả về path trống hoặc chỉ có 1 phần tử nghĩa là Rắn đã bị BỊT KÍN ĐƯỜNG (Game Over)
                std::cout << "Game Over! Rắn không tìm thấy đường đi an toàn nữa.\n";
                // Ở đây bạn có thể xử lý reset game hoặc chuyển cảnh tùy ý
            }
        }

        // --- RENDER ĐỒ HỌA SFML ---
        window.clear(sf::Color(25, 25, 25));

        for (int x = 0; x < WIDTH_CELLS; x++) {
            for (int y = 0; y < HEIGHT_CELLS; y++) {
                cellShape.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cellShape.setPosition(x * CELL_SIZE, y * CELL_SIZE);

                switch (gameMap[x][y]) {
                case CellType::EMPTY: cellShape.setFillColor(sf::Color(40, 40, 40)); break;
                case CellType::SNAKE_HEAD: cellShape.setFillColor(sf::Color::Green); break;
                case CellType::SNAKE_BODY: cellShape.setFillColor(sf::Color(0, 180, 0)); break;
                case CellType::FOOD: cellShape.setFillColor(sf::Color::Red); break;
                case CellType::OBSTACLE: cellShape.setFillColor(sf::Color(50, 100, 250)); break; // Màu xanh biển sáng
                }
                window.draw(cellShape);
            }
        }
        window.display();
    }
    return 0;
}