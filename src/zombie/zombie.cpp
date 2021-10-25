#include "../../include/zombie.h"

void Zombies::cooldown() {
    if (counter>0) counter--;
    else counter = speed;
}

int Zombies::walk() {
    // path: 
    // 0: RD->LD->LU
    // 1: RD->RU->LU
    if (!block) {
        scounter--;
        if (scounter==0) {
            scounter = stride;
            return 1;
        }
        return 0;
    }
    return 1;// when blocked, always ready to get to another grid
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

Zombie::Zombie() {
    this->type = zombie;
    this->HP = zombie_table[this->type].HP;
    this->damage = 1;
    this->speed = zombie_table[this->type].speed;
    this->counter = this->speed;
    this->stride = zombie_table[this->type].stride;
    this->scounter = this->stride;

    this->path = rand()%g_path_num;
    this->block = false;
}

Conehead::Conehead() {
    this->type = conehead;
    this->HP = zombie_table[this->type].HP;
    this->damage = 1;
    this->speed = zombie_table[this->type].speed;
    this->counter = this->speed;
    this->stride = zombie_table[this->type].stride;
    this->scounter = this->stride;

    this->path = rand()%g_path_num;
    this->block = false;
}