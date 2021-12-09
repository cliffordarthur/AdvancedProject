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
    int z_type;
    wxString info;
    wxString special;
}static zombie_table[] = {
    {     "zombie",      zombie,  100, FPS*1,   5,  10,  10, 0,  true,  true, z_ground, 
        "Regular garden-variety zombie.", 
        "---"}, 
    {   "conehead",    conehead,  200, FPS*1,   5,  20,  10, 0,  true,  true, z_ground,
        "His traffic cone headpiece makes him\ntwice as tough as normal zombies.",
        "---"},
    {        "imp",         imp,   50, FPS/2,   5,  10,  10, 0,  true,  true, z_ground, 
        "Imps are tiny zombies hurled by \nGargantuar deep into your defences.", 
        "---"}, 
    { "gargantuar",  gargantuar, 1000, FPS*5,   1, 100, 500, 1, false,  true, z_ground, 
        "Gargantuar is a gigantic zombie.", 
        "hurl imps"},
    {"necromancer", necromancer,  100, FPS*1,   5,  30,   1, 0,  true,  true, z_ground,
        "Necromancers cannot attack plants.\nInstead, they attack zombies and\nincrease their attack power",
        "---"},
    {   "catapult",    catapult,  200, FPS*1,   5,  30,  20, 2,  true,  true, z_ground, 
        "Catapult zombie operates heavy\nmachinery.", 
        "lobs stones at one plant"},
    
    {    "balloon",     balloon,  100, FPS/2,   5,  20,   0, 0,  true, false,    z_air,
        "Balloon zombie floats above the fray,\nimmune to most attacks.",
        "flying"},
    {     "bomber",      bomber,  200, FPS/2,   5,  30,  20, 0,  true, false,    z_air, 
        "Bomber can attack plants without\nstoping.",
        "flying"},
    {  "frostwyrm",   frostwyrm,  400, FPS*2,   3, 100,  10, 0,  true,  true,    z_air, 
        "Frostwyrm is a gigantic zombie in the\nair",
        "flying\nfreezing plants nearby"},
    {     "bullet",      bullet,    1,     1, FPS,   0,   1, 0,  true,  true, z_ground, "", ""},
};

const int zombie_num = sizeof(zombie_table)/sizeof(zombie_table[0])-1;

class Zombies{
protected:
    int type;
    int HP, total_HP;
    int damage, speed, stride; 
    int counter, scounter;
    int path;
    int direction;
    int remain_grid;

    bool crazy;
    int poison;
    int encourage;

    int range;
    bool a_d;

    bool warlike;
    int z_type;
public:
    Zombies():has_throw(-1){}
    void cooldown();
    int walk();
    int cross_grid(int i, int j, bool isbullet=false);
    
    int attack()const{return damage;}
    int show_type()const{return type;}
    int show_HP()const{return HP;}
    int show_t_HP()const{return total_HP;}
    int show_counter()const{return counter;}
    int show_scounter()const{return scounter;}
    int show_path()const {return path;}
    int show_direction()const {return direction;}
    int show_r_g()const {return remain_grid;}
    int show_range()const {return range;}
    bool show_a_d()const {return a_d;}
    bool show_warlike()const {return warlike;}
    int show_z_type()const {return z_type;}
    
    void be_attacked(int h) {HP-=h; if (HP<0) HP=0;}
    void set_total_HP(int H) {total_HP = H;}

    void be_poisoned(int p) {poison = p*FPS;}
    int show_poison()const {return poison;}
    void suffer_poison();

    virtual int show_encourage_r() const{return 0;}
    bool show_encourage() const{return encourage>=0;}
    void get_encouraged();
    void check_encourage(int a);

    void set_direction(int d, int r_g = -1){direction = d; if (r_g >= 0){remain_grid = r_g;}}
    void get_crazy(){if (!crazy) {crazy=true; speed/=2;}}
    bool show_crazy()const {return crazy;}

    int has_throw;//very UGLY!

    virtual int freeze()const{return 0;}
    virtual int show_freeze_r()const{return 0;}
    virtual int show_pos()const{return -1;}
    virtual void set_pos(int grid_id){}
};

class Zombie:public Zombies{
public:
    Zombie();
    Zombie(int HP, int scounter, int path, int poison);
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

class Necromancer:public Zombies{
    int encourage_r = 1;
public:
    Necromancer();
    int show_encourage_r() const {return encourage_r;}
};

class Catapult:public Zombies{
public:
    Catapult();
};

class Balloon:public Zombies{
public:
    Balloon();
};

class Bomber:public Zombies{
public:
    Bomber();
};

class Frostwyrm:public Zombies{
    int freeze_time;
    int freeze_r;
public:
    Frostwyrm();
    int freeze()const{return freeze_time;}
    int show_freeze_r()const{return freeze_r;}
};

class Bullet:public Zombies{
    int grid_x, grid_y;
public:
    Bullet(){}
    Bullet(int coord, int direction, int damage);
    int show_pos()const{return grid_x*MAP_COL+grid_y;}
    void set_pos(int grid_id){grid_x=grid_id/MAP_COL; grid_y=grid_id%MAP_COL;}
};