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
    //         // std::cout<<grids[i*MAP_COL+j].show_type()<<" ";
    //         std::cout<<paths[1][i*MAP_COL+j]<<" ";
    //     }
    //     std::cout<<"\n";
    // }
}

void Map::set_Map(){
    can_buy = true;
    spec_type = -1;
    spec_coord = -1;
    grids.resize(MAP_LINE*MAP_COL);
    paths.resize(g_path_num+a_path_num);
    for (int i = 0; i < g_path_num+a_path_num; i++) {
        paths[i].resize(MAP_LINE*MAP_COL, -1);
    }
    start.resize(g_path_num+a_path_num, -1);
    length.resize(g_path_num+a_path_num, -1);

    for (int i = 0; i < MAP_LINE; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            grids[i*MAP_COL+j].set_coordinate(i, j);
        }
    }
}

/*Return the coordinates of zombie within range r. Return -1 if zombie does not exists*/
int Map::find_zombies(int x, int y, Plant *p) {
    int r = p->show_range();
    for (int rr = 0; rr <= r; rr++){
        for (int i = -rr; i <= rr; i++) {
            if (x + i < 0 || x + i >= MAP_LINE) continue;
            for (int j = -rr; j <= rr; j++) {
                if (y + j < 0 || y + j >= MAP_COL || (-rr+1<=i && i<=rr-1 && -rr+1<=j && j<=rr-1)) continue;
                // if (grids[(x+i)*MAP_COL+y+j].z_num) return (x+i)*MAP_COL+y+j;   
                int target = (x+i)*MAP_COL+y+j;
                if ((p->show_a_t() == z_air && grids[target].a_z_num) || 
                    (p->show_a_t() == z_both && grids[target].z_num) ||
                    (p->show_a_t() == z_ground && (grids[target].z_num>grids[target].a_z_num))){
                    return target;
                }
            }
        }
    }
    return -1;
}

/*Return the coordinates of plant within range. Return -1 if plant does not exists*/
int Map::find_plants(int x, int y, Zombies *z) {
    int target;
    if (z->show_a_d()) {
        for (int rr = 0; rr <= z->show_range(); rr++){
            for (int i = -rr; i <= rr; i++) {
                if (x + i < 0 || x + i >= MAP_LINE) continue;
                for (int j = -rr; j <= rr; j++) {
                    if (y + j < 0 || y + j >= MAP_COL || (-rr+1<=i && i<=rr-1 && -rr+1<=j && j<=rr-1)) continue;
                    target = (x+i)*MAP_COL+y+j;
                    if (grids[target].p_num && (grids[target].has_pumpkin || z->show_type() == gargantuar || grids[target].plant_0->show_s_n())) return target;   
                }
            }
        }
    }
    else {
        int r = z->show_range();
        while (r>=0) {
            target = x*MAP_COL+y;
            if (grids[target].p_num && (grids[target].has_pumpkin || z->show_type() == gargantuar || grids[target].plant_0->show_s_n())) return target;//FIXME: garg... can attack spikeweed!
            int d = paths[z->show_path()][target];
            switch (d) {
                case    diup: x--; break;
                case  didown: x++; break;        
                case  dileft: y--; break;
                case diright: y++; break;
                default: return -1;
            }
            r--;
        }
    }
    return -1;
}

