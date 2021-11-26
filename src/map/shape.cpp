#include "../../include/map.h"


int distance(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

double area(int x1, int y1, int x2, int y2, int x3, int y3) {
    return abs(x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2)/2.0;
}

double area(int x1, int y1, wxPoint wP2, wxPoint wP3) {
    return area(x1, y1, wP2.x, wP2.y, wP3.x, wP3.y);
}

wxColour hp_color(double p, wxColour wC1, wxColour wC2) {
    /*from wC1(green 00ff00) to yellow ffff00 to wC2(red ff0000)*/
    if (p>1 || p<0) return wC1;
    int r1 = wC1.Red(), g1 = wC1.Green(), b1 = wC1.Blue();
    int r2 = wC2.Red(), g2 = wC2.Green(), b2 = wC2.Blue();
    double r3, g3, b3;
    if (p >= 0.5) {
        r3 = 2*(r1-r2)*p + 2*r2 - r1;
        g3 = g1;
    }
    else {
        r3 = r2;
        g3 = 2*(g1-g2)*p + g2;
    }
    b3 = (b1-b2)*p + b2;
    return wxColour(r3, g3, b3);
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
                return (s-area_0<=1 && -1<=s-area_0);// TODO: more accurate
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

