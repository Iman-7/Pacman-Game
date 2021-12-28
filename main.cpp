#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Common.h"
#include "Ghost.h"
#include "Player.h"
#include <string>
using namespace sf;
using namespace std;


char mazeArray[rows][cols];        // maze-text-file loaded into 2d array of chars
char pelletsArray[rows][cols];    // pellet-text-file loaded into 2d array of chars
bool draw_pellet[rows][cols];        // boolean array to toggle pellets display when eaten
Sprite maze[rows][cols];            //array of sprites of maze which takes its values from maze-text-file
CircleShape pellets[rows][cols];    //array of circleshape of pellets which takes its values from pellet-textfile
bool frightmode = false;            //bool that shows if ghosts are in fright mode or not
int mazeGraph[graphSize][graphSize];    // graph matrix & used as a dist array inside floyed warshal
Direction dirs[graphSize][graphSize]; // minimum direction from each point to any other point on the minimum path in between
int blinkyintial_x = 9; //intial position
int blinkyintial_y = 17;
int inkyintial_x = 12;
int inkyintial_y = 15;
int pinkyintial_x = 12;
int pinkyintial_y = 17;
int clydeintial_x = 12;
int clydeintial_y = 19;

double level = 90;

void updateGraph(int src, int dest, Direction dir, Direction otherDir) {
    if(mazeGraph[src][dest] != 1){
        mazeGraph[src][dest] = 1;
        mazeGraph[dest][src] = 1;
        dirs[src][dest] = dir;
        dirs[dest][src] = otherDir;
    }
} //establish adjacency and direction matrix depending on intial connection of nodes

void fillNeighbors(int i, int j){  //calls update function to fill the adjacency and direction matrix
    int src, dest;
    src = i*cols + j;
    if(mazeArray[i][j] == 'l') return;
    if(i > 1 && mazeArray[i-1][j] != 'l'){ //up
        dest = (i-1)*cols+j;
        updateGraph(src, dest, upDir,  downDir);
    }
    
    if(i < rows-2 && mazeArray[i+1][j] != 'l'){ //down
        dest = (i+1)*cols+j;
        updateGraph(src, dest, downDir,  upDir);
    }
    
    if(j > 1 && mazeArray[i][j-1] != 'l'){ //left
        dest = i*cols+j-1;
        updateGraph(src, dest, leftDir,  rightDir);
    }
    
    if(j < cols-2 && mazeArray[i][j+1] != 'l'){ //right
        dest = i*cols+j+1;
        updateGraph(src, dest, rightDir,  leftDir);
        
    }
    
    if(j == 0 && mazeArray[rows/2][cols-1] != 'l'){
        dest = (rows/2)*cols + (cols-1); //portals
        updateGraph(src, dest, leftDir,  rightDir);
    }
    
    if(j == 0 && mazeArray[(rows/2)-1][cols-1] != 'l'){
        dest = ((rows/2)-1)*cols + (cols-1);  //portals second rows
        updateGraph(src, dest, leftDir,  rightDir);
    }
}


void floydWarshall (){ //pathfinding algorithm called one time
    // since we aren't going to use the mazeGraph again
    // we can use it as our distance array instead of creating new one
    int i, j, k;
    cout << "Floyd warshall" << endl;
    ofstream output;
    output.open("/Users/rawansameh/Desktop/output.txt");
    for (i = 0; i < graphSize; i++)
    {
        for (j = 0; j < graphSize; j++)
        {   if(mazeGraph[i][j] != INF){
            output << mazeGraph[i][j] << " = ";
            output << "source:" << "(" << i/cols << ", " << i%cols << ")";
            output << ", dest:" << "(" << j/cols << ", " << j%cols << ")";
            output << endl; } //used in testing
        }
        
    }
    ofstream dirsF;
    dirsF.open("/Users/rawansameh/Desktop/dirsF.txt");
    for (k = 0; k < graphSize; k++)
    {
        for (i = 0; i < graphSize; i++)
        {
            for (j = 0; j < graphSize; j++)
            {
                if ((i != j) && (mazeGraph[i][k] + mazeGraph[k][j] < mazeGraph[i][j]))
                {
                    mazeGraph[i][j] = mazeGraph[i][k] + mazeGraph[k][j];
                    dirs[i][j] = dirs[i][k];
                    if(dirs[i][j] != noDir){
                        dirsF << dirs[i][j] << " = ";
                        dirsF << "source:" << "(" << i/cols << ", " << i%cols << ")";
                        dirsF << ", dest:" << "(" << j/cols << ", " << j%cols << ")";
                        dirsF << endl; } //used in testing
                }
            }
            
        }
    }
}

