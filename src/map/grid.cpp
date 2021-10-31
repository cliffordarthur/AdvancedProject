#include "../../include/map.h"

Grid::Grid(){
    plant_0 = NULL;
    plant_p = NULL;
    has_pumpkin = false;
    p_num = 0;
    z_num = 0;
    type = remote;
    for (int i = 0; i < ZOMBIE_NUM; i++) zombies[i] = NULL;
}

void Grid::set_coordinate(int x0, int y0) {
    x = x0;
    y = y0;
}

bool Grid::can_plant(int c, int tmp) {
    if (c < 0) return false;
    else if (c == shovel) { 
        if ((p_num == 0)||(tmp == 1 && p_num == 1)) return false;
    }
    else if (c == pumpkin) {
        if (has_pumpkin) return false;
    }
    else if ((!has_pumpkin && p_num == 1) || (p_num > 1)) return false;
    else if ((type==melle && plant_table[c].p_type==p_remote) || (type==remote && plant_table[c].p_type==p_melle)) return false;
    return true;
}

void Grid::add_plant(int c) {
    if (c == pumpkin) {
        p_num++;
        has_pumpkin = true;
        Pumpkin *p = new Pumpkin;
        this->plant_p = p;
    }
    else if (c == sunflower) {
        p_num++;
        Sunflower *p = new Sunflower;
        this->plant_0 = p;
    }
    else if (c == wallnut) {
        p_num++;
        Wallnut *p = new Wallnut;
        this->plant_0 = p;
    }
    else if (c == spikeweed) {
        p_num++;
        Spikeweed *p = new Spikeweed;
        this->plant_0 = p;
    }
    else if (c == farmer) {
        p_num++;
        Farmer *p = new Farmer;
        this->plant_0 = p;
    }
    else if (c == dryad) {
        p_num++;
        Dryad *p = new Dryad;
        this->plant_0 = p;
    }
    else if (c == cherry) {
        p_num++;
        Cherry *p = new Cherry;
        this->plant_0 = p;
    }
    else if (c == bamboo) {
        p_num++;
        Bamboo *p = new Bamboo;
        this->plant_0 = p;
    }
    else if (c == cabbage) {
        p_num++;
        Cabbage *p = new Cabbage;
        this->plant_0 = p;
    }
    else if (c == pea) {
        p_num++;
        Pea *p = new Pea;
        this->plant_0 = p;
    }
    else return;//TODO: 
}

int Grid::use_shovel(){
    p_num--;
    if (has_pumpkin) {
        has_pumpkin = false;
        delete this->plant_p;
        this->plant_p = NULL;
        return pumpkin;
    }
    else {
        int tmp = this->plant_0->show_type();
        delete this->plant_0;
        this->plant_0 = NULL;
        return tmp;
    }
}

int Grid::use_shovel(int c) {
    p_num--;
    delete this->plant_0;
    this->plant_0 = NULL;
    return c;
}

void Grid::add_zombie(Zombies *z){
    if (z_num < ZOMBIE_NUM) {
        z_num++; 
        for (int i = 0; i < ZOMBIE_NUM; i++) {
            if (this->zombies[i]==NULL){
                this->zombies[i] = z;
                return;
            }
        }
    }
}

void Grid::del_zombie(int z) {
    zombies[z] = NULL;
    z_num--;
}

void Grid::free_zombie(int z) {
    delete zombies[z];
    zombies[z] = NULL;
    z_num--;
}

void Grid::add_fort() {
    p_num++;
    has_pumpkin = true;
    Pumpkin *p = new Pumpkin;
    p->be_attacked(-9*p->show_HP());
    this->plant_p = p;
}

void Grid::cheat_kill() {
    if (z_num) {
        for (int i = 0, zz = 0; i < ZOMBIE_NUM && zz < z_num; i++) {
            if (zombies[i]){
                zz++;
                zombies[i]->be_attacked(114514);
            }
        }
    }
}