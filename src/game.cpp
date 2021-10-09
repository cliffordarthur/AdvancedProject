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
            if (map.grids[cursor_x][cursor_y].can_plant(shop.show_cart())){
                if (shop.show_cart()!=shovel){
                    map.grids[cursor_x][cursor_y].add_plant(shop.show_cart());
                    sun -= shop.buy();
                }
                else {
                    int r = map.grids[cursor_x][cursor_y].use_shovel();
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
        // time_counter = 0;
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
                flag = map.grids[MAP_LINE-1][MAP_COL-1].add_zombie(z);
                if (!flag) delete z;
                break;
            }
            case conehead: {
                Conehead *z = new Conehead;
                flag = map.grids[MAP_LINE-1][MAP_COL-1].add_zombie(z);
                if (!flag) delete z;
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
    printc(YELLOW_BLACK, "two minites")
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
    sleep(5);
    endwin();
    
    if (lose) printf("You lost the game, and your score is %d\n", score);
    else if (esc) printf("You have quit the game, and your score is %d\n", score);
    else printf("You win the game, and your score is %d\n", score);
}

void Game::start() {
    show_help();

    char ch = 0;
    while(ch != KEYENTER){
        ch = getch();
    }
    refresh_map();
    begin = clock(), now = clock();

    // Zombie* zz = new Zombie;
    // map.grids[3][3].add_zombie(zz);

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
