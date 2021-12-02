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
int Shop::show_fail() {
    if (fail_to_cart >= 0) return fail_to_cart--;
    else return -1;
}

void Shop::add_to_cart(int i, int sun) {
    if (sun < menu[i].costs || menu[i].counter!=0) fail_to_cart = FPS/10;
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
    return menu[r].costs/2;
}

void Shop::update() { //TODO: button colour (maybe unnecessary)
    for (int i = 0; i < plant_num-1; i++){
        menu[i].cooldown();
    }
}