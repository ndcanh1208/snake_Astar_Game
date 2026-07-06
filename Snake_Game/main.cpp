#include "Common.h"
#include "AStar.h"
#include "Snake.h"
#include "Obstacle.h"
#include <ctime>

// Hàm reset game
void resetGame(Snake& snake, sf::Vector2i& food, ObstacleManager& obstacleMgr,
    std::vector<std::vector<int>>& gameMap, int obstacleCount) {
    snake = Snake(sf::Vector2i(3, 3));
    food = sf::Vector2i(12, 10);
    obstacleMgr.generateObstacles(obstacleCount, snake.getHeadPosition(), food);

    for (int x = 0; x < WIDTH_CELLS; x++) {
        for (int y = 0; y < HEIGHT_CELLS; y++) {
            gameMap[x][y] = CellType::EMPTY;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    sf::RenderWindow window(sf::VideoMode(WIDTH_CELLS * CELL_SIZE, HEIGHT_CELLS * CELL_SIZE), "A* Snake Game - Smart AI");
    window.setFramerateLimit(60);

    // Khởi tạo game
    std::vector<std::vector<int>> gameMap(WIDTH_CELLS, std::vector<int>(HEIGHT_CELLS, CellType::EMPTY));
    Snake snake(sf::Vector2i(3, 3));
    sf::Vector2i food(12, 10);

    ObstacleManager obstacleMgr;
    int obstacleCount = 80;
    obstacleMgr.generateObstacles(obstacleCount, snake.getHeadPosition(), food);

    sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
    sf::Clock clock;
    float timer = 0, delay = 0.10f;

    // Font cho text
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        // Nếu không có font, bỏ qua
    }
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    bool gameOver = false;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            // Phím R để reset game
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                resetGame(snake, food, obstacleMgr, gameMap, obstacleCount);
                gameOver = false;
                std::cout << "Game reset!\n";
            }
        }

        // --- UPDATE LOGIC ---
        if (!gameOver && timer > delay) {
            timer = 0;

            // Reset map
            for (int x = 0; x < WIDTH_CELLS; x++) {
                for (int y = 0; y < HEIGHT_CELLS; y++) {
                    gameMap[x][y] = CellType::EMPTY;
                }
            }

            // Nạp các đối tượng
            gameMap[food.x][food.y] = CellType::FOOD;
            obstacleMgr.updateToMap(gameMap);
            snake.updateToMap(gameMap);

            // Sử dụng hàm thông minh mới
            std::vector<sf::Vector2i> path = findPathWithTailCheck(
                snake.getHeadPosition(),
                food,
                gameMap,
                snake
            );

            // Xử lý di chuyển
            if (path.size() > 1) {
                sf::Vector2i nextStep = path[1];
                bool grow = (nextStep == food);
                snake.move(nextStep, grow);

                if (grow) {
                    // Tạo food mới
                    do {
                        food = sf::Vector2i(rand() % WIDTH_CELLS, rand() % HEIGHT_CELLS);
                    } while (gameMap[food.x][food.y] != CellType::EMPTY);

                    obstacleMgr.generateObstacles(obstacleCount, snake.getHeadPosition(), food);
                    std::cout << "an thuc an! Score: " << snake.body.size() - 3 << "\n";
                }
            }
            else {
                std::cout << "GAME OVER! khong tim duoc duong di.\n";
                gameOver = true;
            }
        }

        // --- RENDER ---
        window.clear(sf::Color(25, 25, 25));

        for (int x = 0; x < WIDTH_CELLS; x++) {
            for (int y = 0; y < HEIGHT_CELLS; y++) {
                cellShape.setPosition(x * CELL_SIZE, y * CELL_SIZE);

                switch (gameMap[x][y]) {
                case CellType::EMPTY:
                    cellShape.setFillColor(sf::Color(40, 40, 40));
                    break;
                case CellType::SNAKE_HEAD:
                    cellShape.setFillColor(sf::Color::Green);
                    break;
                case CellType::SNAKE_BODY:
                    cellShape.setFillColor(sf::Color(0, 180, 0));
                    break;
                case CellType::FOOD:
                    cellShape.setFillColor(sf::Color::Red);
                    break;
                case CellType::OBSTACLE:
                    cellShape.setFillColor(sf::Color(50, 100, 250));
                    break;
                }
                window.draw(cellShape);
            }
        }

        // Hiển thị score
        scoreText.setString("Score: " + std::to_string(snake.body.size() - 3));
        window.draw(scoreText);

        // Hiển thị Game Over
        if (gameOver) {
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("GAME OVER - Press R to restart");
            gameOverText.setCharacterSize(30);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(WIDTH_CELLS * CELL_SIZE / 2 - 150, HEIGHT_CELLS * CELL_SIZE / 2 - 20);
            window.draw(gameOverText);
        }

        window.display();
    }
    return 0;
}