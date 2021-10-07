#include "../include/game.h"
#include "../include/map.h"

Game::Game() {
    score = 0;
    sun = 10000;
    cursor_x = 0;
    cursor_y = 0;
    lose = false;
    esc = false;
    time_counter =0;
}

void Game::init() {
    // map.init();
    
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
        printf("Too small\n");
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
}

void Game::refresh_map() {
    //TODO: update the display on the screen
    move(0, 0);

    show_info();
    shop.update();
    map.draw(cursor_x, cursor_y);
    
    refresh();
}

void Game::input(char ch) {
    //TODO: process the input
    
    switch (ch) {    
        case KEYUP: {
            if (cursor_x>0) cursor_x--;
            break;
        }
        case KEYDOWN: {
            if (cursor_x<MAP_LINE-1) cursor_x++;
            break;
        }
        case KEYLEFT: {
            if (cursor_y>0) cursor_y--;
            break;
        }
        case KEYRIGHT: {
            if (cursor_y<MAP_COL-1) cursor_y++;
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
        default: break;
    }
}

bool Game::check_lose() {
    //TODO:
    return lose;
}

void Game::gen_sun() {
    if (time_counter == 8*FPS) {
        sun += (10+rand()%40);
        time_counter = 0;
    }
}

void Game::gen_zombie() {
    //TODO: 
}

void Game::check() {
    //TODO: update the information inside
    if (esc || check_lose()) return;
    score+=1;
    gen_sun();
    gen_zombie();
    map.update(sun);
}

void Game::show_help() {
    // TODO: show_help
    printc(WHITE_BLACK, "PRESS ");
    printc(YELLOW_BLACK, "ENTER ");
    printc(WHITE_BLACK, "TO START\n");
}

void Game::show_result() {
    endwin();
    if (lose) printf("You lose\n");
    else if (esc) printf("You exit\n");
    //FIXME: more info
}

void Game::start() {
    show_help();

    char ch = 0;
    while(ch != KEYENTER){
        ch = getch();
    }
    refresh_map();
    begin = clock(), now = clock();

    while(!lose&&!esc) {
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
