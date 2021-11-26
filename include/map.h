#pragma once 
#include "common.h"
#include "plant.h"
#include "zombie.h"

class Grid{
    int type; /*melle or remote grids*/
    int x, y;
    Plant *plant_0;
    Plant *plant_p;
    // bool has_pumpkin;
    int p_num;

    int z_num;
    int a_z_num;
    Zombies *zombies[ZOMBIE_NUM];
public:
    Grid();
    bool has_pumpkin()const {return plant_p;}
    int show_plant_type()const;
    void set_type(int t){if (type!=g_z_base) type = t;} // is g_z_base necessary?
    int show_type() const {return type;}
    void set_coordinate(int x0, int y0);
    bool can_plant(int c, int tmp);
    void add_plant(int c);
    // int use_shovel();
    void use_shovel(int c);

    void add_zombie(Zombies* z);
    void del_zombie(int z);
    void free_zombie(int z);

    void add_fort();
    void cheat_kill();

    void paint(wxPaintDC &dc);
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
    
    int find_zombies(int x, int y, Plant *p);
    int find_plants(int x, int y, Zombies *z);
    // int find_plants(int r, int x, int y, int p, bool a_d);
    int find_next_n(int n, int x, int y, int p);
    
    void cheat_kill() {for (int i = 0; i < grids.size(); i++) grids[i].cheat_kill();}
    friend class Game;
    friend class Board;
};

const int pumpkin_x = GRID_SIZE/6;
const int pumpkin_y = GRID_SIZE/6;

const int p_other_r = GRID_SIZE/8;

const std::vector<std::vector<wxPoint> > PlantShape = { 
    {wxPoint(GRID_SIZE*7/12, GRID_SIZE*7/24), wxPoint(GRID_SIZE*11/12, GRID_SIZE*7/24), 
     wxPoint(GRID_SIZE*5/6, GRID_SIZE*1/24), wxPoint(GRID_SIZE*2/3, GRID_SIZE*1/24)},
    {wxPoint(GRID_SIZE*2/3, GRID_SIZE*7/24), wxPoint(GRID_SIZE*5/6, GRID_SIZE*7/24), 
     wxPoint(GRID_SIZE*11/12, GRID_SIZE*1/24), wxPoint(GRID_SIZE*7/12, GRID_SIZE*1/24)},    
    {wxPoint(GRID_SIZE*3/4, GRID_SIZE/6)},
};

const int p_lowest = GRID_SIZE*7/24;

const double area_0 = abs((PlantShape[0][1].x+PlantShape[0][2].x-PlantShape[0][0].x-PlantShape[0][3].x)*
                          (PlantShape[0][1].y-PlantShape[0][2].y)/2.0);

int distance(int x1, int y1, int x2, int y2);

double area(int x1, int y1, int x2, int y2, int x3, int y3);

double area(int x1, int y1, wxPoint wP2, wxPoint wP3);

wxColour hp_color(double p, wxColour wC1 = wxColour(0x00, 0xFF, 0x00), wxColour wC2 = wxColour(0xFF, 0x00, 0x00));

bool inShape(int x, int y, bool is_plant, int type);
