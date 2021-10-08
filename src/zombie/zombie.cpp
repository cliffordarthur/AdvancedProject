#include "../../include/zombie.h"

void Zombies::cooldown() {
    if (counter>0) counter--;
    else counter = speed;
}

int Zombies::walk() {
    // path
    // ^        <--
    // |<-- 0     |  1
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

Zombie::Zombie() {
    this->type = zombie;
    this->HP = zombie_table[this->type].HP;
    this->damage = 1;
    this->speed = zombie_table[this->type].speed;
    this->counter = this->speed;
    this->scounter = zombie_table[this->type].stride;

    this->path = rand()%2;
    this->block = false;
}

Conehead::Conehead() {
    this->type = conehead;
    this->HP = zombie_table[this->type].HP;
    this->damage = 1;
    this->speed = zombie_table[this->type].speed;
    this->counter = this->speed;
    this->scounter = zombie_table[this->type].stride;

    this->path = rand()%2;
}