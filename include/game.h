#pragma once 
#include "common.h"
#include "map.h"
#include "plant.h"
#include "zombie.h"

class MyApp: public wxApp {
public:
    virtual bool OnInit();
};

class Board: public wxPanel {
    int sun, score;
    int cursor_x, cursor_y;
    double time_counter;
    bool has_start, has_pause, lose;
    wxTimer *timer;
    wxStatusBar *wxSB;
    wxButton *MapButton1, *MapButton2;
    std::vector<wxButton*> SHOP, CHEAT;
    Map map;
    Shop shop;

    void OnMap1(wxCommandEvent &event);
    void OnMap2(wxCommandEvent &event);
    void OnShop(wxCommandEvent &event);
    void OnTimer(wxTimerEvent &event);
    void OnPaint(wxPaintEvent &event);
    void OnPause(wxCommandEvent &event);

    void OnAddSun(wxCommandEvent &event) {sun += 10000;}
    void OnNoCD(wxCommandEvent &event) {
        shop.no_CDtime();
        CHEAT[2]->SetBackgroundColour(wxColour(GREY));
    }
    void OnAddEnemy(wxCommandEvent &event) {cheat_gen_zombie(20);}
    void OnKillAll(wxCommandEvent &event) {map.cheat_kill();}

    void OnMove(wxMouseEvent &event);
    void OnLeftDown(wxMouseEvent &event);
    void OnRightDown(wxMouseEvent &event);
public:
    Board(wxFrame* parent);
    void Start();
    void read_map(FILE *fp);

    void gen_sun();
    void gen_zombie();
    void cheat_gen_zombie(int num);

    void check();
    void paint_info(wxPaintDC &dc, int colour, wxString wxstr, int ref_x, int ref_y_b, int ref_y_k=0);
};

class MyFrame: public wxFrame {
    void OnQuit(wxCommandEvent &event);
    void OnAbout(wxCommandEvent &event);
public:
    MyFrame(const wxString& title);
};
