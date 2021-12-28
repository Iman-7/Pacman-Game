#include "Player.h"
#include <sstream>
Player::Player(int x, int y)
{
	playeri=x;
	playerj=y;
	Pacman.setPosition(playerj*cellSize, playeri*cellSize);
    pac.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/Pacmannnn.png");
    Pacman.setTexture(pac);
    Vector2u size = pac.getSize();
    Vector2f scale = Vector2f((float) cellSize/size.x, (float) cellSize/size.y);
    Pacman.setScale(scale);
    number_lives = 3;
    font.loadFromFile("/Users/rawansameh/Desktop/Pacman/Pacman/sansation.ttf");
    text.setFont(font); // font is a sf::Font
    
    // set the string to display
   
    
    // set the character size
    text.setCharacterSize(24); // in pixels, not points!
    
    // set the color
    text.setFillColor(sf::Color::Red);
    
    // set the text style
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    
    
   
    
}
void Player::movePlayer( char direction, char mazeArray[][cols])
{
	switch (direction)
	{
		case'L':
			
            if(playerj == 0 && (playeri == rows/2 || playeri == (rows/2)-1)){
                playerj = cols-1;
                left.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/left.png");
                Pacman.setTexture(left);
                Vector2u size = left.getSize();
                Vector2f scale = Vector2f((float) cellSize/size.x, (float) cellSize/size.y);
                Pacman.setScale(scale);
                Pacman.setPosition(cellSize*(cols-1), Pacman.getPosition().y);
                
            }
            else if(mazeArray[playeri][playerj-1] != 'l' && playerj>0)
            {
                Pacman.move(-cellSize,0);
                left.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/left.png");
                Pacman.setTexture(left);
                Vector2u size = left.getSize();
                Vector2f scale = Vector2f((float) cellSize/size.x, (float) cellSize/size.y);
                Pacman.setScale(scale);
                playerj--;
               
               
            }
            break;
            
        case'R':
            if(playerj == cols-1 && (playeri == rows/2 || playeri == (rows/2)-1)){
                playerj = 0;
                pac.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/Pacmannnn.png");
                Pacman.setTexture(pac);
                Vector2u size = pac.getSize();
                Vector2f scale = Vector2f((float) cellSize/size.x, (float) cellSize/size.y);
                Pacman.setScale(scale);
                Pacman.setPosition(0, Pacman.getPosition().y);
                
                
            }
            else if(mazeArray[playeri][playerj+1] != 'l' && playerj< cols-1)
            {
                Pacman.move(cellSize,0);
                pac.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/Pacmannnn.png");
                Pacman.setTexture(pac);
                Vector2u size = pac.getSize();
                Vector2f scale = Vector2f((float) cellSize/size.x, (float) cellSize/size.y);
                Pacman.setScale(scale);
                playerj++;
               
                
            }
            break;
			
		case'U':
			if(mazeArray[playeri-1][playerj] != 'l' && playeri>0)
			{
				Pacman.move(0,-cellSize);
                up.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/up.png");
                Pacman.setTexture(up);
                Vector2u size = up.getSize();
                Vector2f scale = Vector2f((float) cellSize/size.x, (float) cellSize/size.y);
                Pacman.setScale(scale);
				playeri--;
                
              
			}
            
			break;
			
		case'D':
			if(mazeArray[playeri+1][playerj] != 'l' && playeri < rows-1)
			{
				Pacman.move(0,cellSize);
                down.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/down.png");
                Pacman.setTexture(down);
                Vector2u size = down.getSize();
                Vector2f scale = Vector2f((float) cellSize/size.x, (float) cellSize/size.y);
                Pacman.setScale(scale);
				playeri++;
                
               
			}
			break;
	}
}

Sprite& Player::getPlayerSprite ()
{
	return Pacman;
}

void Player::setPlayeri(int x){
	playeri = x;
    Pacman.setPosition(playerj*cellSize, playeri*cellSize);
}
void Player::setPlayerj(int y){
	playerj = y;
    Pacman.setPosition(playerj*cellSize, playeri*cellSize);
}
void Player::setscore(int s){
	score = s;
}
void Player::setnumber_eaten_r_pellets(int n){
	number_eaten_r_pellets = n;
}
void Player::setnumber_eaten_p_pellets(int n){
	number_eaten_p_pellets = n;
}
void Player::setnumber_eaten_ghosts(int g){
	number_eaten_ghosts = g;
}

