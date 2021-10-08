#pragma once 
#include "common.h"
#include "map.h"
#include "plant.h"
#include "zombie.h"

class Game {
    int score;
    int sun;
    int cursor_x, cursor_y;
    Map map;
    Shop shop;
    WINDOW* win;
    bool lose, esc;
    clock_t begin, now;
    int time_counter;
public:
    Game();
    void init();
    void start();
    void gen_sun();
    void gen_zombie();
    void show_result();
    void refresh_map();
    void wait();
    void input(char ch);
    void check();
    // bool check_lose();
    void show_info();
    void show_help();
};