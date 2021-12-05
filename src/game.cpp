#include "../include/game.h"
#include "../include/map.h"

Board::Board(wxFrame *parent): wxPanel(parent, wxID_ANY, wxDefaultPosition){
    timer = new wxTimer(this, wxID_ANY);
    wxSB = parent->GetStatusBar();
    sun = 100;
    score = 0;
    cursor_x = 0;
    cursor_y = 0;
    time_counter = 0;
    has_start = false;
    has_pause = false;
    lose = false;

    MapButton1 = new wxButton(this, MAP_1, wxT("MAP1"), wxPoint(473, 250), wxSize(900, 150));
    MapButton2 = new wxButton(this, MAP_2, wxT("MAP2"), wxPoint(473, 550), wxSize(900, 150));
    MapButton1->SetFont(wxFontInfo(16));
    MapButton2->SetFont(wxFontInfo(16));
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnMap1, this, MAP_1);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnMap2, this, MAP_2);
    Bind(wxEVT_MOTION, &Board::OnMove, this);
    Bind(wxEVT_LEFT_DOWN, &Board::OnLeftDown, this);
    Bind(wxEVT_RIGHT_DOWN, &Board::OnRightDown, this);
    Bind(wxEVT_PAINT, &Board::OnPaint, this);
    Bind(wxEVT_TIMER, &Board::OnTimer, this);
}

void Board::Start() {
    has_start = true;
    timer->Start(1000/FPS);
    for (int i = 0; i < plant_num; i++) {
        wxString shop_name = wxString(plant_table[i].name)+wxString::Format("\n\n%d", plant_table[i].costs);
        SHOP.push_back(new wxButton(this, SHOP_0+i, shop_name, wxPoint(150+100*i, 0), wxSize(100, 100)));
        SHOP[i]->SetBackgroundColour(wxColour(GREY51));
        SHOP[i]->SetFont(wxFontInfo(12));
        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnShop, this, SHOP_0+i);
    }
    CHEAT.push_back(new wxButton(this, ID_Pause, wxT("PAUSE"), wxPoint(18, 140), wxSize(92, 40)));
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnPause, this, ID_Pause);

    #ifdef DOCHEAT
        CHEAT.push_back(new wxButton(this, ID_AS, wxT("ADD SUN"), wxPoint(18, 180), wxSize(92, 40)));
        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnAddSun, this, ID_AS);
        CHEAT.push_back(new wxButton(this, ID_NC, wxT("NO CD"), wxPoint(18, 220), wxSize(92, 40)));
        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnNoCD, this, ID_NC);
        CHEAT.push_back(new wxButton(this, ID_AE, wxT("ADD ENEMY"), wxPoint(18, 260), wxSize(92, 40)));
        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnAddEnemy, this, ID_AE);
        CHEAT.push_back(new wxButton(this, ID_KA, wxT("KILL ALL"), wxPoint(18, 300), wxSize(92, 40)));
        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnKillAll, this, ID_KA);
    #endif
}

void Board::OnMap1(wxCommandEvent &event) {
    FILE *fp = fopen("src/map/map_1.txt", "r");
    read_map(fp);
}

void Board::OnMap2(wxCommandEvent &event) {
    FILE *fp = fopen("src/map/map_2.txt", "r");
    read_map(fp);
}

void Board::OnShop(wxCommandEvent &event) {
    if (shop.show_cart()>=0 && shop.show_cart()!=event.GetId()-SHOP_0) {
        SHOP[shop.show_cart()]->SetBackgroundColour(wxColour(LIGHTSTEELBLUE1));
    }
    shop.add_to_cart(event.GetId()-SHOP_0, sun);
}

