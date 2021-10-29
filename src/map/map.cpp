#include "../../include/map.h"

int get_digits(int num) {
    if (num==0) return 1;
    int d = 0;
    while (num>0) {
        num/=10;
        d++;
    }
    return d;
}



Map::~Map() {
    // for (int i = 0; i < MAP_LINE; i++) {
    //     for (int j = 0; j  <MAP_COL; j++) {
    //         std::cout<<grids[i*MAP_COL+j].show_type()<<" ";
    //     }
    //     std::cout<<"\n";
    // }
    
    for (int i = 0; i < g_path_num; i++) {
        if (!g_path.size()) break;
        while (g_path[i]) {
            Path *p = g_path[i];
            g_path[i] = g_path[i]->next;
            delete p;
        }
    }
    for (int i = 0; i < a_path_num; i++) {
        if (!a_path.size()) break;
        while (a_path[i]) {
            Path *p = a_path[i];
            a_path[i] = a_path[i]->next;
            delete p;
        }
    }
}

void Map::set_Map(){
    can_buy = true;
    spec_type = -1;
    spec_coord = -1;
    grids.resize(MAP_LINE*MAP_COL);
    g_path.resize(g_path_num, nullptr);
    a_path.resize(a_path_num, nullptr);

    for (int i = 0; i < MAP_LINE; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            grids[i*MAP_COL+j].set_coordinate(i, j);
        }
    }
}

void Map::set_type(int t, int x1, int y1, int x2, int y2){
    int s, l;
    if (x1==x2) {
        s = (y1<y2)?y1:y2;
        l = (y1>y2)?y1:y2;
        for (int i = s; i <= l; i++) grids[x1*MAP_COL+i].set_type(t);
    }
    else if (y1==y2) {
        s = (x1<x2)?x1:x2;
        l = (x1>x2)?x1:x2;
        for (int i = s; i <= l; i++) grids[i*MAP_COL+y1].set_type(t);
    }
    else assert(0);
}

bool Map::find_enemy(bool is_plant, int r, int x, int y){//FIXME: 
    if (is_plant) {
        for (int i = 0; i < MAP_LINE; i++) {
            for (int j = 0; j < MAP_COL; j++) {
                if ((abs(x-i)+abs(y-j)<=r) && (grids[i*MAP_COL+j].z_num)) return true;
            }
        }
    }
    else {
        if (grids[x*MAP_COL+y].p_num) {
            if (!grids[x*MAP_COL+y].has_pumpkin && !grids[x*MAP_COL+y].plant_0->show_attacked()) return false;
            return true;
        }
        
    }
    return false;
}

