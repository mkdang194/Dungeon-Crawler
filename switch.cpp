#include "switch.h"
#include <iostream>

Tile *Switch::onEnter(Character *who){
    std::cout << "Switch activated!\n";
    activate();
    return this;
}
