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

void Map::update(int& sun){
    for (int i = 0; i < MAP_LINE; i++) { 
        for (int j = 0; j < MAP_COL; j++) {
            if (grids[i][j].plant_0) {
                switch (grids[i][j].plant_0->show_type()){
                    case sunflower: {
                        if (grids[i][j].plant_0->show_counter()==0){
                            Sunflower* tmpsunflower = new Sunflower;
                            sun += tmpsunflower->gen_sun();
                            delete tmpsunflower;
                        }
                        grids[i][j].plant_0->cooldown();
                        break;
                    }
                    default: break;
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
