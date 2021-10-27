#pragma once
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

#include <iostream>
#include <vector>
// using namespace std;

// the map has 4 lines and 4 columns
// the length of grid is 10 units 
// displayed as 5*20(long name)
extern int MAP_LINE;
extern int MAP_COL;
extern int g_path_num;
extern int a_path_num;
#define GRID_LEN 20
#define GRID_HEI 5

// require at least R_LINES lines and R_COLS columns
// R_LINES > MAP_LINE * (5+1) + SHOP_LINE + INFO_LINE
// R_COLS > MAP_COL * (20+1)
#define R_LINES 45
#define R_COLS 100

// some keys in curses
#define KEYENTER 10
#define KEYESC 27
#define KEYSPACE 32
#define KEYUP (KEY_UP-0400)
#define KEYDOWN (KEY_DOWN-0400)
#define KEYLEFT (KEY_LEFT-0400)
#define KEYRIGHT (KEY_RIGHT-0400)

// FPS
#define FPS 30
// print with color
#define printc(color, format, ...)\
    do {attron(COLOR_PAIR(color));\
    printw(format, ## __VA_ARGS__);\
    attroff(COLOR_PAIR(color));} while(0);\

enum COLORS{WHITE_BLACK, RED_BLACK, GREEN_BLACK, YELLOW_BLACK, BLUE_BLACK, MAGENTA_BLACK, CYAN_BLACK};

#define ZOMBIE_NUM 30

#define WIN_SEC 180

enum Plantname{shovel, sunflower, wallnut, spikeweed, pumpkin};//TODO: peashooter, cherrybomb, snowpea

enum Zombiename{zombie, conehead};//TODO: polevault, newspaper, ...

enum Directions{diup, didown, dileft, diright, diend};

enum Grid_type{melle, remote, mr, g_z_base};

enum Spec_type{fort};