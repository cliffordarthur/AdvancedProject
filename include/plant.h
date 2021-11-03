#pragma once
#include "common.h"

struct Planttable{
    char name[32];
    int type;
    int HP;
    int costs;
    int CDtime;//shop
    int damage;
    int speed;//attack
    int range;
    bool attacked;//FIXME: in zombies
    int p_type;
    int stop_num;
}static plant_table[] = {
    {"    dryad",     dryad, 100, 150, 12*FPS,   5,  2*FPS,  1,  true, p_remote, ZOMBIE_NUM},
    {"sunflower", sunflower,  80,  50,  3*FPS,   0,  3*FPS,  0,  true,  p_other, ZOMBIE_NUM},
    {"   cherry",    cherry, 999, 150, 12*FPS, 500,  1*FPS,  1,  true,  p_other, ZOMBIE_NUM},
    {"  pumpkin",   pumpkin, 500, 125, 12*FPS,   0,      1,  0,  true,  p_other, ZOMBIE_NUM},
    {"  wallnut",   wallnut, 500,  75, 10*FPS,   0,      1,  0,  true,  p_melle, ZOMBIE_NUM},
    {"spikeweed", spikeweed,  30, 100,  5*FPS,  10,  1*FPS,  0, false,  p_melle,          0},
    {"   bamboo",    bamboo, 150, 100,  8*FPS,  20,  1*FPS,  2,  true,  p_melle, ZOMBIE_NUM},//
    {"  cabbage",   cabbage, 150, 100,  8*FPS,  20,  2*FPS,  3,  true,  p_melle, ZOMBIE_NUM},//
    {"   farmer",    farmer, 100, 450, 30*FPS,   0, 12*FPS,  1,  true, p_remote, ZOMBIE_NUM},
    {"      pea",       pea, 100, 100,  6*FPS,  10,  1*FPS,  2,  true, p_remote, ZOMBIE_NUM},//

    {"   shovel",    shovel,   1,   0,      0,   0,      0,  0, false,  p_other,          0},
};

const int plant_num = sizeof(plant_table)/sizeof(plant_table[0]);

class Plant{
protected:
    int type;
    int HP;
    int damage, speed, range;
    int counter;
    bool attacked; // FIXME:whether can be attacked
    
    int p_type;
    int stop_num;
public:
    Plant(){}
    virtual void suicide(){}
    virtual int poison(){return 0;}
    int find_zombie;
    void cooldown();
    void counter_plus(){counter++;}
    int attack()const{return damage;}
    int show_type()const{return type;}
    int show_HP()const{return HP;}
    int show_counter()const{return counter;}
    int show_range()const{return range;}
    bool show_attacked()const{return attacked;} 
    int show_s_n()const{return stop_num;}

    void be_attacked(int h){HP-=h; if (HP<0) HP=0;}
};

class Sunflower: public Plant{
    int sun;
public:
    Sunflower();
    int gen_sun();
};

class Wallnut: public Plant{
public:
    Wallnut();
};

class Spikeweed: public Plant{
public:
    Spikeweed();
};

class Pumpkin: public Plant{
public:
    Pumpkin();
};

class Farmer: public Plant{
public:
    Farmer();
};

class Dryad: public Plant{
public:
    Dryad();
    int poison();
};

class Cherry: public Plant{
public:
    Cherry();
    void suicide();
};

class Cabbage: public Plant{
public:
    Cabbage();
};

class Bamboo: public Plant{
public:
    Bamboo();
};

class Pea: public Plant{
public:
    Pea();
};

class Menu{
    int type;
    int costs, CDtime;
    int counter; 
public:
    Menu(){};
    void menu_set(int t, int c, int cd, int pt){type = t; costs = c; CDtime = cd; counter = 0;}
    void cooldown();
    void no_CDtime() {CDtime = 0; counter = 0;}
    friend class Shop;
};

class Shop{
    int cart;
    Menu menu[plant_num];
    int fail_to_cart;
public: 
    Shop();
    void add_to_cart(int i, int sun);
    void clear_cart();
    int show_cart()const {return cart;}
    int buy();
    int ret_sun(int r);
    void update();
    void no_CDtime(){for (int i = 0; i < plant_num; i++) menu[i].no_CDtime();}
};
//TODO: