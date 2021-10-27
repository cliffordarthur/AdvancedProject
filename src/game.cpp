#include "../include/game.h"
#include "../include/map.h"

Game::Game() {
    score = 0;
    sun = 100;
    cursor_x = 0;
    cursor_y = 0;
    lose = false;
    esc = false;
    time_counter = 0; 
}

void Game::init() {

    win = initscr();
    cbreak();
    keypad(stdscr, true);
    noecho();
    curs_set(0);
    
    if (!has_colors()) {
        endwin();
        printf("Your terminal doesn't support color\n");
        exit(1);
    }
    start_color();

    init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
    init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
    init_pair(MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);
    
    if ((LINES < R_LINES)||(COLS < R_COLS)) {
        endwin();
        printf("In order to display the graph completely, the length and width of your terminal "\
            "should not be less than 100 and 45\n");
        exit(1);
    }
}

void Game::wait() {
    while (((double)(now-begin)/CLOCKS_PER_SEC) < 1.0/FPS) now = clock();
    begin = now;
}

void Game::show_info() {
    printc(WHITE_BLACK, "===========================================");
    printc(YELLOW_BLACK, "INFO");
    printc(WHITE_BLACK, "===========================================\n");
    printc(WHITE_BLACK, "SUN:");
    printc(YELLOW_BLACK, "%d\n", sun);
    printc(WHITE_BLACK, "SCORE:");
    printc(YELLOW_BLACK, "%d\n", score);

    printc(WHITE_BLACK, "TIME:");
    printc(YELLOW_BLACK, "%d\n", time_counter/FPS);
}

void Game::refresh_map() {
    move(0, 0);

    show_info();
    shop.update();
    map.draw(cursor_x, cursor_y);
    
    refresh();
}

void Game::input(char ch) {
    switch (ch) {    
        case KEYUP: {
            if (cursor_x>0) cursor_x--;
            break;
        }
        case KEYDOWN: {
            if ((cursor_x<MAP_LINE-1)&&!(cursor_x==MAP_LINE-2 && cursor_y==MAP_COL-1)) cursor_x++;
            break;
        }
        case KEYLEFT: {
            if (cursor_y>0) cursor_y--;
            break;
        }
        case KEYRIGHT: {
            if ((cursor_y<MAP_COL-1)&&!(cursor_x==MAP_LINE-1 && cursor_y==MAP_COL-2)) cursor_y++;
            break;
        }
        case '0':
        case '1': 
        case '2':
        case '3':
        case '4':
        case '5':
        case '6': 
        case '7':
        case '8':
        case '9': {
            shop.add_to_cart(ch-'0', sun);
            break;
        }
        case KEYENTER: {
            if (map.grids[cursor_x*MAP_COL+cursor_y].can_plant(shop.show_cart())){
                if (shop.show_cart()!=shovel){
                    map.grids[cursor_x*MAP_COL+cursor_y].add_plant(shop.show_cart());
                    sun -= shop.buy();
                }
                else {
                    int r = map.grids[cursor_x*MAP_COL+cursor_y].use_shovel();
                    sun += (shop.ret_sun(r)+shop.buy());
                }
            }
            else map.can_buy = false;
            break;
        }
        case KEYESC: {
            esc = true; 
            break;
        }
        case KEYSPACE: {
            shop.clear_cart();
            break;
        }
        case 'c':sun+=10000;break;
        default: break;
    }
}

void Game::gen_sun() {
    if (!(time_counter % (8*FPS))) {
        sun += (10+rand()%40);
    }
}

void Game::gen_zombie() {
    if (rand()%(FPS*5)==0) {
        int type = zombie;
        bool flag;
        if (rand()%5 == 0) type = conehead;  

        switch (type) {
            case zombie: {
                Zombie *z = new Zombie;
                z->set_direction(map.g_path[z->show_path()]->direction);
                map.grids[(MAP_LINE-1)*MAP_COL+MAP_COL-1].add_zombie(z);          
                break;
            }
            case conehead: {
                Conehead *z = new Conehead;
                z->set_direction(map.g_path[z->show_path()]->direction);
                map.grids[(MAP_LINE-1)*MAP_COL+MAP_COL-1].add_zombie(z);
                break;
            }
            default: break;
        }   
    }
}

