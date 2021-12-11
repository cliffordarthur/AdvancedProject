#include "../../include/map.h"

Grid::Grid(){
    plant_0 = NULL;
    plant_p = NULL;
    choose = -3; // pumpkin: -2; plant: -1; zombie: 0 to n
    p_num = 0;
    z_num = 0;
    a_z_num = 0;
    type = remote;
    for (int i = 0; i < ZOMBIE_NUM; i++) zombies[i] = NULL;
    show_a_z.resize(MAX_SHOW_A_NUM, -1);
    show_g_z.resize(MAX_SHOW_G_NUM, -1);
}

int Grid::show_plant_type() const{
    if (!plant_0) {
        return -1;
    }
    return plant_0->show_type();
}

int Grid::show_zombie_type() const{
    return zombies[choose]->show_type();
}

void Grid::set_coordinate(int x0, int y0) {
    x = x0;
    y = y0;
}

bool Grid::can_plant(int c, int tmp) {
    if (type == g_z_base) return false;
    else if (c < 0) return false;
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

int Grid::add_zombie(Zombies *z){
    if (z_num < ZOMBIE_NUM) {
        z_num++; 
        if (z->show_z_type()==z_air) a_z_num++;
        for (int i = 0; i < ZOMBIE_NUM; i++) {
            if (this->zombies[i]==NULL){
                this->zombies[i] = z;
                return i;
            }
        }
    }
    return -3;
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
        percent = (double)(plant_p->show_HP())/plant_p->show_t_HP();
        dc.SetBrush(hp_color(percent));
        dc.DrawCircle(wxPoint(init_x+pumpkin_x, init_y+pumpkin_y), p_other_r);
    }

    int tmp = show_plant_type();
    if (tmp >= 0){
        percent = (double)(plant_0->show_HP())/plant_0->show_t_HP();
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

    if (z_num) {
        for (int i = 0, g = 0, a = 0; i < ZOMBIE_NUM; i++) {
            if (zombies[i]) {
                if (zombies[i]->show_z_type() == z_ground && g < MAX_SHOW_G_NUM) {
                    percent = (double)(zombies[i]->show_HP())/zombies[i]->show_t_HP();
                    dc.SetBrush(hp_color(percent));
                    dc.DrawRectangle(wxPoint(init_x, init_y)+ZombieShape[0][0]+g*ZombieShape[0][2], wxSize(ZombieShape[0][1].x, ZombieShape[0][1].y));
                    show_g_z[g] = i;
                    g++;
                }
                else if (zombies[i]->show_z_type() == z_air && a < MAX_SHOW_A_NUM) {
                    percent = (double)(zombies[i]->show_HP())/zombies[i]->show_t_HP();
                    dc.SetBrush(hp_color(percent));
                    wxPoint points[] = {
                        wxPoint(init_x, init_y)+ZombieShape[1][0]+a*ZombieShape[1][3], 
                        wxPoint(init_x, init_y)+ZombieShape[1][1]+a*ZombieShape[1][3], 
                        wxPoint(init_x, init_y)+ZombieShape[1][2]+a*ZombieShape[1][3], 
                    };
                    dc.DrawPolygon(3, points);
                    show_a_z[a] = i;
                    a++;
                }
            }
        }
    }
}

void Grid::flash(wxPaintDC &dc, int k, int colour){
    int init_x = MAP_BEGIN_X+y*GRID_SIZE, init_y = MAP_BEGIN_Y+x*GRID_SIZE;
    dc.SetBrush(wxColour(colour));
    if (k == -2) {    
        dc.DrawCircle(wxPoint(init_x+pumpkin_x, init_y+pumpkin_y), p_other_r);
    }
    else if (k == -1) {
        int tmp = show_plant_type();
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
    else {
        if (!zombies[k]) {}
        else if (zombies[k]->show_z_type()==z_ground) {
            for (int i = 0; i < show_g_z.size(); i++) {
                if (show_g_z[i] == k) {
                    dc.DrawRectangle(wxPoint(init_x, init_y)+ZombieShape[0][0]+i*ZombieShape[0][2], wxSize(ZombieShape[0][1].x, ZombieShape[0][1].y));
                    break;
                }
            }
        }
        else {
            for (int i = 0; i < show_a_z.size(); i++) {
                if (show_a_z[i] == k) {
                    wxPoint points[] = {
                        wxPoint(init_x, init_y)+ZombieShape[1][0]+i*ZombieShape[1][3], 
                        wxPoint(init_x, init_y)+ZombieShape[1][1]+i*ZombieShape[1][3], 
                        wxPoint(init_x, init_y)+ZombieShape[1][2]+i*ZombieShape[1][3], 
                    };
                    dc.DrawPolygon(3, points);
                    break;
                }
            }
        }
    }
}

Info Grid::show_info() const {
    Info i;
    switch (choose) {
        case -3: {
            break;
        }
        case -2: {
            if (has_pumpkin()) {
                i.name = wxString("pumpkin");
                i.race = wxString("Plants");
                i.HP = wxString::Format("%d/%d", plant_p->show_HP(), plant_p->show_t_HP());
                i.recharge = wxString::Format("%d", plant_table[pumpkin].CDtime/FPS);
                i.info = plant_table[pumpkin].info;
                i.special = plant_table[pumpkin].strategy;
            }
            break;
        }
        case -1: {
            if (show_plant_type()>=0) {
                i.name = wxString(plant_table[plant_0->show_type()].name);
                i.race = wxString("Plants");
                i.HP = wxString::Format("%d/%d", plant_0->show_HP(), plant_0->show_t_HP());
                i.F = wxString::Format("%.2fs", (double)plant_0->show_freeze()/FPS);
                i.recharge = wxString::Format("%d", plant_table[plant_0->show_type()].CDtime/FPS);
                i.info = plant_table[plant_0->show_type()].info;
                i.special = wxString("");
                if (show_plant_type()==farmer) {
                    for (int h = 0; h < 4; h++) {
                        i.special += directions[show_order(h)]+"->";
                    }
                    i.special += directions[show_order(4)];
                }
                else if (show_plant_type()==pea) {
                    if (show_order()==diup) {i.special="up";}
                    if (show_order()==didown) {i.special="down";}
                    if (show_order()==dileft) {i.special="left";}
                    if (show_order()==diright) {i.special="right";}
                }
                else i.special = plant_table[plant_0->show_type()].strategy;
            }
            break;
        }
        default: {
            i.name = wxString(zombie_table[zombies[choose]->show_type()].name);
            i.race = wxString("Zombies");
            i.HP = wxString::Format("%d/%d", zombies[choose]->show_HP(), zombies[choose]->show_t_HP());
            i.C = zombies[choose]->show_crazy()?"Yes":"No";
            i.E = (zombies[choose]->show_encourage()>0)?"Yes":"No";
            i.P = wxString::Format("%.2fs", (double)zombies[choose]->show_poison()/FPS);
            i.recharge = wxString::Format("%d%%", 100*zombies[choose]->show_scounter()/zombie_table[zombies[choose]->show_type()].stride);
            i.info = zombie_table[zombies[choose]->show_type()].info;
            i.special = zombie_table[zombies[choose]->show_type()].special;
            if (zombies[choose]->show_type()==gargantuar) {
                if (zombies[choose]->has_throw == -1) {i.special = "imp is not hurled";}
                else if(zombies[choose]->has_throw > 0) {i.special = "imp is being hurled";}
                else {i.special = "imp has been hurled";}
            }
        }
    }
    return i;
}

int Grid::show_choose_direction() {
    if (choose==-1 && show_plant_type()==pea) {return plant_0->show_strategy();}
    else if (choose>=0 && zombies[choose]->show_type()==gargantuar) {return zombies[choose]->show_direction();}
    else return -1;
}