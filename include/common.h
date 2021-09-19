#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h>


// the map has 4 lines and 4 columns
#define MAP_LINE 4
#define MAP_COL 4

// require at least R_LINES lines and R_COLS columns
#define R_LINES 48
#define R_COLS 190

// some keys in curses
#define KEYENTER 10
#define KEYESC 27
#define KEYSPACE 32

// FPS
#define FPS 5