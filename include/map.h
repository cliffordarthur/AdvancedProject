#pragma once 
#include "plant.h"
#include "zombie.h"

struct Info {
    wxString name = "--",
             race = "--",
             HP = "--/--", 
             F = "--", 
             P = "--", 
             C = "--", 
             E = "--", 
             recharge = "--",
             info = "",
             special = "";
};

class Grid{
    int type; /*melle or remote grids*/
    int x, y;
    int choose;
    int p_num;
    int z_num;
    int a_z_num;
    Plant *plant_0;
    Plant *plant_p;
    Zombies *zombies[ZOMBIE_NUM];
    std::vector<int> show_a_z, show_g_z;
public:
    Grid();
    bool has_pumpkin()const {return plant_p;}
    int show_plant_type()const;
    int show_zombie_type()const;
    void set_type(int t){if (type!=g_z_base) type = t;}
    int show_type() const {return type;}
    void set_coordinate(int x0, int y0);
    bool can_plant(int c, int tmp);
    void add_plant(int c);
    void use_shovel(int c);

    int add_zombie(Zombies* z);
    void del_zombie(int z);
    void free_zombie(int z);

    void add_fort();
    void cheat_kill();

    void paint(wxPaintDC &dc);
    void set_choose(int c) {choose = c;}
    int show_choose() const {return choose;}
    Info show_info() const;

    int show_order(int i=0)const {if (plant_0) {return plant_0->show_strategy(i);} else {return -1;}}
    int show_tmp_order(int i=0)const {if (plant_0) {return plant_0->show_tmp_strategy(i);} else {return -1;}}
    void set_order(){plant_0->set_strategy();}
    void set_tmp_order(int i, int j=0){plant_0->set_tmp_strategy(i, j);}
    int show_g_z_num()const{return z_num-a_z_num;}
    int show_a_z_num()const{return a_z_num;}
    int return_zumbie_order(int i, bool g = true)const {if (g) {return show_g_z[i];}else {return show_a_z[i];}}
    int show_choose_direction();
    friend class Map;
};

class Map{
    std::vector<Grid> grids;
    std::vector<Bullet*> bullets;
    std::vector< std::vector<int> > paths;
    std::vector<int> start;
    std::vector<int> length;

    int spec_type;
    int spec_coord;
    int cursor_choose;
public:
    bool can_buy;
    Map(){}
    ~Map(){}
    void set_Map();
    void update(int& sun, bool& lose, int& score);
    void draw(int cursor_x, int cursor_y);
    
    int find_zombies(int x, int y, Plant *p);
    int find_plants(int x, int y, Zombies *z);
    int find_next_n(int n, int x, int y, int p);
    
    void cheat_kill() {for (int i = 0; i < grids.size(); i++) grids[i].cheat_kill();}
    void farmer_plant_pumpkin(int grid_id);

    friend class Game;
    friend class Board;
};

const wxString directions[5] = {"up", "left", "self", "right", "down"};

const int pumpkin_x = GRID_SIZE/6;
const int pumpkin_y = GRID_SIZE/6;

const int p_other_r = GRID_SIZE/8;

const int bullet_r = GRID_SIZE/30;

const std::vector<std::vector<wxPoint> > PlantShape = { 
    {wxPoint(GRID_SIZE*7/12, GRID_SIZE*7/24), wxPoint(GRID_SIZE*11/12, GRID_SIZE*7/24), 
     wxPoint(GRID_SIZE*5/6, GRID_SIZE*1/24), wxPoint(GRID_SIZE*2/3, GRID_SIZE*1/24)},
    {wxPoint(GRID_SIZE*2/3, GRID_SIZE*7/24), wxPoint(GRID_SIZE*5/6, GRID_SIZE*7/24), 
     wxPoint(GRID_SIZE*11/12, GRID_SIZE*1/24), wxPoint(GRID_SIZE*7/12, GRID_SIZE*1/24)},    
    {wxPoint(GRID_SIZE*3/4, GRID_SIZE/6)},
};

const int p_lowest = GRID_SIZE*7/24;
const int a_lowest = GRID_SIZE*7/10;

const std::vector<std::vector<wxPoint> > ZombieShape = {
    {wxPoint(GRID_SIZE/20, GRID_SIZE*4/5), wxPoint(GRID_SIZE*13/120, GRID_SIZE*3/20), wxPoint(GRID_SIZE*19/120, 0)},
    {wxPoint(GRID_SIZE/15, GRID_SIZE*2/3), wxPoint(GRID_SIZE*7/30, GRID_SIZE*2/3), 
     wxPoint(GRID_SIZE*3/20, GRID_SIZE*21/40), wxPoint(GRID_SIZE*7/30, 0)},
};

const double area_0 = abs((PlantShape[0][1].x+PlantShape[0][2].x-PlantShape[0][0].x-PlantShape[0][3].x)*
                          (PlantShape[0][1].y-PlantShape[0][2].y)/2.0);

int distance(int x1, int y1, int x2, int y2);

double area(int x1, int y1, int x2, int y2, int x3, int y3);

double area(int x1, int y1, wxPoint wP2, wxPoint wP3);

wxColour hp_color(double p, wxColour wC1 = wxColour(GREEN1), wxColour wC2 = wxColour(RED));

int inShape(int x, int y, bool is_plant, int type);

void DrawStrategy(wxPaintDC &dc, int type, int x, int y, int size, int order[]);