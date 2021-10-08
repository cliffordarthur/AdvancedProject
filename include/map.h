#pragma once 
#include "common.h"
#include "plant.h"
#include "zombie.h"

class Grid{
    int x, y;
    Plant *plant_0;
    Plant *plant_p;
    bool has_pumpkin;
    int p_num;

    int z_num;
    Zombies *zombies[ZOMBIE_NUM];
public:
    Grid();
    void set_coordinate(int x0, int y0);
    bool can_plant(int c);
    void add_plant(int c);
    int use_shovel();

    bool add_zombie(Zombies* z);
    void del_zombie(int z);
    void free_zombie(int z);
    friend class Map;
};

class Map{
    Grid grids[MAP_LINE][MAP_COL];
public:
    bool can_buy;
    Map();
    void update(int& sun, bool& lose);
    void draw(int cursor_x, int cursor_y);
    bool find_enemy(bool is_plant, int r, int x, int y);
    friend class Game;
};