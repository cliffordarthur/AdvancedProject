#include "../../include/map.h"
//TODO:
Map::Map() {
    for (int i = 0; i < MAP_LINE; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            grids[i][j].set_coordinate(i, j);
        }
    }
}

void Map::init() {
    return;
}