void Map::update(int& sun, bool& lose, int& score) {
    for (int i = 0; i < MAP_LINE; i++) { 
        for (int j = 0; j < MAP_COL; j++) {
            if (spec_type==fort && spec_coord==i*MAP_COL+j && grids[i*MAP_COL+j].plant_p->show_HP() <= 0) {
                spec_type = d_fort;
            }
            if (grids[i*MAP_COL+j].plant_p) {
                if (grids[i*MAP_COL+j].plant_p->show_HP() <= 0) {
                    grids[i*MAP_COL+j].use_shovel();
                }
            }
            if (grids[i*MAP_COL+j].plant_0) {
                if (grids[i*MAP_COL+j].plant_0->show_HP() <= 0) {
                    grids[i*MAP_COL+j].use_shovel(1);
                }
                else{
                    grids[i*MAP_COL+j].plant_0->find_zombie = find_enemy(true, grids[i*MAP_COL+j].plant_0->show_range(), i, j);
                    grids[i*MAP_COL+j].plant_0->cooldown();
                    switch (grids[i*MAP_COL+j].plant_0->show_type()) {
                        case sunflower: {
                            if (grids[i*MAP_COL+j].plant_0->show_counter()==0) {
                                Sunflower* tmpsunflower = new Sunflower;
                                sun += tmpsunflower->gen_sun();
                                delete tmpsunflower;
                            } 
                            break;
                        }
                        case wallnut: {
                            break;
                        }
                        case spikeweed: {
                            if (grids[i*MAP_COL+j].plant_0->find_zombie && grids[i*MAP_COL+j].plant_0->show_counter()==0) {
                                for (int k = 0; k < ZOMBIE_NUM; k++) {
                                    if (grids[i*MAP_COL+j].zombies[k]) {
                                        grids[i*MAP_COL+j].zombies[k]->be_attacked(grids[i*MAP_COL+j].plant_0->attack());
                                    }
                                }
                            }
                        }
                        default: break;
                    }
                }
            }
            
            if (grids[i*MAP_COL+j].z_num) {
                int tmp_z_num = grids[i*MAP_COL+j].z_num;
                for (int k = 0, zz = 0; k < ZOMBIE_NUM && zz < tmp_z_num; k++) {
                    if (!grids[i*MAP_COL+j].zombies[k]) continue;
                    zz++;
                    if (grids[i*MAP_COL+j].zombies[k]->show_HP() <= 0) {
                        score += zombie_table[grids[i*MAP_COL+j].zombies[k]->show_type()].score;
                        grids[i*MAP_COL+j].free_zombie(k);
                    }
                    else {
                        if (spec_type==d_fort) {grids[i*MAP_COL+j].zombies[k]->get_crazy();}
                        grids[i*MAP_COL+j].zombies[k]->find_plant = find_enemy(false, 0, i, j);
                        grids[i*MAP_COL+j].zombies[k]->cooldown();
                        switch (grids[i*MAP_COL+j].zombies[k]->show_type()) {
                            case zombie:
                            case conehead:{
                                if (grids[i*MAP_COL+j].zombies[k]->show_counter()==0) {
                                    if (!grids[i*MAP_COL+j].zombies[k]->find_plant) {
                                        int ready = grids[i*MAP_COL+j].zombies[k]->walk();
                                        if (ready) {
                                            int next = grids[i*MAP_COL+j].zombies[k]->cross_grid(i, j);
                                            if (next<0) {lose = true; return;}
                                            Path *p = g_path[grids[i*MAP_COL+j].zombies[k]->show_path()];
                                            while (p) {
                                                if (p->x*MAP_COL+p->y==next) {
                                                    grids[i*MAP_COL+j].zombies[k]->set_direction(p->direction);
                                                    break;
                                                }
                                                p = p->next;
                                            }
                                            grids[next].add_zombie(grids[i*MAP_COL+j].zombies[k]);
                                            grids[i*MAP_COL+j].del_zombie(k);
                                            
                                        }
                                    }
                                    else {
                                        if (grids[i*MAP_COL+j].plant_p) {grids[i*MAP_COL+j].plant_p->be_attacked(grids[i*MAP_COL+j].zombies[k]->attack());}
                                        else {grids[i*MAP_COL+j].plant_0->be_attacked(grids[i*MAP_COL+j].zombies[k]->attack());}
                                    }
                                }
                                break;
                            }
                            default: break;
                        }
                    }
                }
            }
        }
    }
}

