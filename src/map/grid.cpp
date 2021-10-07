#include "../../include/map.h"

Grid::Grid(){
    plant_0 = NULL;
    plant_p = NULL;
    has_pumpkin = false;
    p_num = 0;
    z_num = 0;
}

void Grid::set_coordinate(int x0, int y0) {
    x = x0;
    y = y0;
}

bool Grid::can_plant(int c) {
    if (c < 0) return false;
    else if (c == shovel) { 
        if (p_num == 0) return false;
    }
    else if (c == pumpkin) {
        if (has_pumpkin) return false;
    }
    else if ((!has_pumpkin && p_num == 1) || (p_num > 1)) return false;
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