void initgraph(){ // stores shortest paths from each node on the graph to each other node
    for(int i = 0; i<graphSize; i++){
        for(int j = 0; j<graphSize; j++){
            mazeGraph[i][j]= INF;
            dirs[i][j] = noDir;
        }
    }
    
    for(int i = 0; i<rows; i++){
        for(int j = 0; j<cols; j++){
            fillNeighbors(i, j);
        }
    }
    
    floydWarshall();
}


void initMaze(int &playerx, int &playery, Texture &yellow, Texture &black, int &blinkyx, int &blinkyy, int &inkyx, int &inkyy, int &pinkyx, int &pinkyy, int &clydex, int &clydey) //intializes textures on maze tiles
{
    ifstream input;
    input.open("/Users/rawansameh/Desktop/pacman_maze/maze-text-file.txt");
    for(int i =0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            input >> mazeArray[i][j];
            if(mazeArray[i][j] == 'l')
            {   maze[i][j].setTexture(yellow);
                maze[i][j].setPosition(j*cellSize, i*cellSize);
                Vector2u size = yellow.getSize();
                Vector2f scale = Vector2f((float) cellSize/size.x, (float) cellSize/size.y);
                maze[i][j].scale(scale);
            }
            else if(mazeArray[i][j] == 'b')
            {
                maze[i][j].setTexture(black);
                maze[i][j].setPosition(j*cellSize, i*cellSize);
                Vector2u size = black.getSize();
                Vector2f scale = Vector2f((float) cellSize/size.x, (float) cellSize/size.y);
                maze[i][j].scale(scale);
            }
            else if(mazeArray[i][j] == 'r')
            {
                playerx=i;
                playery=j;
            }
            else if(mazeArray[i][j] == 'x')
            {
                blinkyx=i;
                blinkyy=j;
            }
            else if(mazeArray[i][j] == 'w')
            {
                inkyx=i;
                inkyy=j;
            }
            else if(mazeArray[i][j] == 'y')
            {
                pinkyx=i;
                pinkyy=j;
            }
            else if(mazeArray[i][j] == 'z')
            {
                clydex=i;
                clydey=j;
            }
        }
    }
    
    
}
void Reset(Player &p, ghost &b, ghost &i, ghost &pin, ghost &c){ //reset ghosts and player to their original positions (called when pacman loses a life)
    p.setPlayeri(20);
    p.setPlayerj(17);
    b.setghosti(blinkyintial_x);
    b.setghostj(blinkyintial_y);
    i.setghosti(inkyintial_x);
    i.setghostj(inkyintial_y);
    pin.setghosti(pinkyintial_x);
    pin.setghostj(pinkyintial_y);
    c.setghosti(clydeintial_x);
    c.setghostj(clydeintial_y);
    
}

void initPellets(Texture &purple, Texture &power) //intializes textures to the pellets drawn on the maze
{
    ifstream file2;
    file2.open("/Users/rawansameh/Desktop/pacman_maze/pellet-text-file.txt");
    
    for(int i =0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            file2 >> pelletsArray[i][j];
            
            if(pelletsArray[i][j] == 'o')
            {
                pellets[i][j].setTexture(&purple);
                pellets[i][j].setRadius(12);
                pellets[i][j].setPosition(j*cellSize, i*cellSize);
                Vector2u size = purple.getSize();
                Vector2f scale = Vector2f((float) cellSize/size.x, (float) cellSize/size.y);
                pellets[i][j].scale(scale);
                draw_pellet[i][j] = true;
            }
            else if(pelletsArray[i][j] == 'p')
            {
                pellets[i][j].setTexture(&power);
                pellets[i][j].setRadius(30);
                pellets[i][j].setPosition(j*cellSize, i*cellSize);
                Vector2u size = power.getSize();
                Vector2f scale = Vector2f((float) cellSize/size.x, (float) cellSize/size.y);
                pellets[i][j].scale(scale);
                draw_pellet[i][j] = true;
            }
        }
    }
}


