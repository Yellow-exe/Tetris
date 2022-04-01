#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <windows.h>
#include<conio.h>
using std::cout;
using std::endl;
using namespace sf;
bool rotate = false;
int wid = 10, hei = 20;
int canvas[20][10] = { 0 };
float x_velocity = 0, delay = 0.3; bool init = false;
float interval_timer;
bool move_left = true, move_right = true;
//Struct point
int shapes[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};
struct Point { int x, y; } a[4], b[4];
int colourid = 1;

int main() {
    bool debugging = 0;
    srand(time(0));
    Clock glock; //bang!
    Clock stop; //for debugging
    sf::RenderWindow window(sf::VideoMode(180, 360), "Retard game!");
    Texture blocks;
    blocks.loadFromFile("C:/Users/wolfr/source/codeblock/tetris/images/tiles_i_drew.png");
    Sprite tetrisblocks(blocks);

    while (window.isOpen())
    {
        float time = glock.getElapsedTime().asSeconds();
        glock.restart();
        interval_timer += time;
        move_left = true;
        move_right = true;
        for (int i = 0; i < 4; i++) { if (a[i].x == 0 /*or canvas[a[i].y][a[i].x-1] != 0*/) { move_left = false; } }
        for (int i = 0; i < 4; i++) { if (a[i].x == 9 /*or canvas[a[i].y][a[i].x+1] != 0*/) { move_right = false; } }
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up) { rotate = true; }
                else if (event.key.code == Keyboard::Right && move_right && !debugging) { x_velocity = 1; }
                else if (event.key.code == Keyboard::Left && move_left && !debugging) { x_velocity = -1; }
                else if (event.key.code == Keyboard::Down) { delay = 0.01; }
            }
        }
        ///decipher the codes from "shapes"///
        //int n = 0;
        int n = rand() % 7;
        if (!init) {
            for (int i = 0; i < 4; i++) {
                a[i].x = shapes[n][i] % 2;
                a[i].y = shapes[n][i] / 2;
                colourid = rand() % 8;
                if (colourid == 0) { colourid = 8; }
                init = true;
            }
            for (int i = 0; i < 4; i++) { a[i].x += 3; a[i].y -= 2; }
        }
        ///bottom
        //cout << a[1].y;
        //for(int i = 0; i < 4; i++)
        for (int i = 0; i < 4; i++) {
            if (a[i].y == 19) {
                for (int i = 0; i < 4; i++) {
                    canvas[a[i].y][a[i].x] = colourid;
                }
            }
        }
        int smol_sleep = 0.1;
        ///gravity///
        if (interval_timer > delay) {
            //creating a backup
            for (int i = 0; i < 4; i++) {
                b[i] = a[i];
                a[i].y += 1;
                if (a[i].y == hei) {
                    init = false;
                    canvas[a[i].y][a[i].x] = colourid;
                }
            }

            for (int i = 0; i < 4; i++) {
                if (canvas[a[i].y][a[i].x] != 0) {
                    if (a[0].y < 0) { window.close(); }
                    for (int i = 0; i < 4; i++) {
                        {a[i] = b[i];
                        canvas[a[i].y][a[i].x] = colourid; } init = false;
                    }
                }
            }
            system("cls");
            for (int x = 0; x < hei; x++) {
                for (int y = 0; y < wid; y++) {
                    cout << canvas[x][y] << " ";
                }
                cout << endl;
            }
            cout << colourid;
            interval_timer = 0;
        }

        ///move///
        for (int i = 0; i < 4; i++) { b[i] = a[i]; }
        for (int i = 0; i < 4; i++) { a[i].x += x_velocity; }
        for (int i = 0; i < 4; i++) {
            if (canvas[a[i].y][a[i].x] != 0) {
                for (int i = 0; i < 4; i++) {
                    a[i] = b[i];
                }
            }
        }
        int k = hei - 1;
        for (int i = hei - 1; i > 0; i--) {
            int count = 0;
            for (int j = 0; j < wid; j++) {
                if (canvas[i][j]) { count++; }

                canvas[k][j] = canvas[i][j];
            }
            if (count < wid) k--;
        }


        ///setposition of tetris blocks on the window, and draw//
        window.clear(Color::White);
        for (int i = 0; i < 4; i++) {
            tetrisblocks.setPosition(a[i].x * 18, a[i].y * 18);
            if (colourid == 8) { tetrisblocks.setTextureRect(IntRect(0, 0, 18, 18)); }
            else { tetrisblocks.setTextureRect(IntRect(colourid * 18, 0, 18, 18)); }
            window.draw(tetrisblocks);
        };
        ///setposition of tetris blocks according to the "canvas" and draw
        for (int y = 0; y < hei; y++) {
            for (int x = 0; x < hei; x++) {
                if (canvas[y][x] == 0) { continue; }
                else {
                    if (canvas[y][x] == 8) { tetrisblocks.setTextureRect(IntRect(0, 0, 18, 18)); }
                    else { tetrisblocks.setTextureRect(IntRect(canvas[y][x] * 18, 0, 18, 18)); }
                    tetrisblocks.setPosition(x * 18, y * 18);
                    window.draw(tetrisblocks);
                }
            }
        }
        ///rotate//
        if (rotate) {
            for (int i = 0; i < 4; i++) { b[i] = a[i]; }
            for (int i = 0; i < 4; i++) {
                int x = a[1].y - a[i].y;
                int y = a[1].x - a[i].x;
                a[i].x = a[1].x + x;
                a[i].y = a[1].y - y;
            }
            for (int i = 0; i < 4; i++) { if (canvas[a[i].y][a[i].x] != 0) { for (int i = 0; i < 4; i++) { a[i] = b[i]; } } }
        }
        for (int i = 0; i < 4; i++) { if (a[i].x == -1 || a[i].x == 10)for (int i = 0; i < 4; i++) { a[i] = b[i]; } }
        rotate = false;
        window.display();
        x_velocity = 0;
        delay = 0.3;
    }
    return 0;
}