void Map::draw(int cursor_x, int cursor_y) {
    printnc(MAP_COL*(GRID_LEN+1)/2-2, WHITE_BLACK, "=");
    printc(YELLOW_BLACK, "MAP");
    printnc(MAP_COL*(GRID_LEN+1)/2-1, WHITE_BLACK, "=");
    printc(WHITE_BLACK, "\n\n");

    for (int mapline = 0; mapline <= MAP_LINE; mapline++){
        for (int i = 0, ii; i <= MAP_COL * (1 + GRID_LEN); i++) {
            if ((mapline==cursor_x || mapline==cursor_x+1) && cursor_y*(1+GRID_LEN)<=i && i<=(1+cursor_y)*(1+GRID_LEN)) {
                if (!can_buy) {printc(RED_BLACK, "+");}
                else printc(YELLOW_BLACK, "+");
            }
            else if (i%(1+GRID_LEN)==0) {
                int j = i/(1+GRID_LEN);
                if (((mapline < MAP_LINE) && (j < MAP_COL) && (grids[mapline*MAP_COL+j].show_type() != remote)) || 
                    ((mapline < MAP_LINE) && (j > 0) && (grids[mapline*MAP_COL+j-1].show_type() != remote))     ||
                    ((mapline > 0) && (j < MAP_COL) && (grids[(mapline-1)*MAP_COL+j].show_type() != remote))    ||
                    ((mapline > 0) && (j > 0) && (grids[(mapline-1)*MAP_COL+j-1].show_type() != remote))){
                    printc(GREEN_BLACK, "+");}
                else printc(WHITE_BLACK, "+");
            }
            else if ((mapline<MAP_LINE) && (grids[mapline*MAP_COL+i/(1+GRID_LEN)].show_type()!=remote) ||
              (mapline>0) && (grids[(mapline-1)*MAP_COL+i/(1+GRID_LEN)].show_type()!=remote)) {
                printc(GREEN_BLACK, "+");
            }
            else printc(WHITE_BLACK, "+");
        }

        printw("\n");
        if (mapline == MAP_LINE) break;

        for (int i = 0; i < GRID_HEI; i++) {
            for (int j = 0; j <= MAP_COL*(1+GRID_LEN); j++) {
                if (j%(1+GRID_LEN)==0) {
                    if (mapline == cursor_x && (j==(1+GRID_LEN)*cursor_y||j==(1+GRID_LEN)*(1+cursor_y))) {
                        if (!can_buy) {printc(RED_BLACK, "+");}
                        else printc(YELLOW_BLACK, "+");
                    }
                    else if ((j > 0 && grids[mapline*MAP_COL+j/(1+GRID_LEN)-1].show_type()!=remote)||
                             (j < MAP_COL*(1+GRID_LEN) && grids[mapline*MAP_COL+j/(1+GRID_LEN)].show_type()!=remote)) {
                        printc(GREEN_BLACK, "+");
                    }
                    else printc(WHITE_BLACK, "+");
                } // box
                else if (j%(1+GRID_LEN)==1) {
                    int tmp_y = j/(1+GRID_LEN);
                    if (i==0 && grids[mapline*MAP_COL+tmp_y].plant_0) {
                        int tmp_type = grids[mapline*MAP_COL+tmp_y].plant_0->show_type();
                        int HP = grids[mapline*MAP_COL+tmp_y].plant_0->show_HP();
                        printc(GREEN_BLACK, " %s %d", plant_table[tmp_type].name, HP);
                        j+=(strlen(plant_table[tmp_type].name)+get_digits(HP)+1);
                    }
                    else if (i==1 && grids[mapline*MAP_COL+tmp_y].plant_p) {
                        int HP = grids[mapline*MAP_COL+tmp_y].plant_p->show_HP();
                        printc(YELLOW_BLACK, " %s %d", plant_table[pumpkin].name, HP);
                        j+=(strlen(plant_table[pumpkin].name)+get_digits(HP)+1);
                    }
                    else if (i>=2 && grids[mapline*MAP_COL+tmp_y].z_num > i-2) {
                        int ii = i-2;
                        for (int k = 0; k < ZOMBIE_NUM; k++) {
                            if (grids[mapline*MAP_COL+tmp_y].zombies[k]){
                                if (!ii) {
                                    int tmp_type = grids[mapline*MAP_COL+tmp_y].zombies[k]->show_type();
                                    int HP = grids[mapline*MAP_COL+tmp_y].zombies[k]->show_HP();
                                    printc(BLUE_BLACK, " %s %d", zombie_table[tmp_type].name, HP);
                                    j+=(strlen(zombie_table[tmp_type].name)+get_digits(HP)+1);
                                    break;
                                }
                                ii--;
                            }
                        }
                    }
                    else printw(" ");
                }
                else printw(" ");
            }
            printw("\n");
        }
    }
    can_buy = true;
}
