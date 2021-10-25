#include "../include/game.h"
int MAP_LINE=6;
int MAP_COL=8;
int g_path_num = 1;

int main() {
    Game game;
    game.init();
    game.start();
    return 0;
}