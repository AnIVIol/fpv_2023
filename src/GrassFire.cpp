#include <SFML/Graphics.hpp>
#include <queue>
#include <iostream>
#include <chrono>
#include <thread>

const int gridSize = 10;
const int cellSize = 50;

enum CellState {
    Empty,
    Obstacle,
    Path,
    Start,
    End
};

std::vector<std::vector<CellState>> grid(gridSize, std::vector<CellState>(gridSize, Empty));

int startRow = 3;
int startCol = 0;
int endRow = gridSize - 1;
int endCol = gridSize - 1;

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

void drawGrid(sf::RenderWindow& window) {
    for (int row = 0; row < gridSize; ++row) {
        for (int col = 0; col < gridSize; ++col) {
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            cell.setPosition(col * cellSize, row * cellSize);
            if (grid[row][col] == Empty)
                cell.setFillColor(sf::Color::White);
            else if (grid[row][col] == Obstacle)
                cell.setFillColor(sf::Color::Black);
            else if (grid[row][col] == Start)
                cell.setFillColor(sf::Color::Green);
            else if (grid[row][col] == End)
                cell.setFillColor(sf::Color::Red);
            else if (grid[row][col] == Path)
                cell.setFillColor(sf::Color::Blue);

            window.draw(cell);
        }
    }
}

void grassfire(sf::RenderWindow& window) {
    std::queue<std::pair<int, int>> q;
    q.push({startRow, startCol});

    while (!q.empty()) {
        int row = q.front().first;
        int col = q.front().second;
        q.pop();

        for (int d = 0; d < 4; ++d) {
            int newRow = row + dx[d];
            int newCol = col + dy[d];

            if (newRow >= 0 && newRow < gridSize && newCol >= 0 && newCol < gridSize &&
                grid[newRow][newCol] == Empty) {
                grid[newRow][newCol] = Path;

                sf::sleep(sf::milliseconds(100));
                window.clear();
                drawGrid(window);
                window.display();

                q.push({newRow, newCol});
            }
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(gridSize * cellSize, gridSize * cellSize), "Grassfire Path Planning");

    grid[2][2] = grid[3][2] = grid[4][2] = grid[5][2] = grid[6][2] = Obstacle;

    grid[startRow][startCol] = Start;
    grid[endRow][endCol] = End;

    drawGrid(window);
    window.display();

    sf::sleep(sf::seconds(1));

    grassfire(window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        drawGrid(window);

        window.display();
    }

    return 0;
}
