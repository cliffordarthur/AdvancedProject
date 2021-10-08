#include "../../include/plant.h"

void Plant::cooldown() {
    if ((find_zombie||type==sunflower)&&(counter>0)) counter--;
    else counter = speed;
}

Sunflower::Sunflower() {
    this->type = sunflower;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
    this->find_zombie = false;

    this->sun = 10;
    this->counter = this->speed;
}

int Sunflower::gen_sun() {
    return sun; 
}

Wallnut::Wallnut() {
    this->type = wallnut;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
    this->find_zombie = false;
}

Spikeweed::Spikeweed() {
    this->type = spikeweed;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
    this->find_zombie = false;

    this->counter = this->speed;
}

Pumpkin::Pumpkin() {
    this->type = pumpkin;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
    this->find_zombie = false;
}