void Game::check() {
    if (esc || lose || (time_counter>FPS*WIN_SEC)) return;
    gen_sun();
    gen_zombie();
    map.update(sun, lose, score);
}

void Game::show_help() {
    printc(YELLOW_BLACK, "HELP\n\n");
    printc(WHITE_BLACK, "In the game, you can use the keyboard to buy plants and plant them to a certain area of the map\n"\
        "to resist the attack of zombies.\n\n"\
        "The victory condition is to persist for ");
    printc(YELLOW_BLACK, "three minites")
    printc(WHITE_BLACK, ", so that the zombie does not reach the end\n of the path.\n\n");

    printc(WHITE_BLACK, "You can press the ");
    printc(YELLOW_BLACK, "number keys ");
    printc(WHITE_BLACK, "to select plants, use the ");
    printc(YELLOW_BLACK, "arrow keys ");
    printc(WHITE_BLACK, "to select the area to be\nplanted, and then press ");
    printc(YELLOW_BLACK, "Enter ");
    printc(WHITE_BLACK, "to purchase or press ");
    printc(YELLOW_BLACK, "Space ");
    printc(WHITE_BLACK, "to cancel the purchase.\n\n");

    printc(WHITE_BLACK, "This is the correspondence between plants and number keys:\n");
    printc(YELLOW_BLACK, "  1. "); printc(GREEN_BLACK, "Sunflower\n");
    printc(YELLOW_BLACK, "  2. "); printc(GREEN_BLACK, "Wallnut\n");
    printc(YELLOW_BLACK, "  3. "); printc(GREEN_BLACK, "Spikeweed\n");
    printc(YELLOW_BLACK, "  4. "); printc(GREEN_BLACK, "Pumpkin\n");
    printc(WHITE_BLACK, "Besides, you can press ");
    printc(YELLOW_BLACK, "0 ");
    printc(WHITE_BLACK, "to use the ");
    printc(GREEN_BLACK, "shovel\n\n");

    printc(WHITE_BLACK, "If you do not want to play, you can press ");
    printc(YELLOW_BLACK, "ESC ");
    printc(WHITE_BLACK, "to quit.\n\n");

    printc(WHITE_BLACK, "Now, press ");
    printc(YELLOW_BLACK, "ENTER ");
    printc(WHITE_BLACK, "to start.\n");
}

void Game::show_result() {
    if (lose) {printc(RED_BLACK, "\nYou LOST the game!\n");}
    else if (esc) {printc(YELLOW_BLACK, "\nYou have QUIT the game!\n");}
    else {printc(GREEN_BLACK, "\nYou WIN the game!\n");} 
    printw("Your score is ");
    printc(YELLOW_BLACK, "%d", score);

    refresh();
    sleep(1);
    endwin();
    
    if (lose) printf("You lost the game, and your score is %d\n", score);
    else if (esc) printf("You have quit the game, and your score is %d\n", score);
    else printf("You win the game, and your score is %d\n", score);
}

