#include "../../include/plant.h"

void Plant::cooldown() {
    if ((find_zombie>=0 ||type==sunflower||type==farmer||type==cherry)&&(counter>0)) counter--;
    else counter = speed;
}

void Plant::check_freeze() {
    assert(freeze>0);
    if (freeze>0) {
        freeze--;
    }
    if (freeze==0) {
        speed/=2;
    }
}

Sunflower::Sunflower() {
    this->type = sunflower;
    this->HP = plant_table[this->type].HP;
    this->total_HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->sun = 25;
    this->counter = this->speed;

    this->stop_num = plant_table[this->type].stop_num;
    this->a_target = plant_table[this->type].attack_target;
}

Wallnut::Wallnut() {
    this->type = wallnut;
    this->HP = plant_table[this->type].HP;
    this->total_HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->stop_num = plant_table[this->type].stop_num;
    this->a_target = plant_table[this->type].attack_target;
}

Spikeweed::Spikeweed() {
    this->type = spikeweed;
    this->HP = plant_table[this->type].HP;
    this->total_HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
    this->stop_num = plant_table[this->type].stop_num;
    this->a_target = plant_table[this->type].attack_target;
}

Pumpkin::Pumpkin() {
    this->type = pumpkin;
    this->HP = plant_table[this->type].HP;
    this->total_HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->stop_num = plant_table[this->type].stop_num;
    this->a_target = plant_table[this->type].attack_target;
}

Farmer::Farmer() {
    this->type = farmer;
    this->HP = plant_table[this->type].HP;
    this->total_HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
    this->stop_num = plant_table[this->type].stop_num;
    this->a_target = plant_table[this->type].attack_target;

    memset(this->tmp_order, -1, sizeof(this->tmp_order));
    this->order[0] = 2;
    this->order[1] = 3;
    this->order[2] = 4;
    this->order[3] = 1;
    this->order[4] = 0;
    // this->tmp_order[0] = 2;
    // this->tmp_order[1] = 3;
    // this->tmp_order[2] = 4;
    // this->tmp_order[3] = 1;
    // this->tmp_order[4] = 0;
}

Dryad::Dryad() {
    this->type = dryad;
    this->HP = plant_table[this->type].HP;
    this->total_HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
    this->stop_num = plant_table[this->type].stop_num;
    this->a_target = plant_table[this->type].attack_target;
}

/*poison for 15 secs*/
int Dryad::poison() {
    return 15;
}

Cherry::Cherry() {
    this->type = cherry;
    this->HP = plant_table[this->type].HP;
    this->total_HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
    this->stop_num = plant_table[this->type].stop_num;
    this->a_target = plant_table[this->type].attack_target;
}

void Cherry::suicide() {
    HP = 0;
}

Cabbage::Cabbage() {
    this->type = cabbage;
    this->HP = plant_table[this->type].HP;
    this->total_HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
    this->stop_num = plant_table[this->type].stop_num;
    this->a_target = plant_table[this->type].attack_target;
}

Bamboo::Bamboo() {
    this->type = bamboo;
    this->HP = plant_table[this->type].HP;
    this->total_HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
    this->stop_num = plant_table[this->type].stop_num;
    this->a_target = plant_table[this->type].attack_target;
}

Pea::Pea() {
    this->type = pea;
    this->HP = plant_table[this->type].HP;
    this->total_HP = plant_table[this->type].HP;
    this->damage = plant_table[this->type].damage;
    this->speed = plant_table[this->type].speed;
    this->range = plant_table[this->type].range;
    this->p_type = plant_table[this->type].p_type;
    this->find_zombie = -1;

    this->counter = this->speed;
    this->stop_num = plant_table[this->type].stop_num;
    this->a_target = plant_table[this->type].attack_target;
}