#pragma once
#include "common.h"

const int pumpkin_x = GRID_SIZE/6;
const int pumpkin_y = GRID_SIZE/6;

const int p_other_r = GRID_SIZE/8;

const std::vector<std::vector<wxPoint> > PlantShape = { 
    {wxPoint(GRID_SIZE*7/12, GRID_SIZE*7/24), wxPoint(GRID_SIZE*11/12, GRID_SIZE*7/24), 
     wxPoint(GRID_SIZE*5/6, GRID_SIZE*1/24), wxPoint(GRID_SIZE*2/3, GRID_SIZE*1/24)},
    {wxPoint(GRID_SIZE*2/3, GRID_SIZE*7/24), wxPoint(GRID_SIZE*5/6, GRID_SIZE*7/24), 
     wxPoint(GRID_SIZE*11/12, GRID_SIZE*1/24), wxPoint(GRID_SIZE*7/12, GRID_SIZE*1/24)},    
    {wxPoint(GRID_SIZE*3/4, GRID_SIZE/6)},
};

const int p_lowest = GRID_SIZE*7/24;

const double area_0 = abs((PlantShape[0][1].x+PlantShape[0][2].x-PlantShape[0][0].x-PlantShape[0][3].x)*
                          (PlantShape[0][1].y-PlantShape[0][2].y)/2.0);

int distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double area(int x1, int y1, int x2, int y2, int x3, int y3) {
    return abs(x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2)/2.0;
}

double area(int x1, int y1, wxPoint wP2, wxPoint wP3) {
    return area(x1, y1, wP2.x, wP2.y, wP3.x, wP3.y);
}

bool inShape(int x, int y, bool is_plant, int type) {
    if (is_plant) {
        switch (type) {
            case -1: {
                return (distance(x, y, pumpkin_x, pumpkin_y) <= p_other_r);
            }
            case 0: 
            case 1: {
                double s = area(x, y, PlantShape[type][0], PlantShape[type][PlantShape[type].size()-1]);
                for (int i = 0; i < PlantShape[type].size()-1; i++) {
                    s += area(x, y, PlantShape[type][i], PlantShape[type][i+1]);
                }
                return (s-area_0<=1 && -1<=s-area_0);
            }
            case 2: {
                return (distance(x, y, PlantShape[2][0].x, PlantShape[2][0].y) <= p_other_r);
            }
            default: return false;
        }
    }
    else {
        // TODO: enemy
    }
    return false;
}