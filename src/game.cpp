#include "../include/game.h"
#include "../include/map.h"

Board::Board(wxFrame *parent): wxPanel(parent, wxID_ANY, wxDefaultPosition){
    timer = new wxTimer(this, wxID_ANY);
    wxSB = parent->GetStatusBar();
    sun = 0;
    score = 0;
    cursor_x = 0;
    cursor_y = 0;
    time_counter = 0;
    has_start = false;
    has_pause = false;

    MapButton1 = new wxButton(this, MAP_1, wxT("MAP1"), wxPoint(473, 250), wxSize(900, 150));
    MapButton2 = new wxButton(this, MAP_2, wxT("MAP2"), wxPoint(473, 550), wxSize(900, 150));
    MapButton1->SetFont(wxFontInfo(16));
    MapButton2->SetFont(wxFontInfo(16));
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnMap1, this, MAP_1);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnMap2, this, MAP_2);
    Bind(wxEVT_MOTION, &Board::OnMove, this);
    Bind(wxEVT_LEFT_DOWN, &Board::OnLeftDown, this);
    Bind(wxEVT_RIGHT_DOWN, &Board::OnRightDown, this);
}

void Board::Start() {
    has_start = true;
    timer->Start(1000/FPS);
    for (int i = 0; i < plant_num; i++) {
        SHOP.push_back(new wxButton(this, SHOP_0+i, plant_table[i].name, wxPoint(150+100*i, 0), wxSize(100, 100)));
        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnShop, this, SHOP_0+i);
    }
    
    CHEAT.push_back(new wxButton(this, ID_Pause, wxT("PAUSE"), wxPoint(20, 140), wxSize(80, 40)));
    Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnPause, this, ID_Pause);

    #ifdef DOCHEAT
        CHEAT.push_back(new wxButton(this, ID_Pause, wxT("ADD SUN"), wxPoint(20, 180), wxSize(80, 40)));
        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnAddSun, this, ID_AS);
        CHEAT.push_back(new wxButton(this, ID_Pause, wxT("NO CD"), wxPoint(20, 220), wxSize(80, 40)));
        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnNoCD, this, ID_NC);
        CHEAT.push_back(new wxButton(this, ID_Pause, wxT("ADD ENEMY"), wxPoint(20, 260), wxSize(80, 40)));
        Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Board::OnAddEnemy, this, ID_AE);
        CHEAT.push_back(new wxButton(this, ID_Pause, wxT("KILL ALL"), wxPoint(20, 300), wxSize(80, 40)));
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
    shop.add_to_cart(event.GetId()-SHOP_0, sun);
}

void Board::OnTimer(wxTimerEvent &event) {
    wxSB->SetStatusText(wxString::Format("score: %d, time: %.2f", score, time_counter/FPS));
    #ifdef SHOW_COORD
        wxSB->SetStatusText(wxString::Format("score: %d, time: %.2f, x: %d, y: %d", score, time_counter/FPS, cursor_x, cursor_y));
    #endif
    time_counter++;
    Refresh();
}

void Board::OnPause(wxCommandEvent &event) {
    if (has_pause) timer->Start(1000/FPS);
    else timer->Stop();
    has_pause = !has_pause;
}

void Board::OnMove(wxMouseEvent &event) {
    cursor_x = event.GetPosition().x;
    cursor_y = event.GetPosition().y;
}

void Board::OnLeftDown(wxMouseEvent &event) {
    if (!has_start || has_pause) return;
    

    if (MAP_BEGIN_X <= cursor_x && cursor_x <= MAP_BEGIN_X + GRID_SIZE*MAP_COL && 
        MAP_BEGIN_Y <= cursor_y && cursor_y <= MAP_BEGIN_Y + GRID_SIZE*MAP_LINE) {
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
                            // TODO: SHOW PUMPKIN INFO
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
                            // TODO: SHOW PLANT INFO
                        }
                    }
                }
            }
            else {
                //TODO: enemys
            }
        }
    }
    // else if () TODO: click info
}