void Board::OnTimer(wxTimerEvent &event) {
    time_counter++;

    check();
    wxSB->SetStatusText(wxString::Format("SCORE: %d, TIME: %.2f", score, time_counter/FPS));
    #ifdef SHOW_COORD
        wxSB->SetStatusText(wxString::Format("SCORE: %d, TIME: %.2f, x: %d, y: %d", score, time_counter/FPS, cursor_x, cursor_y));
    #endif
    if (time_counter>FPS*WIN_SEC) {
        wxSB->SetStatusText(wxString::Format("YOU WIN, SCORE: %d", score));
        timer->Stop();
    }
    if (lose) {
        wxSB->SetStatusText(wxString::Format("Game Over, SCORE: %d", score));
        timer->Stop();
    }
    Refresh();
}

void Board::OnPaint(wxPaintEvent &event) {
    wxPaintDC dc(this);
    if (has_start) {
        dc.SetTextForeground(wxColour(ORANGE)); 
        dc.SetFont(wxFontInfo(12).Bold(2).FaceName(wxT("MS yahei")));
        dc.DrawText(wxString::Format("SUN: "), wxPoint(20, 47));
        if (shop.show_fail() >= 0) {
            dc.SetTextForeground(wxColour(RED));
        }
        dc.DrawText(wxString::Format("%d", sun), wxPoint(66, 47));
        
        dc.SetBrush(wxColour(LIGHTCYAN));
        for (int i = 0; i < MAP_LINE; i++) {
            for (int j = 0; j < MAP_COL; j++) {
                if (map.grids[i*MAP_COL+j].show_type() == remote) dc.SetBrush(wxColour(DARKGREY)); 
                else if (map.grids[i*MAP_COL+j].show_type() == g_z_base) dc.SetBrush(wxColour(GREY31));
                else dc.SetBrush(wxColour(LIGHTGREY)); 
                if (map.cursor_choose >= 0) {
                    int tmp_choose = map.grids[map.cursor_choose].show_choose();
                    switch (tmp_choose) {
                        case -3: break;
                        case -2: {
                            if (map.cursor_choose == i*MAP_COL+j) {
                                dc.SetPen(wxPen(wxColour(ORANGE), 4, wxPENSTYLE_SOLID));
                            }
                            break;
                        }
                        case -1: {
                            int tmp_p_type = map.grids[map.cursor_choose].show_plant_type();
                            if (tmp_p_type == farmer) {
                                if (abs(i*MAP_COL+j-map.cursor_choose)<=1 || abs(i*MAP_COL+j-map.cursor_choose)==MAP_COL) {
                                    dc.SetPen(wxPen(wxColour(ORANGE), 4, wxPENSTYLE_SOLID));
                                }
                            }
                            else {
                                int tmp_range = plant_table[tmp_p_type].range;
                                int tmp_x = map.cursor_choose/MAP_COL, tmp_y = map.cursor_choose%MAP_COL;
                                if (abs(i-tmp_x)<=tmp_range && abs(j-tmp_y)<=tmp_range) {
                                    dc.SetPen(wxPen(wxColour(ORANGE), 4, wxPENSTYLE_SOLID));
                                }
                            }                        
                            break;
                        }
                        default: {
                            int tmp_z_type = map.grids[map.cursor_choose].show_zombie_type();
                            if (tmp_z_type == necromancer) {
                                Necromancer *tmp_z = new Necromancer();
                                int tmp_range = tmp_z->show_encourage_r();
                                delete tmp_z;
                                int tmp_x = map.cursor_choose/MAP_COL, tmp_y = map.cursor_choose%MAP_COL;
                                if (abs(i-tmp_x)<=tmp_range && abs(j-tmp_y)<=tmp_range) {
                                    dc.SetPen(wxPen(wxColour(PURPLE), 4, wxPENSTYLE_DOT));
                                }
                            }
                            else {
                                int tmp_range = zombie_table[tmp_z_type].range;
                                int tmp_x = map.cursor_choose/MAP_COL, tmp_y = map.cursor_choose%MAP_COL;
                                if (abs(i-tmp_x)<=tmp_range && abs(j-tmp_y)<=tmp_range) {
                                    dc.SetPen(wxPen(wxColour(ORANGE), 4, wxPENSTYLE_SOLID));
                                }
                                else if (tmp_z_type == frostwyrm) {
                                    Frostwyrm *tmp_z = new Frostwyrm();
                                    int freeze_range = tmp_z->show_freeze_r();
                                    delete tmp_z;
                                    if (abs(i-tmp_x)<=freeze_range && abs(j-tmp_y)<=freeze_range) {
                                        dc.SetPen(wxPen(wxColour(CYAN), 4, wxPENSTYLE_DOT));
                                    }
                                }
                            }
                        }
                    }
                    
                }
                dc.DrawRectangle(wxPoint(MAP_BEGIN_X+j*GRID_SIZE, MAP_BEGIN_Y+i*GRID_SIZE), wxSize(GRID_SIZE, GRID_SIZE));
                dc.SetPen(wxColour(BLACK));
                map.grids[i*MAP_COL+j].paint(dc);
            }
        }

        dc.SetBrush(wxColour(WHITE));
        dc.DrawRectangle(wxPoint(INFO_BEGIN_X, INFO_BEGIN_Y), wxSize(3*GRID_SIZE, 6*GRID_SIZE));
        dc.SetTextForeground(wxColour(BLACK));
        dc.SetFont(wxFontInfo(14));
        dc.DrawText("---------------------STATE--------------------", wxPoint(INFO_BEGIN_X+10, INFO_BEGIN_Y+INFO_GAP_Y));
        dc.DrawText("---------------INFORMATION---------------", wxPoint(INFO_BEGIN_X+10, INFO_BEGIN_Y+INFO_GAP_Y+2*GRID_SIZE));
        dc.DrawText("------------------STRATEGY------------------", wxPoint(INFO_BEGIN_X+10, INFO_BEGIN_Y+INFO_GAP_Y+4*GRID_SIZE));
        dc.DrawText("Name: ", wxPoint(INFO_BEGIN_X+10, INFO_BEGIN_Y+4*INFO_GAP_Y));
        dc.DrawText("Race: ", wxPoint(INFO_BEGIN_X+10, INFO_BEGIN_Y+7*INFO_GAP_Y));
        dc.DrawText("HP: ", wxPoint(INFO_BEGIN_X+10, INFO_BEGIN_Y+10*INFO_GAP_Y));
        dc.DrawText("Freezed: ", wxPoint(INFO_BEGIN_X+10, INFO_BEGIN_Y+13*INFO_GAP_Y));
        dc.DrawText("Poisoned: ", wxPoint(INFO_BEGIN_X+10, INFO_BEGIN_Y+16*INFO_GAP_Y));
        dc.DrawText("Crazy: ", wxPoint(INFO_BEGIN_X+10, INFO_BEGIN_Y+19*INFO_GAP_Y));
        dc.DrawText("Evil: ", wxPoint(INFO_BEGIN_X+10, INFO_BEGIN_Y+22*INFO_GAP_Y));
        dc.DrawText("Special: ", wxPoint(INFO_BEGIN_X+10, INFO_BEGIN_Y+4*GRID_SIZE+4*INFO_GAP_Y));

        if (map.cursor_choose >= 0) {
            int Co = (map.grids[map.cursor_choose].show_choose()<0)?GREEN1:RED;
            paint_info(dc, Co, map.grids[map.cursor_choose].show_info().name, 180, 4);
            paint_info(dc, Co, map.grids[map.cursor_choose].show_info().race, 180, 7);
            paint_info(dc, RED, map.grids[map.cursor_choose].show_info().HP, 180, 10);
            paint_info(dc, CYAN, map.grids[map.cursor_choose].show_info().F, 180, 13);
            paint_info(dc, GREEN4, map.grids[map.cursor_choose].show_info().P, 180, 16);
            paint_info(dc, RED, map.grids[map.cursor_choose].show_info().C, 180, 19);
            paint_info(dc, PURPLE, map.grids[map.cursor_choose].show_info().E, 180, 22);
            
            if (Co==GREEN1) {paint_info(dc, BLACK, "Recharge: ", 10, 4, 2);}
            else {paint_info(dc, BLACK, "Distance: ", 10, 4, 2);}
            paint_info(dc, ORANGE, map.grids[map.cursor_choose].show_info().recharge, 180, 4, 2);
            paint_info(dc, BLACK, map.grids[map.cursor_choose].show_info().info, 10, 7, 2);

            paint_info(dc, BLACK, map.grids[map.cursor_choose].show_info().special, 88, 4, 4);
            int order[5];
            
            if (map.grids[map.cursor_choose].show_choose()==-1 && map.grids[map.cursor_choose].show_plant_type()==farmer){
                for (int h = 0; h < 5; h++) order[h] = map.grids[map.cursor_choose].show_tmp_order(h);
                DrawStrategy(dc, 0, STRATEGY_X, STRATEGY_Y, STRATEGY_SIZE, order);
            }
        }
    }
    else {
        dc.SetFont(wxFontInfo(16).Bold(2));
        dc.DrawText(wxT("CHOOSE A MAP"), wxPoint(832, 80));
    }

}

