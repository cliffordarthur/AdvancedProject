#include "../../include/plant.h"

Sunflower::Sunflower() {
    this->type = sunflower;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;

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
}

Spikeweed::Spikeweed() {
    this->type = spikeweed;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
}

Pumpkin::Pumpkin() {
    this->type = pumpkin;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
}