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
    wxString info;
    wxString strategy;
}static plant_table[] = {
    { "sunflower", sunflower,  80,  50,  3*FPS,   0,  3*FPS,  0,  p_other, ZOMBIE_NUM, z_ground, 
        "Sunflowers are essential for you to \nproduce extra sun. Try planting as \nmany as you can!",
        "---"},
    {    "cherry",    cherry, 999, 150, 12*FPS, 500,  1*FPS,  1,  p_other, ZOMBIE_NUM,   z_both, 
        "Cherry Bombs can blow up all zom-\nbies in an area. They have a short \nfuse so plant them near zombies.",
        "---"},
    {   "pumpkin",   pumpkin, 500, 125, 12*FPS,   0,      1,  0,  p_other, ZOMBIE_NUM, z_ground, 
        "Pumpkins protect plants that are \nwithin their shells.",
        "can be planted over another \nplant."},

    {   "wallnut",   wallnut, 500,  75, 10*FPS,   0,      1,  0,  p_melle, ZOMBIE_NUM, z_ground, 
        "Wall-nuts have hard shells which you \ncan use to protect your other plants.",
        "---"},
    { "spikeweed", spikeweed,  30, 100,  5*FPS,  10,  1*FPS,  0,  p_melle,          0, z_ground, 
        "Spikeweeds can hurt any zombies \nthat step on them.", 
        "cannot be hurt by zombies \n(except for gargantuar)."},
    {    "bamboo",    bamboo, 150, 100,  8*FPS,  20,  1*FPS,  0,  p_melle, ZOMBIE_NUM, z_ground,
        "Bamboos can hurt a zombie in the\n grid.", 
        "---"},
    {   "cabbage",   cabbage, 150, 100,  8*FPS,  10,  FPS/2,  0,  p_melle, ZOMBIE_NUM,   z_both, 
        "Cabbage-pults hurl cabbages at the \nenemy in the air.", 
        "---"},

    {    "farmer",    farmer, 100, 450, 30*FPS,   0, 12*FPS,  1, p_remote, ZOMBIE_NUM, z_ground, 
        "Farmers can plant pumpkins nearby.", 
        "---"},
    {"peashooter",       pea, 100, 100,  6*FPS,  10,  1*FPS,  8, p_remote, ZOMBIE_NUM, z_ground, 
        "Peashooters are your first line of \ndefense. THey shoot peas at attack-\ning zombies.", 
        "---"},
    {     "dryad",     dryad, 100, 150, 12*FPS,   5,  2*FPS,  1, p_remote, ZOMBIE_NUM,   z_both, 
        "Dryads can attack any lane and \nshoot at air zombies too.", 
        "poison the target."},

    {    "shovel",    shovel,   1,   0,      0,   0,      0,  0,  p_other,          0, z_ground, 
        "---", 
        "---"},
};

const int plant_num = sizeof(plant_table)/sizeof(plant_table[0]);

class Plant{
protected:
    int type;
    int HP, total_HP;
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
    virtual int show_strategy(int i=0)const{return -1;}
    virtual int show_tmp_strategy(int i=0)const{return -1;}
    virtual void set_strategy(){}
    virtual void set_tmp_strategy(int i, int j=0){}
    int find_zombie;
    void cooldown();
    void counter_plus(){counter++;}
    int attack()const{return damage;}
    int show_type()const{return type;}
    int show_HP()const{return HP;}
    int show_t_HP()const{return total_HP;}
    int show_counter()const{return counter;}
    int show_range()const{return range;}
    int show_s_n()const{return stop_num;}
    int show_a_t()const{return a_target;}

    void set_total_HP(int H) {total_HP = H;}
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
    int order[5];
    int tmp_order[5];
public:
    Farmer();
    int show_strategy(int i=0)const{return order[i];}
    int show_tmp_strategy(int i=0)const{return tmp_order[i];}
    void set_strategy(){for (int i = 0; i < 5; i++) order[i]=tmp_order[i];}
    void set_tmp_strategy(int i, int j=0){tmp_order[i] = j;}
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
    int direction;
    int tmp_direction;
public:
    Pea();
    int show_strategy(int i=0)const{return direction;}
    int show_tmp_strategy(int i=0)const{return tmp_direction;}
    void set_strategy() {direction = tmp_direction;}
    void set_tmp_strategy(int i, int j=0){tmp_direction = i;}
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
    bool show_CD(int i) const {return menu[i].counter;}
    int show_fail();
    void add_to_cart(int i, int sun);
    void clear_cart();
    int show_cart()const {return cart;}
    int buy();
    int ret_sun(int r);
    void update();
    void no_CDtime(){for (int i = 0; i < plant_num; i++) menu[i].no_CDtime();}
};