#include "../../include/zombie.h"

void Zombies::cooldown() {
    if (counter>0) counter--;
    else counter = speed;
}

int Zombies::walk() {
    scounter--;
    if (scounter==0) {
        scounter = stride;
        return 1;
    }
    return 0;
}

int Zombies::cross_grid(int i, int j) {
    int next = i*MAP_COL+j;
    switch (direction){
        case diup: {next-=MAP_COL; break;}
        case didown: {next+=MAP_COL; break;}
        case dileft: {next-=1; break;}
        case diright: {next+=1; break;}
        case diend: {next = -1; break;}
        default: assert(0);
    }
    return next;
}

void Zombies::suffer_poison() {
    if (poison>0 && poison%FPS==0) {HP-=max(1, total_HP/40);}
    poison--;
}

Zombie::Zombie() {
    this->type = zombie;
    this->total_HP = zombie_table[this->type].HP;
    this->HP = this->total_HP;
    this->damage = zombie_table[this->type].damage;
    this->speed = zombie_table[this->type].speed;
    this->counter = this->speed;
    this->stride = zombie_table[this->type].stride;
    this->scounter = this->stride;

    this->path = rand()%g_path_num;
    this->direction = 0;
    this->crazy = false;
    this->poison = 0;
}

Conehead::Conehead() {
    this->type = conehead;
    this->total_HP = zombie_table[this->type].HP;
    this->HP = this->total_HP;
    this->damage = zombie_table[this->type].damage;
    this->speed = zombie_table[this->type].speed;
    this->counter = this->speed;
    this->stride = zombie_table[this->type].stride;
    this->scounter = this->stride;

    this->path = rand()%g_path_num;
    this->direction = 0;
    this->crazy = false;
    this->poison = 0;
}