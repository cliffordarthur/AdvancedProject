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
    void set_type(int t){if (type!=g_z_base) type = t;}
    int show_type() const {return type;}
    void set_coordinate(int x0, int y0);
    bool can_plant(int c, int tmp);
    void add_plant(int c);
    int use_shovel();
    int use_shovel(int c);

    void add_zombie(Zombies* z);
    void del_zombie(int z);
    void free_zombie(int z);

    void add_fort();
    void cheat_kill();
    friend class Map;
};

class Map{
    std::vector<Grid> grids;
    std::vector< std::vector<int> > paths;
    std::vector<int> start;
    std::vector<int> length;

    int spec_type;
    int spec_coord;
public:
    bool can_buy;
    Map(){}
    ~Map();
    void set_Map();
    void update(int& sun, bool& lose, int& score);
    void draw(int cursor_x, int cursor_y);
    
    // int find_enemy(bool is_plant, int r, int x, int y);
    int find_zombies(int r, int x, int y);
    int find_plants(int r, int x, int y, int p, bool a_d);
    int find_next_n(int n, int x, int y, int p);
    
    void cheat_kill() {for (int i = 0; i < grids.size(); i++) grids[i].cheat_kill();}
    friend class Game;
};