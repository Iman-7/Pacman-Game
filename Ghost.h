
#ifndef Ghost_h
#define Ghost_h

#include <SFML/Graphics.hpp>
#include "Common.h"
#include <string>
using namespace sf;

class ghost{
private:
    int ghosti;
    int ghostj;
    Sprite ghost1;
    Texture ghost_tx;
    Text txt;
    Font fnt;
    Texture fright;
public:
    ghost(int x, int y, Texture tx);
    void setghostSprite(Sprite g);
    Sprite getghostSprite();
    void setghosti(int x);
    void setghostj(int y);
    int getghosti();
    int getghostj();
    void moveGhost(Direction d1, bool &frightmode);
    void displayfright(RenderWindow &window);
};
#endif /* Ghost_h */