void Board::OnPause(wxCommandEvent &event) {
    if (has_pause) {timer->Start(1000/FPS); CHEAT[0]->SetBackgroundColour(wxColour(WHITE));}
    else {CHEAT[0]->SetBackgroundColour(wxColour(GREY)); timer->Stop();}
    has_pause = !has_pause;
}

void Board::OnMove(wxMouseEvent &event) {
    cursor_x = event.GetPosition().x;
    cursor_y = event.GetPosition().y;
}

void Board::OnLeftDown(wxMouseEvent &event) {
    if (!has_start || lose || time_counter>FPS*WIN_SEC) return;

    if (MAP_BEGIN_X <= cursor_x && cursor_x <= MAP_BEGIN_X + GRID_SIZE*MAP_COL && 
        MAP_BEGIN_Y <= cursor_y && cursor_y <= MAP_BEGIN_Y + GRID_SIZE*MAP_LINE) {
        if (has_pause) return;
        int grid_cursor_y = (cursor_x-MAP_BEGIN_X)/GRID_SIZE;
        int grid_cursor_x = (cursor_y-MAP_BEGIN_Y)/GRID_SIZE;
        int grid_id = grid_cursor_x*MAP_COL+grid_cursor_y;

        bool fort_tmp = (map.spec_type==fort && map.spec_coord==grid_id);
        if (shop.show_cart() >= 0 && shop.show_cart() != shovel){ 
            if(map.grids[grid_id].can_plant(shop.show_cart(), fort_tmp)) {
                map.grids[grid_id].add_plant(shop.show_cart());
                sun -= shop.buy();
            }
        }
        else {
            int relative_x = (cursor_x-MAP_BEGIN_X)%GRID_SIZE;
            int relative_y = (cursor_y-MAP_BEGIN_Y)%GRID_SIZE;
            if (relative_y <= p_lowest) {
                if (inShape(relative_x, relative_y, true, -1)) {
                    if (map.grids[grid_id].has_pumpkin()) {
                        if (shop.show_cart() == shovel && !fort_tmp) {
                            map.grids[grid_id].use_shovel(pumpkin);
                            sun += (shop.ret_sun(pumpkin)+shop.buy());
                        }
                        else if (shop.show_cart() == -1) {
                            map.cursor_choose = grid_id;
                            map.grids[grid_id].set_choose(-2);
                        }
                    }
                }
                else if (map.grids[grid_id].show_plant_type()>=0) {
                    int tmp_type = map.grids[grid_id].show_plant_type();
                    if (inShape(relative_x, relative_y, true, plant_table[tmp_type].p_type)) {
                        if (shop.show_cart() == shovel) {
                            map.grids[grid_id].use_shovel(tmp_type);
                            sun += (shop.ret_sun(tmp_type)+shop.buy());
                        }
                        else if (shop.show_cart() == -1) {
                            map.cursor_choose = grid_id;
                            map.grids[grid_id].set_choose(-1);
                        }
                    }
                }
            }
            else if (relative_y <= a_lowest) {
                int shape_n = inShape(relative_x, relative_y, false, z_air);
                if (0 <= shape_n && shape_n < map.grids[grid_id].show_a_z_num()) {
                    map.cursor_choose = grid_id;
                    map.grids[map.cursor_choose].set_choose(map.grids[map.cursor_choose].return_zumbie_order(shape_n, false));
                }
            }
            else {
                int shape_n = inShape(relative_x, relative_y, false, z_ground);
                if (0 <= shape_n && shape_n < map.grids[grid_id].show_g_z_num()) {
                    map.cursor_choose = grid_id;
                    map.grids[map.cursor_choose].set_choose(map.grids[map.cursor_choose].return_zumbie_order(shape_n));
                }
            }
        }
    }
    else if (map.cursor_choose>=0 && map.grids[map.cursor_choose].show_choose()==-1 && map.grids[map.cursor_choose].show_plant_type()==farmer &&
             INFO_BEGIN_X<=cursor_x && INFO_BEGIN_Y+4*GRID_SIZE<=cursor_y) {
        if (INFO_BEGIN_X+STRATEGY_X+3*STRATEGY_SIZE <= cursor_x && cursor_x <= INFO_BEGIN_X+STRATEGY_X+9*STRATEGY_SIZE/2) {
            if (INFO_BEGIN_Y+4*GRID_SIZE+STRATEGY_Y+STRATEGY_SIZE <= cursor_y && cursor_y <= INFO_BEGIN_Y+4*GRID_SIZE+STRATEGY_Y+2*STRATEGY_SIZE &&
                map.grids[map.cursor_choose].show_tmp_order(4)!=-1) {
                map.grids[map.cursor_choose].set_order();
            }
            if (INFO_BEGIN_Y+4*GRID_SIZE+STRATEGY_Y+STRATEGY_SIZE <= cursor_y && cursor_y <= INFO_BEGIN_Y+4*GRID_SIZE+STRATEGY_Y+3*STRATEGY_SIZE) {
                for (int i = 0; i < 5; i++) map.grids[map.cursor_choose].set_tmp_order(i, -1);
            }
        }
        else {
            if (cursor_x-INFO_BEGIN_X-STRATEGY_X+STRATEGY_SIZE<0 || cursor_y-INFO_BEGIN_Y-STRATEGY_Y-4*GRID_SIZE<0) return;
            int stray = (cursor_x-INFO_BEGIN_X-STRATEGY_X+STRATEGY_SIZE)/STRATEGY_SIZE;
            int strax = (cursor_y-INFO_BEGIN_Y-STRATEGY_Y-4*GRID_SIZE)/STRATEGY_SIZE;
            if (0<=stray && stray<=2 && 0<=strax && strax<=2 && (stray==1||strax==1)) {
                int grid_id = 2*strax+stray-1;
                for (int i = 0; i < 5; i++) {
                    if (map.grids[map.cursor_choose].show_tmp_order(i) == grid_id) break;
                    if (map.grids[map.cursor_choose].show_tmp_order(i)<0) {
                        map.grids[map.cursor_choose].set_tmp_order(i, grid_id);
                        break;
                    }
                }  
            }
        }
        Refresh();
    }
}

