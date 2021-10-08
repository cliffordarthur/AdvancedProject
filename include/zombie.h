#pragma once
#include "common.h"

struct Zombietable{
    char name[32];
    int type;
    int HP;
    // int damage; // always 1?
    int speed;  // take 'speed' time to attack or move
    int stride; // take 'stride' steps to move through a grid
}static zombie_table[] = {
    {"  zombie",   zombie, 10, 1*FPS, 5}, 
    {"conehead", conehead, 20, 1*FPS, 5},
};

const int zombie_num = sizeof(zombie_table)/sizeof(zombie_table[0]);

class Zombies{
protected:
    int type;
    int HP;
    int damage, speed, stride; 
    int counter, scounter;
    int path;
public:
    Zombies(){}
    bool find_plant;
    bool block;
    void cooldown();
    int attack()const{return damage;}
    int walk();
    int show_type()const{return type;}
    int show_HP()const{return HP;}
    int show_counter()const{return counter;}
    int show_path()const {return path;}

    void be_attacked(int h){HP-=h; if (HP<0) HP=0;};
};

class Zombie:public Zombies{
public:
    Zombie();
};

class Conehead:public Zombies{
public:
    Conehead();
};