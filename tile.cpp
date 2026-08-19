#include "Tile.h"
#include "Character.h"

//world behavior unit (polymorphism)

void Tile::setCharacter(Character* c) {
    character = c;
}

Character* Tile::getCharacter() const {
    return character;
}

std::string Tile::getTexture() const {
    return texture;
}

bool Tile::onLeave(Tile* destTile, Character* who){
    return true;
}

//default movement behavior: walkable, unless override special behavior only
Tile* Tile::onEnter(Character* who){
    return this;
}

int Tile::getRow() const{
    return rows;
}

int Tile::getCol() const{
    return cols;
}
