#include "../../include/plant.h"

void Plant::cooldown() {
    if ((find_zombie>=0 ||type==sunflower||type==farmer||type==cherry)&&(counter>0)) counter--;
    else counter = speed;
}

Sunflower::Sunflower() {
    this->type = sunflower;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

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
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;
}

Spikeweed::Spikeweed() {
    this->type = spikeweed;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
}

Pumpkin::Pumpkin() {
    this->type = pumpkin;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;
}

Farmer::Farmer() {
    this->type = farmer;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
}

Dryad::Dryad() {
    this->type = dryad;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
}

int Dryad::poison() {
    return 15;
}

Cherry::Cherry() {
    this->type = cherry;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
}

void Cherry::suicide() {
    HP = 0;
}

Cabbage::Cabbage() {
    this->type = cabbage;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
}

Bamboo::Bamboo() {
    this->type = bamboo;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
}

Pea::Pea() {
    this->type = pea;
    this->HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->attacked = plant_table[this->type].attacked;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
}