int Map::find_next_n(int n, int x, int y, int p) {
    assert(n>=0);
    int target;
    while (n >= 0) {
        target =  x*MAP_COL+y;
        int d = paths[p][target];
        switch (d) {
            case    diup: x--; break;
            case  didown: x++; break;
            case  dileft: y--; break;
            case diright: y++; break;
            default: return target;
        }
        n--;
    }
    return target;
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
                if (grids[i*MAP_COL+j].plant_0->show_freeze()) {grids[i*MAP_COL+j].plant_0->check_freeze();}
                if (grids[i*MAP_COL+j].plant_0->show_HP() <= 0) {
                    grids[i*MAP_COL+j].use_shovel(1);
                }
                else{
                    grids[i*MAP_COL+j].plant_0->find_zombie = find_zombies(i, j, grids[i*MAP_COL+j].plant_0);
                    grids[i*MAP_COL+j].plant_0->cooldown();
                    switch (grids[i*MAP_COL+j].plant_0->show_type()) {
                        case sunflower: {
                            if (grids[i*MAP_COL+j].plant_0->show_counter()==0) {
                                sun += grids[i*MAP_COL+j].plant_0->gen_sun();
                            } 
                            break;
                        }
                        case wallnut: {
                            break;
                        }
                        case spikeweed: {
                            if (grids[i*MAP_COL+j].plant_0->find_zombie >= 0 && grids[i*MAP_COL+j].plant_0->show_counter()==0) {
                                for (int k = 0, zz = 0; k < ZOMBIE_NUM && zz < grids[i*MAP_COL+j].z_num-grids[i*MAP_COL+j].a_z_num; k++) {
                                    if (grids[i*MAP_COL+j].zombies[k] && grids[i*MAP_COL+j].zombies[k]->show_z_type()!=z_air) {
                                        zz++;
                                        grids[i*MAP_COL+j].zombies[k]->be_attacked(grids[i*MAP_COL+j].plant_0->attack());
                                    }
                                }
                            }
                            break;
                        }
                        case farmer: {
                            if (grids[i*MAP_COL+j].plant_0->show_counter()==0) {
                                if (!grids[i*MAP_COL+j].plant_p) {
                                    grids[i*MAP_COL+j].add_plant(pumpkin);
                                }
                                else if (j < MAP_COL-1 && grids[i*MAP_COL+j+1].show_type()!=g_z_base && !grids[i*MAP_COL+j+1].plant_p) {
                                    grids[i*MAP_COL+j+1].add_plant(pumpkin);
                                }
                                else if (i < MAP_LINE-1 && grids[(i+1)*MAP_COL+j].show_type()!=g_z_base && !grids[(i+1)*MAP_COL+j].plant_p) {
                                    grids[(i+1)*MAP_COL+j].add_plant(pumpkin);
                                }
                                else if (j > 0 && grids[i*MAP_COL+j-1].show_type()!=g_z_base && !grids[i*MAP_COL+j-1].plant_p) {
                                    grids[i*MAP_COL+j-1].add_plant(pumpkin);
                                }
                                else if (i > 0 && grids[(i-1)*MAP_COL+j].show_type()!=g_z_base && !grids[(i-1)*MAP_COL+j].plant_p) {
                                    grids[(i-1)*MAP_COL+j].add_plant(pumpkin);
                                }
                                else grids[i*MAP_COL+j].plant_0->counter_plus();
                            }
                            break;
                        }
                        case dryad: {
                            if (grids[i*MAP_COL+j].plant_0->find_zombie >= 0 && grids[i*MAP_COL+j].plant_0->show_counter()==0) {
                                int target = grids[i*MAP_COL+j].plant_0->find_zombie;
                                for (int k = 0; k < ZOMBIE_NUM; k++) {
                                    if (grids[target].zombies[k]) {
                                        grids[target].zombies[k]->be_attacked(grids[i*MAP_COL+j].plant_0->attack());
                                        grids[target].zombies[k]->be_poisoned(grids[i*MAP_COL+j].plant_0->poison());
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                        case bamboo:
                        case cabbage:
                        case pea: {
                            if (grids[i*MAP_COL+j].plant_0->find_zombie >= 0 && grids[i*MAP_COL+j].plant_0->show_counter()==0) {
                                int target = grids[i*MAP_COL+j].plant_0->find_zombie;
                                for (int k = 0; k < ZOMBIE_NUM; k++) {
                                    if (grids[target].zombies[k]) {
                                        grids[target].zombies[k]->be_attacked(grids[i*MAP_COL+j].plant_0->attack());
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                        case cherry: {
                            if (grids[i*MAP_COL+j].plant_0->show_counter()==0) {
                                int range = grids[i*MAP_COL+j].plant_0->show_range();
                                for (int _i = -range; _i <= range; _i++) {
                                    if (i+_i < 0 || i+_i >= MAP_LINE) continue;
                                    for (int _j = -range; _j <= range; _j++) {
                                        if (j+_j < 0 || j+_j >= MAP_COL) continue;
                                        for (int k = 0, zz = 0; k < ZOMBIE_NUM && zz < grids[(i+_i)*MAP_COL+j+_j].z_num; k++) {
                                            if (grids[(i+_i)*MAP_COL+j+_j].zombies[k]) {
                                                zz++;
                                                grids[(i+_i)*MAP_COL+j+_j].zombies[k]->be_attacked(grids[i*MAP_COL+j].plant_0->attack());
                                            }
                                        }
                                    }
                                }
                                grids[i*MAP_COL+j].plant_0->suicide();
                            }
                            break;
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
                    grids[i*MAP_COL+j].zombies[k]->check_encourage(zombie_table[necromancer].damage);
                    if (grids[i*MAP_COL+j].zombies[k]->show_poison()) {grids[i*MAP_COL+j].zombies[k]->suffer_poison();}

                    if (grids[i*MAP_COL+j].zombies[k]->show_HP() <= 0) {
                        score += zombie_table[grids[i*MAP_COL+j].zombies[k]->show_type()].score;
                        grids[i*MAP_COL+j].free_zombie(k);
                    }
                    else if (grids[i*MAP_COL+j].zombies[k]->show_type() == balloon &&
                            (grids[i*MAP_COL+j].zombies[k]->show_t_HP() > grids[i*MAP_COL+j].zombies[k]->show_HP() ||
                             grids[i*MAP_COL+j].zombies[k]->show_r_g() == 0)){
                        Zombies *o = grids[i*MAP_COL+j].zombies[k];
                        Zombie *z = new Zombie(o->show_HP(), o->show_scounter(), o->show_path(), o->show_poison());
                        z->set_direction(paths[z->show_path()][i*MAP_COL+j], o->show_r_g());
                        grids[i*MAP_COL+j].add_zombie(z);
                        grids[i*MAP_COL+j].free_zombie(k);
                    }
                    else if (grids[i*MAP_COL+j].zombies[k]->show_type() == gargantuar &&
                        grids[i*MAP_COL+j].zombies[k]->has_throw==-1 &&
                        ((grids[i*MAP_COL+j].zombies[k]->show_HP() <= grids[i*MAP_COL+j].zombies[k]->show_t_HP()/2) || 
                        (grids[i*MAP_COL+j].zombies[k]->show_r_g() <= length[grids[i*MAP_COL+j].zombies[k]->show_path()]/2))) {
                        grids[i*MAP_COL+j].zombies[k]->has_throw = FPS;
                    }
                    else if (grids[i*MAP_COL+j].zombies[k]->has_throw>0) {grids[i*MAP_COL+j].zombies[k]->has_throw--;}
                    else {
                        if (spec_type==d_fort) {grids[i*MAP_COL+j].zombies[k]->get_crazy();}
                        int target = find_plants(i, j, grids[i*MAP_COL+j].zombies[k]);
                        grids[i*MAP_COL+j].zombies[k]->cooldown();
                        
                        /* The zombie can move or attack */
                        if (grids[i*MAP_COL+j].zombies[k]->show_counter()==0) {
                            if (grids[i*MAP_COL+j].zombies[k]->has_throw==0) {
                                Imp *z = new Imp(grids[i*MAP_COL+j].zombies[k]->show_path());
                                int ttarget = find_next_n(length[z->show_path()]/4, i, j, z->show_path());
                                z->set_direction(paths[z->show_path()][ttarget]);
                                grids[ttarget].add_zombie(z);
                                grids[i*MAP_COL+j].zombies[k]->has_throw = -2;
                                continue;
                            }
                            /* The zombie finds plants */
                            if (target != -1 || grids[i*MAP_COL+j].zombies[k]->show_type()==necromancer) {
                                switch (grids[i*MAP_COL+j].zombies[k]->show_type()) {
                                    case frostwyrm: {
                                        if (grids[target].plant_p) {grids[target].plant_p->be_attacked(grids[i*MAP_COL+j].zombies[k]->attack());}
                                        else if (grids[target].plant_0) {grids[target].plant_0->be_attacked(grids[i*MAP_COL+j].zombies[k]->attack());}

                                        int r = grids[i*MAP_COL+j].zombies[k]->show_freeze_r();assert(r==1);
                                        for (int _i = -r; _i <= r; _i++) {
                                            if (i + _i < 0 || i + _i >= MAP_LINE) continue;
                                            for (int _j = -r; _j <= r; _j++) {
                                                if (j + _j < 0 || j + _j >= MAP_COL) continue;
                                                int f_target = (i+_i)*MAP_COL+j+_j;
                                                if (grids[f_target].plant_0) grids[f_target].plant_0->be_freezed(grids[i*MAP_COL+j].zombies[k]->freeze());
                                            }
                                        }
                                        break;
                                    }
                                    case necromancer: {
                                        int r = grids[i*MAP_COL+j].zombies[k]->show_encourage_r();
                                        for (int _i = -r; _i <= r; _i++) {
                                            if (i + _i < 0 || i + _i >= MAP_LINE) continue;
                                            for (int _j = -r; _j <= r; _j++) {
                                                if (j + _j < 0 || j + _j >= MAP_COL) continue;
                                                int e_target = (i+_i)*MAP_COL+j+_j;
                                                for (int kk = 0, zz = 0; kk < ZOMBIE_NUM && zz < grids[e_target].z_num; kk++) {
                                                    if (grids[e_target].zombies[kk]) {
                                                        zz++;
                                                        if (grids[e_target].zombies[kk]->show_type() != necromancer && grids[e_target].zombies[kk]->show_type() != balloon) {
                                                            grids[e_target].zombies[kk]->get_encouraged();
                                                        }
                                                    }
                                                }
                                                
                                            }
                                        }
                                        break;
                                    }
                                    case gargantuar: {
                                        if (grids[target].plant_p) {grids[target].plant_p->be_attacked(grids[i*MAP_COL+j].zombies[k]->attack());}
                                        if (grids[target].plant_0) {grids[target].plant_0->be_attacked(grids[i*MAP_COL+j].zombies[k]->attack());}
                                        break;
                                    }
                                    default: {
                                        if (grids[target].plant_p) {grids[target].plant_p->be_attacked(grids[i*MAP_COL+j].zombies[k]->attack());}
                                        else if (grids[target].plant_0) {grids[target].plant_0->be_attacked(grids[i*MAP_COL+j].zombies[k]->attack());}
                                        break;
                                    }
                                }
                            }
                            /* The zombie is not warlike or cannot find plants, so he moved */
                            if (!grids[i*MAP_COL+j].zombies[k]->show_warlike() || target == -1) {
                                int ready = grids[i*MAP_COL+j].zombies[k]->walk();
                                if (ready) {
                                    int next = grids[i*MAP_COL+j].zombies[k]->cross_grid(i, j);
                                    if (next<0) {
                                        if (grids[i*MAP_COL+j].zombies[k]->show_z_type() == z_ground) {
                                            lose = true;
                                            return;
                                        }
                                        else {
                                            grids[i*MAP_COL+j].free_zombie(k);
                                        }
                                    }
                                    else {
                                        grids[i*MAP_COL+j].zombies[k]->set_direction(paths[grids[i*MAP_COL+j].zombies[k]->show_path()][next]);
                                        grids[next].add_zombie(grids[i*MAP_COL+j].zombies[k]);
                                        grids[i*MAP_COL+j].del_zombie(k);   
                                    }                               
                                }
                            }
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
                        switch (plant_table[tmp_type].p_type){
                            case p_melle: printc(GREEN_BLACK, " %s ", plant_table[tmp_type].name);break;
                            case p_remote: printc(CYAN_BLACK, " %s ", plant_table[tmp_type].name);break;
                            default: printc(YELLOW_BLACK, " %s ", plant_table[tmp_type].name);break;
                        }
                        if (grids[mapline*MAP_COL+tmp_y].plant_0->show_freeze()) {printc(BLUE_BLACK, "%d", HP);}
                        else printc(WHITE_BLACK, "%d", HP);
                        j+=(strlen(plant_table[tmp_type].name)+get_digits(HP)+1);
                    }
                    else if (i==1 && grids[mapline*MAP_COL+tmp_y].plant_p) {
                        int HP = grids[mapline*MAP_COL+tmp_y].plant_p->show_HP();
                        printc(YELLOW_BLACK, " %s ", plant_table[pumpkin].name);
                        printc(WHITE_BLACK, "%d", HP);
                        j+=(strlen(plant_table[pumpkin].name)+get_digits(HP)+1);
                    }
                    else if (i>=2 && grids[mapline*MAP_COL+tmp_y].z_num > i-2) {
                        int ii = i-2;
                        for (int k = 0; k < ZOMBIE_NUM; k++) {
                            if (grids[mapline*MAP_COL+tmp_y].zombies[k]){
                                if (!ii) {
                                    int tmp_type = grids[mapline*MAP_COL+tmp_y].zombies[k]->show_type();
                                    int HP = grids[mapline*MAP_COL+tmp_y].zombies[k]->show_HP();
                                    if (grids[mapline*MAP_COL+ tmp_y].zombies[k]->show_z_type()==z_air) {printc(CYAN_BLACK, " %s ", zombie_table[tmp_type].name);}
                                    else printc(BLUE_BLACK, " %s ", zombie_table[tmp_type].name);
                                    
                                    if (grids[mapline*MAP_COL+ tmp_y].zombies[k]->show_crazy()) {printc(RED_BLACK, "%d", HP);}
                                    else if (grids[mapline*MAP_COL+ tmp_y].zombies[k]->show_poison()) {printc(GREEN_BLACK, "%d", HP);}
                                    else if (grids[mapline*MAP_COL+tmp_y].zombies[k]->show_encourage()) {printc(MAGENTA_BLACK, "%d", HP)}
                                    else printc(WHITE_BLACK, "%d", HP);
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
