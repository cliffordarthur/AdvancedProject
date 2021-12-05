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

Map::~Map() {}

void Map::set_Map(){
    can_buy = true;
    spec_type = -1;
    spec_coord = -1;
    cursor_choose = -1;
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
                    if (grids[target].p_num && (grids[target].has_pumpkin() || z->show_type() == gargantuar || grids[target].plant_0->show_s_n())) return target;   
                }
            }
        }
    }
    else {
        int r = z->show_range();
        while (r>=0) {
            target = x*MAP_COL+y;
            if (grids[target].p_num && (grids[target].has_pumpkin() || z->show_type() == gargantuar || grids[target].plant_0->show_s_n())) return target;
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

void Map::farmer_plant_pumpkin(int grid_id) {
    int tmp[] = {-MAP_COL, -1, 0, 1, MAP_COL};
    for (int i = 0; i < 5; i++) {
        int ref = tmp[grids[grid_id].plant_0->show_strategy(i)];
        
        bool flag;
        if (ref == tmp[0]) {flag = (grid_id>=MAP_COL);}
        else if (ref == tmp[1]) {flag = (grid_id%MAP_COL);}
        else if (ref == tmp[3]) {flag = (grid_id%MAP_COL<(MAP_COL-1));}
        else if (ref == tmp[4]) {flag = (grid_id/MAP_COL<(MAP_LINE-1));}
        else {flag = true;}

        if (flag && grids[grid_id+ref].show_type()!=g_z_base && !grids[grid_id+ref].plant_p) {
            grids[grid_id+ref].add_plant(pumpkin);
            return;
        }
    }
    grids[grid_id].plant_0->counter_plus();
}

void Map::update(int& sun, bool& lose, int& score) {
    for (int i = 0; i < MAP_LINE; i++) { 
        for (int j = 0; j < MAP_COL; j++) {
            int grid_id = i*MAP_COL+j;
            if (spec_type==fort && spec_coord==grid_id && grids[grid_id].plant_p->show_HP() <= 0) {
                spec_type = d_fort;
            }
            if (grids[grid_id].plant_p) {
                if (grids[grid_id].plant_p->show_HP() <= 0) {   
                    if (grids[grid_id].show_choose() == -2) {grids[grid_id].set_choose(-3);}
                    grids[grid_id].use_shovel(pumpkin);
                }
            }
            if (grids[grid_id].plant_0) {
                if (grids[grid_id].plant_0->show_freeze()) {grids[grid_id].plant_0->check_freeze();}
                if (grids[grid_id].plant_0->show_HP() <= 0) {
                    if (grids[grid_id].show_choose() == -1) {grids[grid_id].set_choose(-3);}
                    grids[grid_id].use_shovel(grids[grid_id].show_plant_type());
                }
                else{
                    grids[grid_id].plant_0->find_zombie = find_zombies(i, j, grids[grid_id].plant_0);
                    grids[grid_id].plant_0->cooldown();
                    switch (grids[grid_id].plant_0->show_type()) {
                        case sunflower: {
                            if (grids[grid_id].plant_0->show_counter()==0) {
                                sun += grids[grid_id].plant_0->gen_sun();
                            } 
                            break;
                        }
                        case wallnut: {
                            break;
                        }
                        case spikeweed: {
                            if (grids[grid_id].plant_0->find_zombie >= 0 && grids[grid_id].plant_0->show_counter()==0) {
                                for (int k = 0, zz = 0; k < ZOMBIE_NUM && zz < grids[grid_id].z_num-grids[grid_id].a_z_num; k++) {
                                    if (grids[grid_id].zombies[k] && grids[grid_id].zombies[k]->show_z_type()!=z_air) {
                                        zz++;
                                        grids[grid_id].zombies[k]->be_attacked(grids[grid_id].plant_0->attack());
                                    }
                                }
                            }
                            break;
                        }
                        case farmer: {
                            if (grids[grid_id].plant_0->show_counter()==0) {
                                farmer_plant_pumpkin(grid_id);
                            }
                            break;
                        }
                        case dryad: {
                            if (grids[grid_id].plant_0->find_zombie >= 0 && grids[grid_id].plant_0->show_counter()==0) {
                                int target = grids[grid_id].plant_0->find_zombie;
                                for (int k = 0; k < ZOMBIE_NUM; k++) {
                                    if (grids[target].zombies[k]) {
                                        grids[target].zombies[k]->be_attacked(grids[grid_id].plant_0->attack());
                                        grids[target].zombies[k]->be_poisoned(grids[grid_id].plant_0->poison());
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                        case bamboo:
                        case cabbage:
                        case pea: {
                            if (grids[grid_id].plant_0->find_zombie >= 0 && grids[grid_id].plant_0->show_counter()==0) {
                                int target = grids[grid_id].plant_0->find_zombie;
                                for (int k = 0; k < ZOMBIE_NUM; k++) {
                                    if (grids[target].zombies[k]) {
                                        grids[target].zombies[k]->be_attacked(grids[grid_id].plant_0->attack());
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                        case cherry: {
                            if (grids[grid_id].plant_0->show_counter()==0) {
                                int range = grids[grid_id].plant_0->show_range();
                                for (int _i = -range; _i <= range; _i++) {
                                    if (i+_i < 0 || i+_i >= MAP_LINE) continue;
                                    for (int _j = -range; _j <= range; _j++) {
                                        if (j+_j < 0 || j+_j >= MAP_COL) continue;
                                        for (int k = 0, zz = 0; k < ZOMBIE_NUM && zz < grids[(i+_i)*MAP_COL+j+_j].z_num; k++) {
                                            if (grids[(i+_i)*MAP_COL+j+_j].zombies[k]) {
                                                zz++;
                                                grids[(i+_i)*MAP_COL+j+_j].zombies[k]->be_attacked(grids[grid_id].plant_0->attack());
                                            }
                                        }
                                    }
                                }
                                grids[grid_id].plant_0->suicide();
                            }
                            break;
                        }
                        default: break;
                    }
                }
            }
            
            if (grids[grid_id].z_num) {
                int tmp_z_num = grids[grid_id].z_num;
                for (int k = 0, zz = 0; k < ZOMBIE_NUM && zz < tmp_z_num; k++) {
                    if (!grids[grid_id].zombies[k]) continue;
                    zz++;
                    grids[grid_id].zombies[k]->check_encourage(zombie_table[necromancer].damage);
                    if (grids[grid_id].zombies[k]->show_poison()) {grids[grid_id].zombies[k]->suffer_poison();}

                    if (grids[grid_id].zombies[k]->show_HP() <= 0) {
                        if (cursor_choose == grid_id && grids[grid_id].show_choose() == k) {
                            grids[grid_id].set_choose(-3);
                        }
                        score += zombie_table[grids[grid_id].zombies[k]->show_type()].score;
                        grids[grid_id].free_zombie(k);
                    }
                    else if (grids[grid_id].zombies[k]->show_type() == balloon &&
                            (grids[grid_id].zombies[k]->show_t_HP() > grids[grid_id].zombies[k]->show_HP() ||
                             grids[grid_id].zombies[k]->show_r_g() == 0)){
                        Zombies *o = grids[grid_id].zombies[k];
                        Zombie *z = new Zombie(o->show_HP(), o->show_scounter(), o->show_path(), o->show_poison());
                        z->set_direction(paths[z->show_path()][grid_id], o->show_r_g());
                        int tmp_c = grids[grid_id].add_zombie(z);
                        if (cursor_choose == grid_id && grids[grid_id].show_choose() == k) {
                            grids[grid_id].set_choose(tmp_c);
                        }
                        grids[grid_id].free_zombie(k);
                    }
                    else if (grids[grid_id].zombies[k]->show_type() == gargantuar &&
                        grids[grid_id].zombies[k]->has_throw==-1 &&
                        ((grids[grid_id].zombies[k]->show_HP() <= grids[grid_id].zombies[k]->show_t_HP()/2) || 
                        (grids[grid_id].zombies[k]->show_r_g() <= length[grids[grid_id].zombies[k]->show_path()]/2))) {
                        grids[grid_id].zombies[k]->has_throw = FPS;
                    }
                    else if (grids[grid_id].zombies[k]->has_throw>0) {grids[grid_id].zombies[k]->has_throw--;}
                    else {
                        if (spec_type==d_fort) {grids[grid_id].zombies[k]->get_crazy();}
                        int target = find_plants(i, j, grids[grid_id].zombies[k]);
                        grids[grid_id].zombies[k]->cooldown();
                        
                        /* The zombie can move or attack */
                        if (grids[grid_id].zombies[k]->show_counter()==0) {
                            if (grids[grid_id].zombies[k]->has_throw==0) {
                                Imp *z = new Imp(grids[grid_id].zombies[k]->show_path());
                                int ttarget = find_next_n(length[z->show_path()]/4, i, j, z->show_path());
                                z->set_direction(paths[z->show_path()][ttarget]);
                                grids[ttarget].add_zombie(z);
                                grids[grid_id].zombies[k]->has_throw = -2;
                                continue;
                            }
                            /* The zombie finds plants */
                            if (target != -1 || grids[grid_id].zombies[k]->show_type()==necromancer) {
                                switch (grids[grid_id].zombies[k]->show_type()) {
                                    case frostwyrm: {
                                        if (grids[target].plant_p) {grids[target].plant_p->be_attacked(grids[grid_id].zombies[k]->attack());}
                                        else if (grids[target].plant_0) {grids[target].plant_0->be_attacked(grids[grid_id].zombies[k]->attack());}

                                        int r = grids[grid_id].zombies[k]->show_freeze_r();
                                        for (int _i = -r; _i <= r; _i++) {
                                            if (i + _i < 0 || i + _i >= MAP_LINE) continue;
                                            for (int _j = -r; _j <= r; _j++) {
                                                if (j + _j < 0 || j + _j >= MAP_COL) continue;
                                                int f_target = (i+_i)*MAP_COL+j+_j;
                                                if (grids[f_target].plant_0) grids[f_target].plant_0->be_freezed(grids[grid_id].zombies[k]->freeze());
                                            }
                                        }
                                        break;
                                    }
                                    case necromancer: {
                                        int r = grids[grid_id].zombies[k]->show_encourage_r();
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
                                        if (grids[target].plant_p) {grids[target].plant_p->be_attacked(grids[grid_id].zombies[k]->attack());}
                                        if (grids[target].plant_0) {grids[target].plant_0->be_attacked(grids[grid_id].zombies[k]->attack());}
                                        break;
                                    }
                                    default: {
                                        if (grids[target].plant_p) {grids[target].plant_p->be_attacked(grids[grid_id].zombies[k]->attack());}
                                        else if (grids[target].plant_0) {grids[target].plant_0->be_attacked(grids[grid_id].zombies[k]->attack());}
                                        break;
                                    }
                                }
                            }
                            /* The zombie is not warlike or cannot find plants, so he moved */
                            if (!grids[grid_id].zombies[k]->show_warlike() || target == -1) {
                                int ready = grids[grid_id].zombies[k]->walk();
                                if (ready) {
                                    int next = grids[grid_id].zombies[k]->cross_grid(i, j);
                                    if (next<0) {
                                        if (cursor_choose == grid_id && k == grids[grid_id].show_choose()) {
                                            cursor_choose = -1;
                                            grids[grid_id].set_choose(-3);
                                        }
                                        if (grids[grid_id].zombies[k]->show_z_type() == z_ground) {
                                            lose = true;
                                            return;
                                        }
                                        else {
                                            grids[grid_id].free_zombie(k);
                                        }
                                    }
                                    else {
                                        grids[grid_id].zombies[k]->set_direction(paths[grids[grid_id].zombies[k]->show_path()][next]);
                                        int tmp_c = grids[next].add_zombie(grids[grid_id].zombies[k]);
                                        if (cursor_choose == grid_id && k == grids[grid_id].show_choose()) {
                                            grids[grid_id].set_choose(-3);
                                            cursor_choose = next;
                                            grids[cursor_choose].set_choose(tmp_c);
                                        }
                                        grids[grid_id].del_zombie(k);   
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