void Board::OnRightDown(wxMouseEvent &event) {
    if (!has_start || has_pause || lose || time_counter>FPS*WIN_SEC) return;
    shop.clear_cart();
    map.cursor_choose = -1;
}

void Board::read_map(FILE *fp) {
    delete MapButton1;
    delete MapButton2;
    if (!fp) {
        std::cerr << "Fail to open the map!\n";
        exit(-1);
    }
    fscanf(fp, "%d", &MAP_LINE);
    fscanf(fp, "%d", &MAP_COL);
    fscanf(fp, "%d", &g_path_num);
    fscanf(fp, "%d", &a_path_num);

    if (!(MAP_LINE>0 && MAP_COL>0 && g_path_num>0 && a_path_num>=0)) {
        std::cerr << "Invalid map_size or path_num\n";
        exit(-2);
    }
    if (MAP_LINE > RECOMMEMD_MAX_LINE || MAP_COL > RECOMMEMD_MAX_COLS) {
        std::cerr << "I suggest that The size of the map should not be greater than "
                  << RECOMMEMD_MAX_LINE << " * " << RECOMMEMD_MAX_COLS<<std::endl 
                  << "Otherwise, the problem of incomplete graphic display may occur.\n";
    }
    map.set_Map();

    for (int i = 0; i < g_path_num + a_path_num; i++) {
        int tmpx, tmpy, tmpd;
        int lastx = -1, lasty = -1, lastd = -1;
        char name[6];
        bool flag = true;

        while (1) {
            if (flag) {
                fscanf(fp, "%d", &tmpx);
                fscanf(fp, "%d", &tmpy);
                fscanf(fp, "%5s", name);
                
                if (!(tmpx>=0 && tmpx<MAP_LINE && tmpy>=0 && tmpy<MAP_COL)) {
                    std::cerr << "Invalid path\n";
                    exit(-3);
                }

                if (strcmp(name, "up")==0) tmpd=diup;
                else if (strcmp(name, "down")==0) tmpd=didown;
                else if (strcmp(name, "left")==0) tmpd=dileft;
                else if (strcmp(name, "right")==0) tmpd=diright;
                else if (strcmp(name, "end")==0) {tmpd=diend; flag=false;}
                else {
                    std::cerr << "Invalid path\n";
                    exit(-3);
                }
            }

            if (i < g_path_num+a_path_num) {
                if (map.start[i] == -1) {
                    map.start[i] = tmpx*MAP_COL+tmpy;
                    if (i < g_path_num) map.grids[tmpx*MAP_COL+tmpy].set_type(g_z_base);
                }
                else {
                    switch (lastd) {
                        case diup: {
                            if (!(lastx > tmpx && lasty == tmpy)) {
                                std::cerr << "Invalid path\n";
                                exit(-3);
                            }
                            for (int l = lastx; l > tmpx ; l--) {
                                map.paths[i][l*MAP_COL+tmpy] = lastd;
                                map.length[i]++;
                            }
                            break;
                        }
                        case didown: {
                            if (!(lastx < tmpx && lasty == tmpy)) {
                                std::cerr << "Invalid path\n";
                                exit(-3);
                            }
                            for (int l = lastx; l < tmpx ; l++) {
                                map.paths[i][l*MAP_COL+tmpy] = lastd;
                                map.length[i]++;
                            }
                            break;
                        }
                        case dileft: {
                            if (!(lasty > tmpy && lastx == tmpx)) {
                                std::cerr << "Invalid path\n";
                                exit(-3);
                            }
                            for (int l = lasty; l > tmpy ; l--) {
                                map.paths[i][tmpx*MAP_COL+l] = lastd;
                                map.length[i]++;
                            }
                            break;
                        }
                        case diright: {
                            if (!(lasty < tmpy && lastx == tmpx)) {
                                std::cerr << "Invalid path\n";
                                exit(-3);
                            } 
                            for (int l = lasty; l < tmpy ; l++) {
                                map.paths[i][tmpx*MAP_COL+l] = lastd;
                                map.length[i]++;
                            }
                            break;
                        }
                        case diend: {
                            map.paths[i][lastx*MAP_COL+lasty] = lastd;
                            map.length[i]++;
                            break;
                        }
                        default: {
                            std::cerr << "Invalid path\n";
                            exit(-3);
                        }
                    }
                    if (lastd == diend) break;
                }
                lastx = tmpx;
                lasty = tmpy;
                lastd = tmpd;
            }
        }
    }

    char buf[10];
    fscanf(fp, "%9s", buf);
    if (strcmp(buf, "pvz") && strcmp(buf, "default") && strcmp(buf, "melle") && strcmp(buf, "remote")) {
        std::cerr << "Invalid grid_type\n"; 
        exit(-4);
    }
    if (strcmp(buf, "pvz")==0) {
        for (int i = 0; i < MAP_LINE; i++) {
            for (int j = 0; j < MAP_COL; j++) {
                map.grids[i*MAP_COL+j].set_type(mr);
            }
        }
    }
    else {
        for (int i = 0; i < g_path_num; i++) {
            for (int j = 0; j < MAP_LINE*MAP_COL; j++) {
                if (map.paths[i][j]>=0) {map.grids[j].set_type(melle);}
            }
        }
    }
    while (strcmp(buf, "melle")==0 || strcmp(buf, "remote")==0) {
        int tmpx, tmpy;
        fscanf(fp, "%d", &tmpx);
        fscanf(fp, "%d", &tmpy);
        if (strcmp(buf, "melle")) {map.grids[tmpx*MAP_COL+tmpy].set_type(melle);}
        else {map.grids[tmpx*MAP_COL+tmpy].set_type(remote);}
        fscanf(fp, "%9s", buf);
    }

    fscanf(fp, "%9s", buf);
    if (strcmp(buf, "fort")==0) {
        int fx, fy;
        fscanf(fp, "%d", &fx);
        fscanf(fp, "%d", &fy);
        map.spec_type = fort;
        map.spec_coord = fx*MAP_COL+fy;
        map.grids[map.spec_coord].add_fort();
    }
    else if (strcmp(buf, "none")==0) {}
    else {
        std::cerr << "Invalid special_grid\n";
        exit(-5);
    }

    fscanf(fp, "%9s", buf);
    if (strcmp(buf, "endfile")) fclose(fp);
    fclose(fp);
    
    Start();
}

