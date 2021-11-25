#include "../../include/plant.h"

void Menu::cooldown(){
    if (counter != 0) counter--;
}

Shop::Shop(){
    cart = -1;
    fail_to_cart = -1;
    for (int i = 0; i < plant_num; i++) {
        menu[i].menu_set(i, plant_table[i].costs, plant_table[i].CDtime, plant_table[i].p_type);
    }
}

void Shop::add_to_cart(int i, int sun) {
    if (sun < menu[i].costs || menu[i].counter!=0) fail_to_cart = i;
    else cart = i;
}

void Shop::clear_cart() {
    cart = -1;
}

int Shop::buy() {
    menu[cart].counter = menu[cart].CDtime;
    int tmp = menu[cart].costs;
    clear_cart();
    return tmp;
}

int Shop::ret_sun(int r) {
    return menu[r].costs/2;//FIXME: may be a bug
}

void Shop::update() {
    printnc(MAP_COL*(GRID_LEN+1)/2-2, WHITE_BLACK, "=");
    printc(YELLOW_BLACK, "SHOP");
    printnc(MAP_COL*(GRID_LEN+1)/2-2, WHITE_BLACK, "=");
    printc(WHITE_BLACK, "\n");

    for (int i = 1; i < plant_num-1; i++){
        menu[i].cooldown();
        if (i==cart) {printc(MAGENTA_BLACK, "%d\t%s\t%d\t%d", i, plant_table[i].name, menu[i].costs, menu[i].counter/FPS);}
        else if (i==fail_to_cart) {printc(RED_BLACK, "%d\t%s\t%d\t%d", i, plant_table[i].name, menu[i].costs, menu[i].counter/FPS);fail_to_cart=-1;}
        else {            
            if (plant_table[i].p_type==p_melle) {printc(GREEN_BLACK, "%d\t%s", i, plant_table[i].name);}
            else if (plant_table[i].p_type==p_remote) {printc(CYAN_BLACK, "%d\t%s", i, plant_table[i].name);}
            else {printc(YELLOW_BLACK, "%d\t%s", i, plant_table[i].name);}
            
            printc(WHITE_BLACK, "\t%d\t%d", menu[i].costs, menu[i].counter/FPS)
        }
        
        if (i%2) {printnc(MAP_COL*(1+GRID_LEN)/3, WHITE_BLACK, " ");}
        else {printw("\n");}

        if (i == plant_num - 2) i = -1;
        else if (i == 0) break;
    }
    
    if (cart==shovel) {printc(MAGENTA_BLACK, "-\t%s\n", plant_table[shovel].name);}
    else printc(YELLOW_BLACK, "-\t%s\n", plant_table[shovel].name);
    
}