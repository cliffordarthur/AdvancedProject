#pragma once 
#include "common.h"
#include "plant.h"
#include "zombie.h"

class Grid{
    int type; /*melle or remote grids*/
    int x, y;
    Plant *plant_0;
    Plant *plant_p;
    bool has_pumpkin;
    int p_num;

    int z_num;
    Zombies *zombies[ZOMBIE_NUM];
public:
    Grid();
    void set_type(int t){type = t;}
    void set_coordinate(int x0, int y0);
    bool can_plant(int c);
    void add_plant(int c);
    int use_shovel();
    int use_shovel(int c);

    bool add_zombie(Zombies* z);
    void del_zombie(int z);
    void free_zombie(int z);
    friend class Map;
};

struct Path{
    int type;
    int x, y;
    int direction;
};

class Map{
    Grid grids[MAP_LINE][MAP_COL];
    Path path[128];
public:
    bool can_buy;
    Map();
    void update(int& sun, bool& lose, int& score);
    void draw(int cursor_x, int cursor_y);
    bool find_enemy(bool is_plant, int r, int x, int y);
    friend class Game;
};