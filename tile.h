#ifndef TILE_H
#define TILE_H
#include <string>

class Character;


class Tile{
protected:
    std::string texture;
    Character* character = nullptr; // observer ptr (raw ptr)
    int rows, cols;

public:

    Tile(std::string t, int r, int c) : texture(t), rows(r), cols(c) {}

    virtual ~Tile() = default;
    virtual std::string getTexture() const;
    virtual bool onLeave(Tile* destTile, Character* who);
    virtual Tile* onEnter(Character* who);

    void setCharacter(Character* c);
    Character* getCharacter() const;
    int getRow() const;
    int getCol() const;
};

#endif // TILE_H
