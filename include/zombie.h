#pragma once
#include "common.h"

struct Zombietable{
    char name[32];
    int type;
    int HP;
    int speed;  // take 'speed' time to attack or move
    int stride; // take 'stride' steps to move through a grid
    int score;
    int damage;
    int range;
    bool attack_direction; // false: forward; true: around
    bool warlike;
}static zombie_table[] = {
    {"    zombie",     zombie,  100, FPS*1, 5,  10,  10, 0,  true, true}, 
    {"  conehead",   conehead,  200, FPS*1, 5,  20,  10, 0,  true, true},
    {"       imp",        imp,   50, FPS/2, 5,  10,  10, 0,  true, true}, 
    {"gargantuar", gargantuar, 1000, FPS*5, 1, 100, 500, 1, false, true},
};

const int zombie_num = sizeof(zombie_table)/sizeof(zombie_table[0]);

class Zombies{
protected:
    int type;
    int HP, total_HP;;
    int damage, speed, stride; 
    int counter, scounter;
    int path;
    int direction;
    int remain_grid;

    bool crazy;
    int poison;
    bool encourage;

    int range;
    bool a_d;

    bool warlike;

public:
    Zombies():has_throw(-1){}
    void cooldown();
    int walk();
    int cross_grid(int i, int j);
    
    int attack()const{return damage;}
    int show_type()const{return type;}
    int show_HP()const{return HP;}
    int show_t_HP()const{return total_HP;}
    int show_counter()const{return counter;}
    int show_path()const {return path;}
    int show_r_g()const {return remain_grid;}
    int show_range()const {return range;}
    bool show_a_d()const {return a_d;}
    bool show_warlike()const {return warlike;}
    
    void be_attacked(int h) {HP-=h; if (HP<0) HP=0;}
    void set_total_HP(int H) {total_HP = H;}

    void be_poisoned(int p) {poison = p*FPS;}
    int show_poison()const {return poison;}
    void suffer_poison();

    void set_direction(int d, int r_g = -1){direction = d; if (r_g >= 0){remain_grid = r_g;}}
    void get_crazy(){if (!crazy) {crazy=true; speed/=2;}}

    int has_throw;//very UGLY!
};

class Zombie:public Zombies{
public:
    Zombie();
};

class Conehead:public Zombies{
public:
    Conehead();
};

class Imp:public Zombies{
public:
    Imp(int p);
};

class Gargantuar:public Zombies{
public:
    Gargantuar();
};