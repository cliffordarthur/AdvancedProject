#include "common.h"

class Grid{
    int x, y;
public:
    Grid();
    void set_coordinate(int x, int y);
    friend class Map;
};

class Map{
    Grid grids[MAP_LINE][MAP_COL];
public:
    Map();
    void init();
};