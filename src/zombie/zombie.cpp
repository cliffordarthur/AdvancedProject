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
    remain_grid--;
    return next;
}

void Zombies::suffer_poison() {
    if (poison>0 && poison%FPS==0) {HP-=max(1, total_HP/40);}
    poison--;
}

void Zombies::get_encouraged() {
    if (encourage==-1) {    
        damage *= 2;
    }
    if (encourage<FPS/10) encourage = FPS;
    
}

void Zombies::check_encourage(int a) {
    if (encourage>0) {
        HP -= (a*encourage/FPS);
        encourage--;
    }
    else if (encourage==0) {
        encourage = -1;
        damage /= 2;
    }
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
    this->encourage = -1;

    this->range = zombie_table[this->type].range;
    this->a_d = zombie_table[this->type].attack_direction;

    this->warlike = zombie_table[this->type].warlike;
    this->z_type = zombie_table[this->type].z_type;
}

Zombie::Zombie(int HP, int scounter, int path, int poison) {
    this->type = zombie;
    this->total_HP = zombie_table[this->type].HP;
    this->HP = HP;
    this->damage = zombie_table[this->type].damage;
    this->speed = zombie_table[this->type].speed;
    this->counter = this->speed;
    this->stride = zombie_table[this->type].stride;
    this->scounter = scounter;

    this->path = path;
    this->direction = 0;

    this->crazy = false;
    this->poison = poison;
    this->encourage = -1;

    this->range = zombie_table[this->type].range;
    this->a_d = zombie_table[this->type].attack_direction;

    this->warlike = zombie_table[this->type].warlike;
    this->z_type = zombie_table[this->type].z_type;
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
    this->encourage = -1;

    this->range = zombie_table[this->type].range;
    this->a_d = zombie_table[this->type].attack_direction;

    this->warlike = zombie_table[this->type].warlike;
    this->z_type = zombie_table[this->type].z_type;
}

Imp::Imp(int p) {
    this->type = imp;
    this->total_HP = zombie_table[this->type].HP;
    this->HP = this->total_HP;
    this->damage = zombie_table[this->type].damage;
    this->speed = zombie_table[this->type].speed;
    this->counter = this->speed;
    this->stride = zombie_table[this->type].stride;
    this->scounter = this->stride;

    this->path = p;
    this->direction = 0;

    this->crazy = false;
    this->poison = 0;
    this->encourage = -1;

    this->range = zombie_table[this->type].range;
    this->a_d = zombie_table[this->type].attack_direction;

    this->warlike = zombie_table[this->type].warlike;
    this->z_type = zombie_table[this->type].z_type;
}

Gargantuar::Gargantuar() {
    this->type = gargantuar;
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
    this->encourage = -1;

    this->range = zombie_table[this->type].range;
    this->a_d = zombie_table[this->type].attack_direction;

    this->warlike = zombie_table[this->type].warlike;
    this->z_type = zombie_table[this->type].z_type;
}

Necromancer::Necromancer() {
    this->type = necromancer;
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
    this->encourage = -1;

    this->range = zombie_table[this->type].range;
    this->a_d = zombie_table[this->type].attack_direction;

    this->warlike = zombie_table[this->type].warlike;
    this->z_type = zombie_table[this->type].z_type;
}

Catapult::Catapult() {
    this->type = catapult;
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
    this->encourage = -1;

    this->range = zombie_table[this->type].range;
    this->a_d = zombie_table[this->type].attack_direction;

    this->warlike = zombie_table[this->type].warlike;
    this->z_type = zombie_table[this->type].z_type;
}

Balloon::Balloon() {
    this->type = balloon;
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
    this->encourage = -1;

    this->range = zombie_table[this->type].range;
    this->a_d = zombie_table[this->type].attack_direction;

    this->warlike = zombie_table[this->type].warlike;
    this->z_type = zombie_table[this->type].z_type;
}

Bomber::Bomber() {
    this->type = bomber;
    this->total_HP = zombie_table[this->type].HP;
    this->HP = this->total_HP;
    this->damage = zombie_table[this->type].damage;
    this->speed = zombie_table[this->type].speed;
    this->counter = this->speed;
    this->stride = zombie_table[this->type].stride;
    this->scounter = this->stride;

    this->path = g_path_num+rand()%a_path_num;
    this->direction = 0;

    this->crazy = false;
    this->poison = 0;
    this->encourage = -1;

    this->range = zombie_table[this->type].range;
    this->a_d = zombie_table[this->type].attack_direction;

    this->warlike = zombie_table[this->type].warlike;
    this->z_type = zombie_table[this->type].z_type;
}

Frostwyrm::Frostwyrm() {
    this->type = frostwyrm;
    this->total_HP = zombie_table[this->type].HP;
    this->HP = this->total_HP;
    this->damage = zombie_table[this->type].damage;
    this->speed = zombie_table[this->type].speed;
    this->counter = this->speed;
    this->stride = zombie_table[this->type].stride;
    this->scounter = this->stride;

    this->path = g_path_num+rand()%a_path_num;
    this->direction = 0;

    this->crazy = false;
    this->poison = 0;
    this->encourage = -1;

    this->range = zombie_table[this->type].range;
    this->a_d = zombie_table[this->type].attack_direction;

    this->warlike = zombie_table[this->type].warlike;
    this->z_type = zombie_table[this->type].z_type;  

    this->freeze_time = 10;  
    this->freeze_r = 1;
}