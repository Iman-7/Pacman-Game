#include "Ghost.h"

ghost::ghost(int x, int y, Texture tx){
	ghosti = x;
	ghostj = y;
	ghost_tx = tx;
    Vector2u size = tx.getSize();
    Vector2f scale = Vector2f((float) cellSize/size.x, (float) cellSize/size.y);
    ghost1.setTexture(ghost_tx);
	
	ghost1.setPosition(ghostj*cellSize,ghosti*cellSize);
	
	ghost1.scale(scale);
    
    fnt.loadFromFile("/Users/rawansameh/Desktop/Pacman/Pacman/sansation.ttf");
    txt.setFont(fnt);
    txt.setCharacterSize(24);
    txt.setFillColor(sf::Color::Blue);
    txt.setStyle(sf::Text::Bold | sf::Text::Underlined);

}
void ghost::setghostSprite(Sprite g){
    ghost1 = g;
    ghost1.setPosition(ghostj*cellSize, ghosti*cellSize);
}
Sprite ghost::getghostSprite ()
{
	return ghost1;
}
void ghost::setghosti(int x){
	ghosti = x;
    ghost1.setPosition(ghostj*cellSize, ghosti*cellSize);
}
void ghost::setghostj(int y){
	ghostj = y;
    ghost1.setPosition(ghostj*cellSize, ghosti*cellSize);
}
int ghost::getghosti(){
	return ghosti;
}
int ghost::getghostj(){
	return ghostj;
}

void ghost::moveGhost(Direction d1, bool &frightmode)
{    if(frightmode)
{
    fright.loadFromFile("/Users/rawansameh/Desktop/pacman_maze/Frightmode.png");
    ghost1.setTexture(fright);
    Vector2u size = fright.getSize();
    Vector2f scale = Vector2f((float) cellSize/size.x, (float) cellSize/size.y);
    ghost1.setScale(scale);
}
    else
    {
        ghost1.setTexture(ghost_tx);
    }

	switch(d1)
    {
            case leftDir:
            if(ghostj == 0)
            {
                ghostj = cols-1;
                ghost1.setPosition(cellSize*ghostj, ghost1.getPosition().y);
               
            }
            else
            {
                
                ghost1.move(-cellSize,0);
                ghostj--;
            }
         
            break;
            
        case rightDir:
            if(ghostj == cols-1)
            {
                ghostj = 0;
                
                ghost1.setPosition(ghostj, ghost1.getPosition().y);
                
            }
            else
            {
                
                ghost1.move(cellSize,0);
                
                ghostj++;
            }
            break;
		case upDir:
            
            ghost1.move(0,-cellSize);
            
			ghosti--;
			break;
			
		case downDir:
            
            ghost1.move(0,cellSize);
           
			ghosti++;
			break;
			default:
			break;
	}
            
}
void ghost::displayfright(RenderWindow &window){
    
    txt.setString("Fright Mode");
    window.draw(txt);
}
