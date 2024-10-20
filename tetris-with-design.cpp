//Before use, you need to configure the SFML library documentation: https://habr.com/ru/articles/703500/
//just translate the article in a translator and paste the code

#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

const int fieldWidth = 18;
const int fieldHeight = 27;
int field[fieldHeight][fieldWidth] = { 0 };

struct Point { int x, y; } a[4], b[4];

int figures[7][4] = {
    1,3,5,7,
    2,4,5,7,
    3,5,4,6,
    3,5,4,7,
    2,3,5,7,
    3,5,7,6,
    2,3,4,5
};

bool doesPieceFit() {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 0 || a[i].x >= fieldWidth || a[i].y >= fieldHeight) return false;
        if (field[a[i].y][a[i].x]) return false;
    }
    return true;
}

int main() {
    srand(time(0));

    RenderWindow window(VideoMode(320, 480), "Tetris");

    Texture texture;
    texture.loadFromFile("tiles.png");

    Sprite block(texture);
    block.setTextureRect(IntRect(0, 0, 18, 18));

    int dx = 0;
    bool rotate = false;
    float timer = 0, delay = 0.3;
    Clock clock;

    Color colors[6] = {
        Color::Red, Color::Green, Color::Blue,
        Color::Magenta, Color::Cyan, Color::Yellow
    };

    Color currentColor = colors[rand() % 6];

    int currentPiece = rand() % 7;
    for (int i = 0; i < 4; i++) {
        a[i].x = figures[currentPiece][i] % 2 + fieldWidth / 2 - 1;
        a[i].y = figures[currentPiece][i] / 2;
    }

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Up) rotate = true;
                if (e.key.code == Keyboard::Left) dx = -1;
                if (e.key.code == Keyboard::Right) dx = 1;
            }
        }

        for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
        if (!doesPieceFit()) for (int i = 0; i < 4; i++) a[i] = b[i];

        if (rotate) {
            Point center = a[1];
            for (int i = 0; i < 4; i++) {
                int x = a[i].y - center.y;
                int y = a[i].x - center.x;
                a[i].x = center.x - x;
                a[i].y = center.y + y;
            }
            if (!doesPieceFit()) for (int i = 0; i < 4; i++) a[i] = b[i];
        }

        if (timer > delay) {
            for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

            if (!doesPieceFit()) {
                for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = 1;
                currentPiece = rand() % 7;
                currentColor = colors[rand() % 7];
                for (int i = 0; i < 4; i++) {
                    a[i].x = figures[currentPiece][i] % 2 + fieldWidth / 2 - 1;
                    a[i].y = figures[currentPiece][i] / 2;
                }
            }
            timer = 0;
        }

        dx = 0;
        rotate = 0;

        window.clear(Color::Black);

        for (int i = 0; i < fieldHeight; i++) {
            for (int j = 0; j < fieldWidth; j++) {
                if (field[i][j] == 0) continue;
                block.setPosition(j * 18, i * 18);
                block.setColor(Color(128, 128, 128));
                window.draw(block);
            }
        }

        for (int i = 0; i < 4; i++) {
            block.setPosition(a[i].x * 18, a[i].y * 18);
            block.setColor(currentColor);
            window.draw(block);
        }

        window.display();
    }

    return 0;
}
