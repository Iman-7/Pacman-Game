
#ifndef Pacman_h
#define Pacman_h

#include <SFML/Graphics.hpp>
#include <iostream>

#include "Common.h"
#include "Ghost.h"
using namespace sf;
using namespace std;


class Player
{
private:
    int playeri;
    int playerj;
    Sprite Pacman;
    int number_lives;
    Texture pac;
    Texture left;
    Texture up;
    Texture down;
    int score;
    int number_eaten_r_pellets = 0;
    int number_eaten_p_pellets = 0;
    int number_eaten_ghosts = 0;
    int max_score;
    int max_pellet= 274;
    Text text;
    Font font;
public:
    Player(int x, int y);
    void movePlayer( char direction, char mazeArray[][cols]);
    Sprite& getPlayerSprite();
    void setPlayeri(int x);
    void setPlayerj(int y);
    void setscore(int s);
    void setnumber_eaten_r_pellets(int n);
    void setnumber_eaten_p_pellets(int n);
    void setnumber_eaten_ghosts(int g);
    int getPlayeri();
    int getPlayerj();
    int getscore();
    int getnumber_eaten_r_pellets();
    int getnumber_eaten_p_pellets();
    int getnumber_eaten_ghosts();
    void setnumber_lives(int num);
    int getnumber_lives();
    int calculatescore();
    bool GhostsEatPacman(ghost &g, bool &pacman_alive);
    void DisplayGameOver(bool pacman_alive, RenderWindow &window);
    void DisplayWin(RenderWindow &window, bool &win);
    bool IsGameOver();
    bool eatghosts(ghost g1, bool ghost_alive);
    void displayscore(RenderWindow &window);
    void displaylives(RenderWindow &window);
    void displaylevels(RenderWindow &window);
};
#endif /* Pacman_h */
