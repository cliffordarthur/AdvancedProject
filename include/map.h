#pragma once 
#include "common.h"
#include "plant.h"

class Grid{
    int x, y;
    Plant *plant_0;
    Plant *plant_p;
    bool has_pumpkin;
    int p_num;

    int z_num;
public:
    Grid();
    void set_coordinate(int x0, int y0);
    bool can_plant(int c);
    void add_plant(int c);
    int use_shovel();
    friend class Map;
};

class Map{
    Grid grids[MAP_LINE][MAP_COL];
public:
    bool can_buy;
    Map();
    void update(int& sun);
    void draw(int cursor_x, int cursor_y);
    friend class Game;
};