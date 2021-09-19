#include "../include/game.h"
#include "../include/map.h"

Game::Game() {
    score = 0;
    sun = 0;
    cursor_x = 0;
    cursor_y = 0;
    lose = false;
    esc = false;
}

void Game::init() {
    map.init();
    
    win = initscr();
    cbreak();
    keypad(stdscr, true);
    noecho();

    if (!has_colors()) {
        endwin();
        printf("Your terminal doesn't support color\n");
        exit(1);
    }
    start_color();
    
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

void Game::refresh_map() {
    //TODO: update the display on the screen
    refresh();
}

void Game::input(char ch) {
    //TODO: process the input
    switch (ch) {
        case KEYENTER: break;
        case KEYESC: esc = true; break;
        case KEYSPACE: break;
        default: break;
    }
}

bool Game::check_lose() {
    //TODO:
    return lose;
}

void Game::gen_sun() {
    //TODO: 
}

void Game::gen_zombie() {
    //TODO: 
}

void Game::check() {
    //TODO: update the information inside
    if (esc || check_lose()) return;
    gen_sun();
    gen_zombie();
}

void Game::show_result() {
    if (lose) printf("You lose\n");
    else if (esc) printf("You exit\n");
    //FIXME: more info
}

void Game::start() {
    char ch = 0;
    while(ch != KEYENTER){
        ch = getch();
    }
    begin = clock(), now = clock();

    while(!lose) {
        wait();
        refresh_map();
        ch = getch();
        input(ch);
        check();
    }
    show_result();
}


// enter 10
// space 32
// esc 27
// 
// 
// 