void Board::gen_sun() {
    if (!(int(time_counter) % (8*FPS))) {
        sun += (25+rand()%50);
    }
}

void Board::gen_zombie() {
    if (rand()%(FPS*5)==0) {
        int type = zombie;

        int tmp = rand()%100;
        if (tmp<30) type = zombie;//30
        else if (tmp<50) type = conehead;//20
        else if (tmp<55) type = gargantuar;//5
        else if (tmp<60) type = necromancer;//5
        else if (tmp<70) type = catapult;//10
        else if (tmp<85) type = balloon;//15
        else if (tmp<95) type = bomber;//10
        else type = frostwyrm;//5

        int target, length;
        switch (type) {
            case zombie: {
                Zombie *z = new Zombie;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                map.grids[target].add_zombie(z);
                break;
            }
            case conehead: {
                Conehead *z = new Conehead;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                map.grids[target].add_zombie(z);
                break;
            }
            case gargantuar: {
                Gargantuar *z = new Gargantuar;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                map.grids[target].add_zombie(z);
                break;
            }
            case necromancer: {
                Necromancer *z = new Necromancer;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                map.grids[target].add_zombie(z);
                break;                
            }
            case catapult: {
                Catapult *z = new Catapult;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                map.grids[target].add_zombie(z);
                break;
            }
            case balloon: {
                Balloon *z = new Balloon;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                map.grids[target].add_zombie(z);
                break; 
            }
            case bomber: {
                Bomber *z = new Bomber;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                map.grids[target].add_zombie(z);
                break; 
            }
            case frostwyrm: {
                Frostwyrm *z = new Frostwyrm;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                map.grids[target].add_zombie(z);
                break;                
            }
            default: break;
        }   
    }
}

