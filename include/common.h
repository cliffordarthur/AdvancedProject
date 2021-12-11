#pragma once
#include <wx/wxprec.h>

#include <wx/wx.h>

// #include <stdio.h>
// #include <stdlib.h>
// #include <time.h>
// #include <assert.h>
// #include <string.h>
// #include <unistd.h>

// #include <iostream>
// #include <vector>
// using namespace std;

extern int MAP_LINE;
extern int MAP_COL;
extern int g_path_num;
extern int a_path_num;

// FPS
#define FPS 30

inline int max(int a, int b) {
    return (a>b)?a:b;
}

#define ZOMBIE_NUM 256

#define WIN_SEC 600

enum Plantname{sunflower, cherry, pumpkin, wallnut, spikeweed, bamboo, cabbage, farmer, pea, dryad, shovel};

enum Zombiename{zombie, conehead, imp, gargantuar, necromancer, catapult, balloon, bomber, frostwyrm, bullet};

enum Directions{diup, didown, dileft, diright, diend};

enum Grid_type{melle, remote, mr, g_z_base};

enum Plant_type{p_melle, p_remote, p_other};

enum Zombie_type{z_ground, z_air, z_both};

enum Spec_type{fort, d_fort};

// P3
// #define SHOW_COORD

#define DOCHEAT

#define MAP_BEGIN_X 150
#define MAP_BEGIN_Y 140
#define GRID_SIZE 120
#define INFO_BEGIN_X 1450
#define INFO_BEGIN_Y 140
#define INFO_GAP_Y (GRID_SIZE/12)
#define STRATEGY_X (INFO_GAP_Y*10)
#define STRATEGY_Y (INFO_GAP_Y*8)
#define STRATEGY_SIZE (INFO_GAP_Y*4)

#define RECOMMEMD_MAX_LINE 6
#define RECOMMEMD_MAX_COLS 10

#define MAX_SHOW_A_NUM 4
#define MAX_SHOW_G_NUM 6

enum wxEVENT_ID {MAP_1 = wxID_HIGHEST + 1, MAP_2, 
    ID_Pause, ID_AS, ID_NC, ID_AE, ID_KA, SHOP_0};

enum wxColourReverse {
    BLACK = 0x000000, 
    RED = 0x0000FF, 
    GREEN4 = 0x008B00,
    ORANGE = 0x00A5FF,
    GREEN1 = 0x00FF00,
    GREY31 = 0x4F4F4F,
    GREY51 = 0x828282, 
    DARKGREY = 0xA9A9A9, 
    GREY = 0xBEBEBE,
    LIGHTGREY = 0xD3D3D3, 
    THISTLE2 = 0xEED2EE, 
    PURPLE = 0xF020A0,
    LIGHTSTEELBLUE1 = 0xFFE1CA,
    CYAN = 0xFFFF00, 
    LIGHTCYAN = 0xFFFFE0, 
    WHITE = 0xFFFFFF,
};