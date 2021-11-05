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
    int p_type;
    int stop_num;
    int attack_target;
}static plant_table[] = {
    {"    dryad",     dryad, 100, 150, 12*FPS,   5,  2*FPS,  1, p_remote, ZOMBIE_NUM,   z_both},

    {"sunflower", sunflower,  80,  50,  3*FPS,   0,  3*FPS,  0,  p_other, ZOMBIE_NUM, z_ground},
    {"   cherry",    cherry, 999, 150, 12*FPS, 500,  1*FPS,  1,  p_other, ZOMBIE_NUM,   z_both},
    {"  pumpkin",   pumpkin, 500, 125, 12*FPS,   0,      1,  0,  p_other, ZOMBIE_NUM, z_ground},

    {"  wallnut",   wallnut, 500,  75, 10*FPS,   0,      1,  0,  p_melle, ZOMBIE_NUM, z_ground},
    {"spikeweed", spikeweed,  30, 100,  5*FPS,  10,  1*FPS,  0,  p_melle,          0, z_ground},
    {"   bamboo",    bamboo, 150, 100,  8*FPS,  20,  1*FPS,  0,  p_melle, ZOMBIE_NUM, z_ground},//
    {"  cabbage",   cabbage, 150, 100,  8*FPS,  10,  FPS/2,  0,  p_melle, ZOMBIE_NUM,   z_both},//

    {"   farmer",    farmer, 100, 450, 30*FPS,   0, 12*FPS,  1, p_remote, ZOMBIE_NUM, z_ground},
    {"      pea",       pea, 100, 100,  6*FPS,  10,  1*FPS,  2, p_remote, ZOMBIE_NUM, z_ground},//

    {"   shovel",    shovel,   1,   0,      0,   0,      0,  0, p_other,          0, z_ground},
};

const int plant_num = sizeof(plant_table)/sizeof(plant_table[0]);

class Plant{
protected:
    int type;
    int HP;
    int damage, speed, range;
    int counter;
    
    int p_type;
    int stop_num;
    int a_target;
    int freeze;
public:
    Plant():freeze(0){}
    virtual void suicide(){}
    virtual int poison(){return 0;}
    virtual int gen_sun()const{return 0;}
    int find_zombie;
    void cooldown();
    void counter_plus(){counter++;}
    int attack()const{return damage;}
    int show_type()const{return type;}
    int show_HP()const{return HP;}
    int show_counter()const{return counter;}
    int show_range()const{return range;}
    int show_s_n()const{return stop_num;}
    int show_a_t()const{return a_target;}

    void be_attacked(int h){HP-=h; if (HP<0) HP=0;}
    void be_freezed(int f){if (!freeze) {speed*=2;} freeze = FPS*f;}
    int show_freeze()const {return freeze;}
    void check_freeze();
};

class Sunflower: public Plant{
    int sun;
public:
    Sunflower();
    int gen_sun()const{return sun;}
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