void Board::cheat_gen_zombie(int num) {
    for (int i = 0; i < num; i++) {
        int type = rand()%zombie_num;
        int target, length;
        switch (type) {
            case imp:
            case zombie:{
                Zombie *z = new Zombie;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                z->be_attacked(-9*z->show_HP());
                z->set_total_HP(z->show_HP());
                map.grids[target].add_zombie(z);
                break;
            }
            case conehead:{
                Conehead *z = new Conehead;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                z->be_attacked(-9*z->show_HP());
                z->set_total_HP(z->show_HP());
                map.grids[target].add_zombie(z);
                break;
            }
            case gargantuar:{
                Gargantuar *z = new Gargantuar;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                z->be_attacked(-9*z->show_HP());
                z->set_total_HP(z->show_HP());
                map.grids[target].add_zombie(z);
                break;
            }
            case necromancer:{
                Necromancer *z = new Necromancer;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                z->be_attacked(-9*z->show_HP());
                z->set_total_HP(z->show_HP());
                map.grids[target].add_zombie(z);
                break;
            }
            case catapult:{
                Catapult *z = new Catapult;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                z->be_attacked(-9*z->show_HP());
                z->set_total_HP(z->show_HP());
                map.grids[target].add_zombie(z);
                break;
            }
            case balloon:{
                Balloon *z = new Balloon;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                z->be_attacked(-9*z->show_HP());
                z->set_total_HP(z->show_HP());
                map.grids[target].add_zombie(z);
                break;
            }
            case bomber:{
                Bomber *z = new Bomber;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                z->be_attacked(-9*z->show_HP());
                z->set_total_HP(z->show_HP());
                map.grids[target].add_zombie(z);
                break;
            }
            case frostwyrm:{
                Frostwyrm *z = new Frostwyrm;
                target = map.start[z->show_path()];
                length = map.length[z->show_path()];
                z->set_direction(map.paths[z->show_path()][target], length);
                z->be_attacked(-9*z->show_HP());
                z->set_total_HP(z->show_HP());
                map.grids[target].add_zombie(z);
                break;
            }
            default:break;
        }
        
    }
}

