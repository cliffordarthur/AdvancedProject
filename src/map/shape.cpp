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
                return (abs(s-area_0) <= 1);
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

void DrawStrategy(wxPaintDC &dc, int type, int x, int y, int size, int order[]) {
    dc.SetPen(wxColour(BLACK));
    dc.DrawRectangle(wxPoint(INFO_BEGIN_X+x, INFO_BEGIN_Y+4*GRID_SIZE+y), wxSize(size, size));
    dc.DrawRectangle(wxPoint(INFO_BEGIN_X+x-size, INFO_BEGIN_Y+4*GRID_SIZE+y+size), wxSize(size, size));
    dc.DrawRectangle(wxPoint(INFO_BEGIN_X+x, INFO_BEGIN_Y+4*GRID_SIZE+y+size), wxSize(size, size));
    dc.DrawRectangle(wxPoint(INFO_BEGIN_X+x+size, INFO_BEGIN_Y+4*GRID_SIZE+y+size), wxSize(size, size));
    dc.DrawRectangle(wxPoint(INFO_BEGIN_X+x, INFO_BEGIN_Y+4*GRID_SIZE+y+2*size), wxSize(size, size));
    
    dc.DrawRectangle(wxPoint(INFO_BEGIN_X+x+3*size, INFO_BEGIN_Y+4*GRID_SIZE+y+size), wxSize(3*size/2, size));
    dc.DrawRectangle(wxPoint(INFO_BEGIN_X+x+3*size, INFO_BEGIN_Y+4*GRID_SIZE+y+2*size), wxSize(3*size/2, size));
    dc.DrawText("Save", wxPoint(INFO_BEGIN_X+x+13*size/4, INFO_BEGIN_Y+4*GRID_SIZE+y+5*size/4));
    dc.DrawText("Clear", wxPoint(INFO_BEGIN_X+x+13*size/4, INFO_BEGIN_Y+4*GRID_SIZE+y+9*size/4));
    switch (type) {
        case 0: {
            int t = 0;
            wxPoint wp;
            while (1) {
                if (t>=5 || order[t]==-1) {break;}
                else if (order[t]==0) {wp = wxPoint(INFO_BEGIN_X+x+size/4, INFO_BEGIN_Y+4*GRID_SIZE+y+size/4);}
                else if (order[t]==1) {wp = wxPoint(INFO_BEGIN_X+x-3*size/4, INFO_BEGIN_Y+4*GRID_SIZE+y+5*size/4);}
                else if (order[t]==2) {wp = wxPoint(INFO_BEGIN_X+x+size/4, INFO_BEGIN_Y+4*GRID_SIZE+y+5*size/4);}
                else if (order[t]==3) {wp = wxPoint(INFO_BEGIN_X+x+5*size/4, INFO_BEGIN_Y+4*GRID_SIZE+y+5*size/4);}
                else {wp = wxPoint(INFO_BEGIN_X+x+size/4, INFO_BEGIN_Y+4*GRID_SIZE+y+9*size/4);}
                dc.DrawText(wxString::Format("%d", t), wp);
                t++;
            }
            break;
        }
        case 1: {
            // TODO: pea
            break;
        }
        default: {}
    }
}