int Game::read_map(int choice) {
    FILE *fp;
    if (choice==0) fp = fopen("src/map/map.txt", "r");
    else fp = fopen("src/map/map_1.txt", "r");
    if (!fp) return -1;
    
    fscanf(fp, "%d", &MAP_LINE);
    fscanf(fp, "%d", &MAP_COL);
    fscanf(fp, "%d", &g_path_num);
    fscanf(fp, "%d", &a_path_num);
    map.set_Map();

    if (!(MAP_LINE>0 && MAP_COL>0 && g_path_num>0 && a_path_num>=0)) return -2;
    for (int i = 0; i < g_path_num + a_path_num; i++) {
        int tmpx, tmpy, tmpd;
        char name[6];
        bool flag = true;
        Path *p = nullptr;

        while (flag) {         
            fscanf(fp, "%d", &tmpx);
            fscanf(fp, "%d", &tmpy);
            fscanf(fp, "%5s", name);
                    
            if (!(tmpx>=0 && tmpx<MAP_LINE && tmpy>=0 && tmpy<MAP_COL)) return -3;
            if (p) {
                switch (p->direction) {
                    case    diup: if (!(p->x > tmpx && p->y == tmpy)) return -3; break;
                    case  didown: if (!(p->x < tmpx && p->y == tmpy)) return -3; break;
                    case  dileft: if (!(p->y > tmpy && p->x == tmpx)) return -3; break;
                    case diright: if (!(p->y < tmpy && p->x == tmpx)) return -3; break;
                    default: return -3;
                }
            }
            
            if (strcmp(name, "up")==0) tmpd=diup;
            else if (strcmp(name, "down")==0) tmpd=didown;
            else if (strcmp(name, "left")==0) tmpd=dileft;
            else if (strcmp(name, "right")==0) tmpd=diright;
            else if (strcmp(name, "end")==0) {tmpd=diend; flag=false;}
            else return -3;

            Path *q = new Path(tmpx, tmpy, tmpd, nullptr);
            if (!p) {
                if (i<g_path_num) {
                    map.grids[tmpx*MAP_LINE+tmpy].set_type(g_z_base);
                    map.g_path[i] = q;
                    p = map.g_path[i];
                }
                else {
                    map.a_path[i-g_path_num] = q;
                    p = map.a_path[i-g_path_num];
                }
            }
            else {
                p->next = q;
                p = q;
            }
        }
    }
    
    char buf[10];
    fscanf(fp, "%9s", buf);
    if (strcmp(buf, "pvz") && strcmp(buf, "default") && strcmp(buf, "melle") && strcmp(buf, "remote")) {
        return -4;
    }
    if (strcmp(buf, "pvz")==0) {
        for (int i = 0; i < MAP_LINE; i++) {
            for (int j = 0; j < MAP_COL; j++) {
                map.grids[i*MAP_COL+j].set_type(mr);
            }
        }
    }
    else {
        for (int i = 0; i < g_path_num; i++) {
            Path *p = map.g_path[i];
            while (p && p->next) {
                map.set_type(melle, p->x, p->y, p->next->x, p->next->y);
                p = p->next;
            }
        }
    }
    while (strcmp(buf, "melle")==0 || strcmp(buf, "remote")==0) {
        int tmpx, tmpy;
        fscanf(fp, "%d", &tmpx);
        fscanf(fp, "%d", &tmpy);
        if (strcmp(buf, "melle")) {map.grids[tmpx*MAP_COL+tmpy].set_type(melle);}
        else {map.grids[tmpx*MAP_COL+tmpy].set_type(remote);}
        fscanf(fp, "%9s", buf);
    }

    fscanf(fp, "%9s", buf);
    if (strcmp(buf, "fort")==0) {
        int fx, fy;
        fscanf(fp, "%d", &fx);
        fscanf(fp, "%d", &fy);
        map.spec_type = fort;
        map.spec_coord = fx*MAP_COL+fy;
    }
    else if (strcmp(buf, "none")==0) {}
    else return -5;

    fscanf(fp, "%9s", buf);
    if (strcmp(buf, "endfile")) fclose(fp);

    fclose(fp);
    return 0;
}

void Game::start() {
    show_help();
    char ch = 0;
    int map_choice = 0;
    while(ch != KEYENTER){
        ch = getch();
        if (ch==KEYESC) {endwin();return;}
        if (ch=='1') map_choice = 1;
        if (ch=='0') map_choice = 0;
    }
    int flag = read_map(map_choice);
    if(flag<0) {endwin();printf("%d bad idea %d\n", map_choice, flag);return;}//TODO: MORE

    refresh_map();
    begin = clock(), now = clock();

    while(!lose&&!esc&&(time_counter<=FPS*WIN_SEC)) {
        time_counter++;
        wait();
        refresh_map();
        timeout(0);
        ch = getch();
        input(ch);
        check();
    }
    show_result();
}