void Board::check() {
    if ( lose || (time_counter>FPS*WIN_SEC)) return;
    gen_sun();
    gen_zombie();
    map.update(sun, lose, score);
    shop.update();
    for (int i = 0; i < SHOP.size(); i++) {
        if (shop.show_cart() == i) SHOP[i]->SetBackgroundColour(wxColour(THISTLE2));
        else if (shop.show_CD(i)) SHOP[i]->SetBackgroundColour(wxColour(GREY51));
        else SHOP[i]->SetBackgroundColour(wxColour(LIGHTSTEELBLUE1));
    }
}

void Board::paint_info(wxPaintDC &dc, int colour, wxString wxstr, int ref_x, int ref_y_b, int ref_y_k) {
    dc.SetTextForeground(wxColour(colour));
    dc.DrawText(wxstr, wxPoint(INFO_BEGIN_X+ref_x, INFO_BEGIN_Y+ref_y_b*INFO_GAP_Y+ref_y_k*GRID_SIZE));
}

MyFrame::MyFrame(const wxString &title):wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(5000, 5000)) {
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);

    wxStatusBar *wxSB = CreateStatusBar();
    wxSB->SetStatusText("Welcome!");

    Bind(wxEVT_MENU, &MyFrame::OnQuit, this, wxID_EXIT);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);

    Board *board = new Board(this);
}

void MyFrame::OnQuit(wxCommandEvent &event){
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent &event){
    wxMessageBox("Author: Li Borui\nVersion: 1.0", "About the game", wxOK | wxICON_INFORMATION, this);
}

bool MyApp::OnInit() {
    MyFrame *frame = new MyFrame(wxT("Game"));
    srand(time(NULL));
    frame->Show(true);
    return true;
}