void checkPlayerPelletsCollision(Player &p, Clock &c) { //checks collision between pellets and pacman
    
    if (pelletsArray[p.getPlayeri()][p.getPlayerj()] =='o') {
        for (int i=0; i<rows; i++) {
            for (int j=0; j<cols; j++) {
                if(draw_pellet[i][j] && i==p.getPlayeri() && j==p.getPlayerj())   {                 draw_pellet[i][j]=false;
                    p.setnumber_eaten_r_pellets(p.getnumber_eaten_r_pellets()+1);
                    
                }
            }
        }
    }
    if (pelletsArray[p.getPlayeri()][p.getPlayerj()] =='p'){
        for (int i=0; i<rows; i++) {
            for (int j=0; j<cols; j++) {
                if(draw_pellet[i][j] && i==p.getPlayeri() && j==p.getPlayerj()) {
                    draw_pellet[i][j]=false;
                        if(!frightmode)
                    {
                            frightmode = true;
                            c.restart();
                        
                    }
                    p.setnumber_eaten_p_pellets(p.getnumber_eaten_p_pellets()+1);
                
                }
            }
        }
    }
}

bool ghostmovement(int i, int j, ghost &ghosty, Player &p, bool bl, bool pacman_alive, bool ghost_alive) //Sprite &ghost1)
{
    int kg = ghosty.getghosti()*cols + ghosty.getghostj();
    Direction d;
    int kp;
   
    if(bl)
    {
        if(frightmode)
        {
            kp = i*cols + j;
            
            if(p.eatghosts(ghosty, ghost_alive))
            {
                ghosty.setghosti(i);
                ghosty.setghostj(j);
    
            }
        }
        else
        {
            kp = p.getPlayeri()*cols+ p.getPlayerj();
        }
        
        d = dirs[kg][kp];
       
    }
    else
    {
        if(frightmode)
        {
            kp = i*cols +j;
            d = dirs[kg][kp];
        }
        else
        {
        do
        {
            int x, y;
            int start, end;
            
            start = p.getPlayeri()-5;
            if(start < 0)
                start = 0;
            end = (p.getPlayeri()+5)%rows;
            if(start > end)
                swap(start, end);
            x = (start+(rand()%(end-start)))% rows;
            
            start = p.getPlayerj()-5;
            if(start < 0)
                start = 0;
            end = (p.getPlayerj()+5)%cols;
            if(start > end)
                swap(start, end);
            
            y = (start+(rand()%(end-start)))% cols;
            kp = x*cols + y;
            
            d = dirs[kg][kp];
        } while(d==noDir);
        }
       
        
       // cout << "source:" << "(" << kg/cols << ", " << kg%cols << ")" << d; //displays direction between nodes(used while testing the ghosts movements)
        //cout << ", dest:" << "(" << kp/cols << ", " << kp%cols << ")";
        //cout << endl;
    }
    if(pacman_alive)
    {
        if(!frightmode)
        {
             pacman_alive = !p.GhostsEatPacman(ghosty, pacman_alive);
        }
       
    }

    if(pacman_alive)
    {
        ghosty.moveGhost(d,frightmode);

        if(!frightmode)
        {
             return !p.GhostsEatPacman(ghosty, pacman_alive);
        }
        else if(p.eatghosts(ghosty, ghost_alive))
        {
            ghosty.setghosti(i);
            ghosty.setghostj(j);
     
            }
    }
    
        
    return pacman_alive;
}



