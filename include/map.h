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

struct Path{
    int x, y;
    int direction;
    Path* next;
    
    Path(const int& x = 0, const int& y = 0, const int& direction = 0, Path* next = nullptr):
        x(x), y(y), direction(direction), next(next) {}
};

class Map{
    std::vector<Grid> grids;
    std::vector<Path*> g_path;
    std::vector<Path*> a_path;
    int spec_type;
    int spec_coord;
public:
    bool can_buy;
    Map(){}
    ~Map();
    void set_Map();
    void set_type(int t, int x1, int y1, int x2, int y2);
    void update(int& sun, bool& lose, int& score);
    void draw(int cursor_x, int cursor_y);
    bool find_enemy(bool is_plant, int r, int x, int y);
    void cheat_kill() {for (int i = 0; i < grids.size(); i++) grids[i].cheat_kill();}
    friend class Game;
};