int Player::getPlayeri(){
	return playeri;
}
int Player::getPlayerj(){
	return playerj;
}
int Player::getscore(){
	return score;
}
int Player::getnumber_eaten_r_pellets(){
	return number_eaten_r_pellets;
}
int Player::getnumber_eaten_p_pellets(){
	return number_eaten_p_pellets;
}
int Player::getnumber_eaten_ghosts(){
	return number_eaten_ghosts;
}

void Player::setnumber_lives(int num) {
    number_lives = num;
}
int Player::getnumber_lives() {
    return number_lives;
}

int Player::calculatescore()
{
    score = number_eaten_r_pellets * 10 + number_eaten_p_pellets * 50;
    int g = number_eaten_ghosts;
    if (g == 1)
        score = number_eaten_r_pellets * 10 + number_eaten_p_pellets * 50 + 200;
    else if (g == 2)
        score = number_eaten_r_pellets * 10 + number_eaten_p_pellets * 50 + 600;
    else if (g == 3)
        score = number_eaten_r_pellets * 10 + number_eaten_p_pellets * 50 + 1400;
    else if (g == 4)
        score = number_eaten_r_pellets * 10 + number_eaten_p_pellets * 50 + 3000;
   if(score == 1770){
        setnumber_lives(number_lives++);
    }
    return score;
}

bool Player::GhostsEatPacman(ghost &g, bool &pacman_alive){
    if(getPlayeri() == g.getghosti() && getPlayerj() == g.getghostj()){
          pacman_alive = false;
        setnumber_lives(getnumber_lives()-1);
        return true;
    }
    return false;
}
    
    void Player::DisplayGameOver(bool pacman_alive, RenderWindow &window){
        if(number_lives==0) {
           
            // select the font
             text.setString("Game Over");
            text.setPosition(3*cellSize, 0*cellSize);
            pacman_alive = false;
            
            // inside the main loop, between window.clear() and window.display()
              window.draw(text);
            
            
        }
    }
void Player::DisplayWin(RenderWindow &window, bool &win){
    if(number_eaten_r_pellets+number_eaten_p_pellets  >= max_pellet){
        text.setString("You Win!");
        text.setPosition(3*cellSize, 0*cellSize);
        if(!win){
            win = true;
        }
        window.draw(text);
        
    }
}

bool Player::IsGameOver(){
    
    if(number_lives == 0)
        return true;
        
    return false;
    
}
bool Player::eatghosts(ghost g1, bool ghost_alive){
    if(getPlayeri() == g1.getghosti() && getPlayerj() == g1.getghostj()){
        ghost_alive = false;
        setnumber_eaten_ghosts(getnumber_eaten_ghosts()+1);
        return true;
        
    }
    return false;
    
}

void Player::displayscore(RenderWindow &window)
{
    Text text1;
    text1.setString("Your Score: ");
    text1.setPosition(25*cellSize,0*cellSize);
    text1.setFont(font);
    text1.setCharacterSize(24);
    text1.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    window.draw(text1);
    stringstream ss;
    ss << score;
    text.setString(ss.str());
    text.setPosition(30*cellSize, 0*cellSize);
    window.draw(text);
    
}
void Player::displaylives(RenderWindow &window){
    Text text1;
    text1.setString("Number of lives: ");
    text1.setPosition(2*cellSize,25*cellSize);
    text1.setFont(font);
    text1.setCharacterSize(24);
    text1.setFillColor(sf::Color::Red);
    text1.setStyle(sf::Text::Bold | sf::Text::Underlined);
    window.draw(text1);
    stringstream ss;
    ss << number_lives;
    text.setString(ss.str());
    text.setPosition(9*cellSize, 25*cellSize);
    window.draw(text);
}
void Player::displaylevels(RenderWindow &window){
    Text text1;
    text1.setString("Please enter level:  1) Easy   2) Intermediate   3) Hard");
    text1.setPosition(7*cellSize,14*cellSize);
    text1.setFont(font);
    text1.setCharacterSize(27);
    text1.setFillColor(sf::Color::White);
    text1.setStyle(sf::Text::Bold);
    window.draw(text1);
}
