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
    bool attacked;
}static plant_table[] = {
    {"   shovel",    shovel,  1,   0,      0, 0,     0,  0, false},
    {"sunflower", sunflower,  8,  50,  3*FPS, 0, 3*FPS, 99,  true},
    {"  wallnut",   wallnut, 50,  75, 10*FPS, 0,     1,  0,  true},
    {"spikeweed", spikeweed,  1, 100,  5*FPS, 1, 1*FPS,  0, false},
    {"  pumpkin",   pumpkin, 50, 125, 12*FPS, 0,     1,  0,  true},

};

const int plant_num = sizeof(plant_table)/sizeof(plant_table[0]);

class Plant{
protected:
    int type;
    int HP;
    int damage, speed, range;
    int counter;
    bool attacked; //whether can be attacked
public:
    Plant(){};
    bool find_zombie;
    void cooldown();
    int attack()const{return damage;}
    int show_type()const{return type;}
    int show_HP()const{return HP;}
    int show_counter()const{return counter;}
    int show_range()const{return range;}
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

// class Shovel: public Plant{
// public:
//     Shovel(){};
// };

class Menu{
    int type;
    int costs, CDtime;
    int counter;    
public:
    Menu(){};
    void menu_set(int t, int c, int cd){type = t; costs = c; CDtime = cd; counter = 0;}
    void cooldown();
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
};
//TODO: