#include "../../include/map.h"

Grid::Grid(){
    plant_0 = NULL;
    plant_p = NULL;
    // has_pumpkin = false;
    p_num = 0;
    z_num = 0;
    a_z_num = 0;
    type = remote;
    for (int i = 0; i < ZOMBIE_NUM; i++) zombies[i] = NULL;
}

int Grid::show_plant_type() const{
    if (!plant_0) {
        return -1;
    }
    return plant_0->show_type();
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
        if (has_pumpkin()) return false;
    }
    else if ((!has_pumpkin() && p_num == 1) || (p_num > 1)) return false;
    else if ((type==melle && plant_table[c].p_type==p_remote) || (type==remote && plant_table[c].p_type==p_melle)) return false;
    return true;
}

void Grid::add_plant(int c) {
    if (c == pumpkin) {
        p_num++;
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
}

void Grid::use_shovel(int c) {
    p_num--;
    if (c == pumpkin) {
        assert(has_pumpkin());
        delete this->plant_p;
        this->plant_p = NULL;
    }
    else {
        assert(c==show_plant_type());
        delete this->plant_0;
        this->plant_0 = NULL;
    }
}

void Grid::add_zombie(Zombies *z){
    if (z_num < ZOMBIE_NUM) {
        z_num++; 
        if (z->show_z_type()==z_air) a_z_num++;
        for (int i = 0; i < ZOMBIE_NUM; i++) {
            if (this->zombies[i]==NULL){
                this->zombies[i] = z;
                return;
            }
        }
    }
}

void Grid::del_zombie(int z) {
    if (zombies[z]->show_z_type()==z_air) a_z_num--;
    zombies[z] = NULL;
    z_num--;
}

void Grid::free_zombie(int z) {
    if (zombies[z]->show_z_type()==z_air) a_z_num--;
    delete zombies[z];
    zombies[z] = NULL;
    z_num--;
}

void Grid::add_fort() {
    p_num++;
    // has_pumpkin = true;
    Pumpkin *p = new Pumpkin;
    p->be_attacked(-9*p->show_HP());
    p->set_total_HP(p->show_HP());
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

void Grid::paint(wxPaintDC &dc) {
    double percent;
    int init_x = MAP_BEGIN_X+y*GRID_SIZE, init_y = MAP_BEGIN_Y+x*GRID_SIZE;
    if (has_pumpkin()) {
        percent = plant_p->show_HP()/plant_p->show_t_HP();
        dc.SetBrush(hp_color(percent));
        dc.DrawCircle(wxPoint(init_x+pumpkin_x, init_y+pumpkin_y), p_other_r);
    }

    int tmp =  show_plant_type();
    if (tmp >= 0){
        percent = plant_0->show_HP()/plant_0->show_t_HP();
        dc.SetBrush(hp_color(percent));
        if (plant_table[tmp].p_type == p_other) {
            dc.DrawCircle(wxPoint(init_x, init_y)+PlantShape[p_other][0], p_other_r);
        }
        else {
            wxPoint points[] = {
                wxPoint(init_x, init_y)+PlantShape[plant_table[tmp].p_type][0], 
                wxPoint(init_x, init_y)+PlantShape[plant_table[tmp].p_type][1],
                wxPoint(init_x, init_y)+PlantShape[plant_table[tmp].p_type][2],
                wxPoint(init_x, init_y)+PlantShape[plant_table[tmp].p_type][3],
            };
            dc.DrawPolygon(4, points);
        }
    }

    // paint enemy
}