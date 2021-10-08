#include "../../include/map.h"

int get_digits(int num) {
    int d = 0;
    while (num>0) {
        num/=10;
        d++;
    }
    return d;
}

Map::Map() {
    can_buy = true;
    for (int i = 0; i < MAP_LINE; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            grids[i][j].set_coordinate(i, j);
        }
    }
}

bool Map::find_enemy(bool is_plant, int r, int x, int y){
    if (is_plant) {
        for (int i = 0; i < MAP_LINE; i++) {
            for (int j = 0; j < MAP_COL; j++) {
                if ((abs(x-i)+abs(y-j)<=r) && (grids[i][j].z_num)) return true;
            }
        }
    }
    else {
        if (grids[x][y].p_num) return true;
    }
    return false;
}

void Map::update(int& sun, bool& lose) {
    for (int i = 0; i < MAP_LINE; i++) { 
        for (int j = 0; j < MAP_COL; j++) {
            if (grids[i][j].plant_p) {
                if (grids[i][j].plant_p->show_HP() <= 0) {
                    grids[i][j].use_shovel();
                }
            }
            else if (grids[i][j].plant_0) {
                if (grids[i][j].plant_0->show_HP() <= 0) {
                    grids[i][j].use_shovel();
                }
                else{
                    grids[i][j].plant_0->find_zombie = find_enemy(true, grids[i][j].plant_0->show_range(), i, j);
                    grids[i][j].plant_0->cooldown();
                    switch (grids[i][j].plant_0->show_type()) {
                        case sunflower: {
                            if (grids[i][j].plant_0->show_counter()==0) {
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
                            if (grids[i][j].plant_0->find_zombie && grids[i][j].plant_0->show_counter()==0) {
                                for (int k = 0; k < ZOMBIE_NUM; k++) {
                                    if (grids[i][j].zombies[k]) {
                                        grids[i][j].zombies[k]->be_attacked(grids[i][j].plant_0->attack());
                                    }
                                }
                            }
                        }
                        default: break;
                    }
                }
            }
            
            if (grids[i][j].z_num) {
                for (int k = 0; k < ZOMBIE_NUM; k++) {
                    if (!grids[i][j].zombies[k]) continue;
                    if (grids[i][j].zombies[k]->show_HP() <= 0) {
                        grids[i][j].free_zombie(k);
                    }
                    else {
                        grids[i][j].zombies[k]->find_plant = find_enemy(false, 0, i, j);
                        grids[i][j].zombies[k]->cooldown();
                        switch (grids[i][j].zombies[k]->show_type()) {
                            case zombie:
                            case conehead:{
                                if (grids[i][j].zombies[k]->show_counter()==0) {
                                    if (!grids[i][j].zombies[k]->find_plant) {
                                        int ready = grids[i][j].zombies[k]->walk();
                                        if (ready) {
                                            int ni, nj;
                                            if (grids[i][j].zombies[k]->show_path()==0) {                                                
                                                if (i == MAP_LINE-1 && j != 0) {ni = i; nj = j-1;}   
                                                else if (j == 0 && i != 0) {ni = i-1; nj = j;}
                                                else {lose = true; return;}                                                
                                            }
                                            else if (grids[i][j].zombies[k]->show_path()==1) {
                                                if (i != 0 && j == MAP_COL-1) {ni = i-1; nj = j;}
                                                else if (j != 0 && i == 0) {ni = i; nj = j-1;}
                                                else {lose = true; return;}
                                            }
                                            bool b = !grids[ni][nj].add_zombie(grids[i][j].zombies[k]);
                                            if (!b) grids[i][j].del_zombie(k);
                                            else {grids[i][j].zombies[k]->block = b;}
                                        }
                                    }
                                    else {
                                        if (grids[i][j].plant_p) {grids[i][j].plant_p->be_attacked(grids[i][j].zombies[k]->attack());}
                                        else {grids[i][j].plant_0->be_attacked(grids[i][j].zombies[k]->attack());}
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
    printc(WHITE_BLACK, "===========================================");
    printc(YELLOW_BLACK, "MAP");
    printc(WHITE_BLACK, "============================================\n\n");

    for (int mapline = 0; mapline <= MAP_LINE; mapline++){
        if ((mapline != cursor_x) && (mapline != (cursor_x+1))) {
            for (int i = 0; i <= MAP_COL*(1+GRID_LEN); i++) printw("+");
        }
        else {
            for (int i = 0; i <= MAP_COL*(1+GRID_LEN); i++) {
                if (cursor_y*(1+GRID_LEN)<=i && i<=(1+cursor_y)*(1+GRID_LEN)) {
                    if (!can_buy) {printc(RED_BLACK, "+");}
                    else printc(YELLOW_BLACK, "+");
                }
                else printw("+");
            }    
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
                    else printw("+");
                } // box
                else if (j%(1+GRID_LEN)==1) {
                    int tmp_y = j/(1+GRID_LEN);
                    if (i==0 && grids[mapline][tmp_y].plant_0) {
                        int tmp_type = grids[mapline][tmp_y].plant_0->show_type();
                        int HP = grids[mapline][tmp_y].plant_0->show_HP();
                        printc(GREEN_BLACK, " %s %d", plant_table[tmp_type].name, HP);
                        j+=(strlen(plant_table[tmp_type].name)+get_digits(HP)+1);
                    }
                    else if (i==1 && grids[mapline][tmp_y].plant_p) {
                        int HP = grids[mapline][tmp_y].plant_p->show_HP();
                        printc(YELLOW_BLACK, " %s %d", plant_table[pumpkin].name, HP);
                        j+=(strlen(plant_table[pumpkin].name)+get_digits(HP)+1);
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
