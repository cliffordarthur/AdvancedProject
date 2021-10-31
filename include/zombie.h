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
}static zombie_table[] = {
    {"  zombie",   zombie, 100, 1*FPS, 5, 10, 10}, 
    {"conehead", conehead, 200, 1*FPS, 5, 20, 10},
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
    bool crazy;
    int poison;
public:
    Zombies(){}
    int find_plant;
    void cooldown();
    int attack()const{return damage;}
    int walk();
    int show_type()const{return type;}
    int show_HP()const{return HP;}
    int show_counter()const{return counter;}
    int show_path()const {return path;}
    
    void be_attacked(int h) {HP-=h; if (HP<0) HP=0;}
    void set_total_HP(int H) {total_HP = H;}

    void be_poisoned(int p) {poison = p*FPS;}
    int show_poison()const {return poison;}
    void suffer_poison();

    int cross_grid(int i, int j);
    void set_direction(int d){direction = d;}
    void get_crazy(){if (!crazy) {crazy=true; speed/=2;}}
};

class Zombie:public Zombies{
public:
    Zombie();
};

class Conehead:public Zombies{
public:
    Conehead();
};