int main() {
    // Create the main window
    RenderWindow window(VideoMode(cols*cellSize, rows*cellSize), "Pacman Game");
    
    // Maze and Ghosts
    Texture black, yellow;
    black.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/black.jpeg");
    yellow.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/yellow-new.png");
    
    int playerx, playery;
    int blinkyx, blinkyy, inkyx, inkyy, pinkyx, pinkyy, clydex, clydey;
    
    initMaze(playerx, playery, yellow, black ,blinkyx, blinkyy, inkyx, inkyy, pinkyx, pinkyy, clydex, clydey);
    Player p(playerx, playery);
    
    
    // Ghosts
    
    Texture  blink, pink, clyd, ink, fright;
    blink.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/blinky2.png");
    pink.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/pinky2.png");
    clyd.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/clyde2.png");
    ink.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/inky2.png");
   
    
    ghost Blinky(blinkyx, blinkyy, blink);
    ghost Pinky(pinkyx, pinkyy, pink);
    ghost Clyde(clydex, clydey, clyd);
    ghost Inky(inkyx, inkyy, ink);
    
    
    // Pellets
    Texture purple, power;
    purple.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/peee.png");
    power.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/unnamed.png");
    initPellets(purple, power);
    
    bool testBlinky = false;
    //setting the initial number of lives at game start
    p.setnumber_lives(3);
    
    // FloyedWarshall to calculate minimum directions
    initgraph();
    Clock clockGhosts, clockBlinky, clockPac, clockfright;
    
    Direction d;
    int kp = p.getPlayeri()*cols+ p.getPlayerj();
    bool pacman_alive = true;
    bool playerwin = false;
    bool ghost_alive = true;
    // Start the game loop -- this is where my logic happens
    Clock s, display;

    SoundBuffer buffer;
    if (!buffer.loadFromFile("/Users/rawansameh/Desktop/Pacman/Pacman/pacman_beginning.wav"))
        return -1;
    Sound sound;
    sound.setBuffer(buffer);
    sound.play();
    

    while (window.isOpen())
    {
        if(s.getElapsedTime().asSeconds() >= 4)
        {
            s.restart();
            sound.play();
        }

        if(pacman_alive  && playerwin == false){
            if(clockGhosts.getElapsedTime().asSeconds() >= level){
                if(clockPac.getElapsedTime().asSeconds() >= 3) {
                    kp = p.getPlayeri()*cols+ p.getPlayerj();
                    clockPac.restart();
                }
                
                pacman_alive = ghostmovement(blinkyintial_x, blinkyintial_y,Blinky, p, true, pacman_alive, ghost_alive);
                
                 if(clockBlinky.getElapsedTime().asSeconds() >= 2)
                {
                    pacman_alive = ghostmovement(pinkyintial_x, pinkyintial_y, Pinky, p, false, pacman_alive, ghost_alive);
                }
                if(clockBlinky.getElapsedTime().asSeconds() >= 6)
                
                {
                    pacman_alive = ghostmovement(clydeintial_x, clydeintial_y, Clyde, p, false, pacman_alive, ghost_alive);
                }
                if(clockBlinky.getElapsedTime().asSeconds() >= 8)
                {
                    pacman_alive = ghostmovement(inkyintial_x, inkyintial_y, Inky, p, false, pacman_alive, ghost_alive);
                }
                
                // the direction stuff
                clockGhosts.restart();
            }
            checkPlayerPelletsCollision(p, clockfright);
          
            if(clockfright.getElapsedTime().asSeconds() > 10){
                frightmode = false;
                
            }
            cout << "Your score is " << p.calculatescore();
        }
        else {
            if(!p.IsGameOver()){
                Reset( p, Blinky, Inky, Pinky, Clyde);
                clockGhosts.restart();
                clockBlinky.restart();
                clockPac.restart();
                pacman_alive = true;
            }
            
        }
        
        // Process events
        Event event;

        while (window.pollEvent(event))
        {     
            // Close window: exit
            if (event.type == Event::Closed)
            {
                window.close();
            }
            
            if(pacman_alive && playerwin == false && event.type == Event::KeyPressed)
            {
                switch (event.key.code) {
                    case Keyboard::Escape:
                        window.close();
                        break;
                    case Keyboard::Num1:
                        level = 2;
                        break;
                    case Keyboard::Num2:
                        level = 1;
                        break;
                    case Keyboard::Num3:
                        level = 0.5;
                        break;
                    case Keyboard::Left:
                        p.movePlayer('L', mazeArray);
                        break;
                        
                    case Keyboard::Right:
                        p.movePlayer ('R', mazeArray);
                        break;
                        
                    case Keyboard::Up:
                        p.movePlayer ('U', mazeArray);
                        break;
                        
                    case Keyboard::Down:
                        p.movePlayer ('D', mazeArray);
                        break;
                        
                    default:
                        break;
                }
            }
        }
        
        
        
        // Clear screen
        window.clear();
        
        // Draw any of the things that I declared to the screen that I want to be visible
        for(int i =0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                window.draw(maze[i][j]);
            }
        }
        
        
        for(int i =0; i < rows; i++)
        {
            for(int j = 0; j < cols; j++)
            {
                if(draw_pellet[i][j] == true)
                    window.draw(pellets[i][j]);
            }
        }
        
        window.draw(p.getPlayerSprite());
        window.draw(Blinky.getghostSprite());
        window.draw(Inky.getghostSprite());
        window.draw(Pinky.getghostSprite());
        window.draw(Clyde.getghostSprite());
        
        
        p.DisplayGameOver(pacman_alive, window);
        p.DisplayWin(window, playerwin);
        p.displayscore(window);
        p.displaylives(window);
        if(frightmode)
        {
            Blinky.displayfright(window);
        }
        if(display.getElapsedTime().asSeconds() <=4){
            p.displaylevels(window);
        }


        // Update the window
        window.display();
        
    }
    
    
    return 0;
}