void Board::OnRightDown(wxMouseEvent &event) {
    shop.clear_cart();
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
    wxMessageBox("About the game", "ABOUT", wxOK | wxICON_INFORMATION, this);
}

bool MyApp::OnInit() {
    MyFrame *frame = new MyFrame(wxT("Game"));
    srand(time(NULL));
    frame->Show(true);
    return true;
}

Game::Game() {
    score = 0;
    sun = 100;
    cursor_x = 0;
    cursor_y = 0;
    lose = false;
    esc = false;
    time_counter = 0; 
}

void Game::init() {

    win = initscr();
    cbreak();
    keypad(stdscr, true);
    noecho();
    curs_set(0);
    
    if (!has_colors()) {
        endwin();
        printf("Your terminal doesn't support color\n");
        exit(1);
    }
    start_color();

    init_pair(WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
    init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
    init_pair(GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
    init_pair(YELLOW_BLACK, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
    init_pair(MAGENTA_BLACK, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);
    if ((LINES < R_LINES)||(COLS < R_COLS)) {
        endwin();
        printf("In order to display the graph completely, the length and width of your terminal "\
            "should not be less than 170 and 52\n");
        exit(1);
    }
}

void Game::wait() {
    while (((double)(now-begin)/CLOCKS_PER_SEC) < 1.0/FPS) now = clock();
    begin = now;
}

void Game::show_info() {
    printnc(MAP_COL*(GRID_LEN+1)/2-2, WHITE_BLACK, "=");
    printc(YELLOW_BLACK, "INFO");
    printnc(MAP_COL*(GRID_LEN+1)/2-2, WHITE_BLACK, "=");
    printc(WHITE_BLACK, "\n");
    printc(WHITE_BLACK, "SUN:");
    printc(YELLOW_BLACK, "%d", sun);
    printnc(MAP_COL*(1+GRID_LEN)/3, WHITE_BLACK, " ");
    printc(WHITE_BLACK, "SCORE:");
    printc(YELLOW_BLACK, "%d", score);
    printnc(MAP_COL*(1+GRID_LEN)/3, WHITE_BLACK, " ");
    printc(WHITE_BLACK, "TIME:");
    printc(YELLOW_BLACK, "%d\n", time_counter/FPS);
}

void Game::refresh_map() {
    move(0, 0);

    show_info();
    shop.update();
    map.draw(cursor_x, cursor_y);
    
    refresh();
}

void Game::input(char ch) {
    switch (ch) {    
        case KEYUP: {
            if (cursor_x && (map.grids[(cursor_x-1)*MAP_COL+cursor_y].show_type()!=g_z_base)) cursor_x--;
            break;
        }
        case KEYDOWN: {
            if ((cursor_x < MAP_LINE-1) && (map.grids[(cursor_x+1)*MAP_COL+cursor_y].show_type()!=g_z_base)) cursor_x++;
            break;
        }
        case KEYLEFT: {
            if (cursor_y && (map.grids[cursor_x*MAP_COL+cursor_y-1].show_type()!=g_z_base)) cursor_y--;
            break;
        }
        case KEYRIGHT: {
            if ((cursor_y < MAP_COL-1) && (map.grids[cursor_x*MAP_COL+cursor_y+1].show_type()!=g_z_base)) cursor_y++;
            break;
        }
        case '0':
        case '1': 
        case '2':
        case '3':
        case '4':
        case '5':
        case '6': 
        case '7':
        case '8':
        case '9': {
            shop.add_to_cart(ch-'0', sun);
            break;
        }
        case '-': {
            shop.add_to_cart(shovel, sun);
            break;
        }
        case KEYENTER: {
            int tmp = 0;
            if (map.spec_type==fort && map.spec_coord==cursor_x*MAP_COL+cursor_y) {tmp = 1;}
            if (map.grids[cursor_x*MAP_COL+cursor_y].can_plant(shop.show_cart(), tmp)){
                if (shop.show_cart()!=shovel){
                    map.grids[cursor_x*MAP_COL+cursor_y].add_plant(shop.show_cart());
                    sun -= shop.buy();
                }
                else {
                    int r = (tmp)?(map.grids[cursor_x*MAP_COL+cursor_y].use_shovel(1)):(map.grids[cursor_x*MAP_COL+cursor_y].use_shovel());
                    sun += (shop.ret_sun(r)+shop.buy());
                }
            }
            else map.can_buy = false;
            break;
        }
        case KEYESC: {
            esc = true; 
            break;
        }
        case KEYSPACE: {
            shop.clear_cart();
            break;
        }
        case 's': {
            sun += 10000;
            break;
        }
        case 'c':{
            shop.no_CDtime();
            break;
        }
        case 'k':{
            map.cheat_kill();
            break;
        }
        case 'z': {
            cheat_gen_zombie(20);
            break;
        }
        default: break;
    }
}

void Game::gen_sun() {
    if (!(time_counter % (8*FPS))) {
        sun += (10+rand()%40);
    }
}

void Game::gen_zombie() {
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

void Game::cheat_gen_zombie(int num) {
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

void Game::check() {
    if (esc || lose || (time_counter>FPS*WIN_SEC)) return;
    gen_sun();
    gen_zombie();
    map.update(sun, lose, score);
}

void Game::show_help() {
    printc(YELLOW_BLACK, "HELP\n\n");
    printc(WHITE_BLACK, "In the game, you can use the keyboard to buy plants and plant them to a certain area of the map"\
        "to resist the attack of zombies.\n\n"\
        "The victory condition is to persist for ");
    printc(YELLOW_BLACK, "ten minites")
    printc(WHITE_BLACK, ", so that the zombie does not reach the end of the path.\n\n");

    printc(WHITE_BLACK, "You can press the ");
    printc(YELLOW_BLACK, "number keys ");
    printc(WHITE_BLACK, "to select plants, use the ");
    printc(YELLOW_BLACK, "arrow keys ");
    printc(WHITE_BLACK, "to select the area to be planted, and then press ");
    printc(YELLOW_BLACK, "Enter ");
    printc(WHITE_BLACK, "to purchase or press ");
    printc(YELLOW_BLACK, "Space ");
    printc(WHITE_BLACK, "to cancel the purchase.\n\n");

    printc(WHITE_BLACK, "This is the correspondence between plants and number keys:\n");
    for (int i = 1; i < plant_num-1; i++) {
        printc(YELLOW_BLACK, "  %d.  ", i);
        printc(GREEN_BLACK, plant_table[i].name);
        if (i%2) printw("\t\t\t");
        else printw("\n");

        if (i == plant_num-2) i = -1;
        else if (!i) break;
    }
    printc(WHITE_BLACK, "\nBesides, you can press ");
    printc(YELLOW_BLACK, "- ");
    printc(WHITE_BLACK, "to use the ");
    printc(GREEN_BLACK, "shovel\n\n");

    printc(WHITE_BLACK, "If you do not want to play, you can press ");
    printc(YELLOW_BLACK, "ESC ");
    printc(WHITE_BLACK, "to quit.\n\n");

    printc(WHITE_BLACK, "Now, you can press ");
    printc(YELLOW_BLACK, "1 ");
    printc(WHITE_BLACK, "to use the custom map or press nothing to use the default map. Then press ")
    printc(YELLOW_BLACK, "ENTER ");
    printc(WHITE_BLACK, "to start.\n");
}

void Game::show_result() {
    if (lose) {printc(RED_BLACK, "\nYou LOST the game!\n");}
    else if (esc) {printc(YELLOW_BLACK, "\nYou have QUIT the game!\n");}
    else {printc(GREEN_BLACK, "\nYou WIN the game!\n");} 
    printw("Your score is ");
    printc(YELLOW_BLACK, "%d", score);

    refresh();
    sleep(5);
    endwin();
    
    if (lose) printf("You lost the game, and your score is %d\n", score);
    else if (esc) printf("You have quit the game, and your score is %d\n", score);
    else printf("You win the game, and your score is %d\n", score);
}

int Game::read_map(int choice) {
    FILE *fp;
    if (choice==1) fp = fopen("src/map/map_1.txt", "r");
    else if (choice==2) fp = fopen("src/map/map_2.txt", "r");
    else if (choice==3) fp = fopen("src/map/map_3.txt", "r");
    else fp = fopen("src/map/map.txt", "r");
    if (!fp) return -1;
    
    fscanf(fp, "%d", &MAP_LINE);
    fscanf(fp, "%d", &MAP_COL);
    fscanf(fp, "%d", &g_path_num);
    fscanf(fp, "%d", &a_path_num);
    
    if (!(MAP_LINE>0 && MAP_COL>0 && g_path_num>0 && a_path_num>=0)) return -2;
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
                        
                if (!(tmpx>=0 && tmpx<MAP_LINE && tmpy>=0 && tmpy<MAP_COL)) return -3;

                if (strcmp(name, "up")==0) tmpd=diup;
                else if (strcmp(name, "down")==0) tmpd=didown;
                else if (strcmp(name, "left")==0) tmpd=dileft;
                else if (strcmp(name, "right")==0) tmpd=diright;
                else if (strcmp(name, "end")==0) {tmpd=diend; flag=false;}
                else return -3;
            }

            if (i < g_path_num+a_path_num) {
                if (map.start[i] == -1) {
                    map.start[i] = tmpx*MAP_COL+tmpy;
                    if (i < g_path_num) map.grids[tmpx*MAP_COL+tmpy].set_type(g_z_base);
                }
                else {
                    switch (lastd) {
                        case diup: {
                            if (!(lastx > tmpx && lasty == tmpy)) return -3;
                            for (int l = lastx; l > tmpx ; l--) {
                                map.paths[i][l*MAP_COL+tmpy] = lastd;
                                map.length[i]++;
                            }
                            break;
                        }
                        case didown: {
                            if (!(lastx < tmpx && lasty == tmpy)) return -3;
                            for (int l = lastx; l < tmpx ; l++) {
                                map.paths[i][l*MAP_COL+tmpy] = lastd;
                                map.length[i]++;
                            }
                            break;
                        }
                        case dileft: {
                            if (!(lasty > tmpy && lastx == tmpx)) return -3;
                            for (int l = lasty; l > tmpy ; l--) {
                                map.paths[i][tmpx*MAP_COL+l] = lastd;
                                map.length[i]++;
                            }
                            break;
                        }
                        case diright: {
                            if (!(lasty < tmpy && lastx == tmpx)) return -3; 
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
                        default: return -3;
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
        return -4;
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
    else return -5;

    fscanf(fp, "%9s", buf);
    if (strcmp(buf, "endfile")) fclose(fp);

    fclose(fp);
    return 0;
}

void Game::start() {
    show_help();
    char ch = 0;
    int map_choice = 0;
    while(ch != KEYENTER){
        ch = getch();
        if (ch==KEYESC) {endwin();return;}
        if (ch<='3' && ch>='0') map_choice = ch-'0';
    }
    int flag = read_map(map_choice);
    switch (flag) {
        case -1: {endwin(); printf("Fail to open the file\n"); return;}
        case -2: {endwin(); printf("Invalid map_size or path_num\n"); return;}
        case -3: {endwin(); printf("Invalid path\n"); return;}
        case -4: {endwin(); printf("Invalid grid_type\n"); return;}
        case -5: {endwin(); printf("Invalid special_grid\n"); return;}
        default: break;
    }

    refresh_map();
    begin = clock(), now = clock();

    while(!lose&&!esc&&(time_counter<=FPS*WIN_SEC)) {
        time_counter++;
        wait();
        refresh_map();
        timeout(0);
        ch = getch();
        input(ch);
        check();
    }
    show_result();
}
