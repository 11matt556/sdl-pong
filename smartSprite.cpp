#include <cmath>
#include "viewport.h"
#include "smartSprite.h"
#include <sstream>

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

SmartSprite::SmartSprite(const std::string& name, const Vector2f& pos, const Drawable& p) :
 Sprite(name, pos, 
   Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
            Gamedata::getInstance().getXmlInt(name+"/speedY"))
 ), 
 io(IOmod::getInstance()), 
 enemy(p), 
 currentMode(CHASE), 
 moveSpeed(Gamedata::getInstance().getXmlInt(name+"/moveSpeed"))
{ }

//void SmartSprite::goLeft()  { setVelocityX( -20 ); }
//void SmartSprite::goRight() { setVelocityX( 20 ); }
//void SmartSprite::goUp()    { setVelocityY( -20); }
//void SmartSprite::goDown()  { setVelocityY( 20 ); }

//Direction can be 8,2,4,6 for UP, DOWN, LEFT, RIGHT corresponding to the numpad
void SmartSprite::moveDirection(float speed, int direction){ 
	if(direction == 8){
		setVelocityY(-speed);
	}
	else if(direction == 2){
		setVelocityY(speed);
		
	}
	else if(direction == 4){
		setVelocityX(-speed);
		if(speed != 0){
			//needToFlip = true;
		}
	}
	else if(direction == 6){
		setVelocityX(speed);
		if(speed != 0){
			//needToFlip = false;
		}	
	}
	else{
		setVelocityX(0);
		setVelocityY(0);
		std::cout<<"Invalid Direction"<<std::endl;
	}
	
}

void SmartSprite::draw() const {
  Sprite::draw();
  int x = 10 + getX() - Viewport::getInstance().getX();
  int y = 10 + getY() - Viewport::getInstance().getY();
  std::stringstream strm;
  strm << currentMode;
  io.writeText( strm.str(), x, y); 
  
  
   //std::stringstream strm;
  //std::stringstream strm2;
  
  //strm << "Fps: " << clock.getFps();
  //strm2 << "Avgfps: " << clock.getAvgFps();
  
  //SDL_Color custColor = {255,165,0,255};

  //io.writeText(strm.str(), 30, 60);
  //io.writeText(strm2.str(), 30, 120, custColor);
  
}

void SmartSprite::update(Uint32 ticks) {
    Sprite::update(ticks);
    
  try{
    
  float x= getX()+getFrame()->getWidth()/2;
  float y= getY()+getFrame()->getHeight()/2;
  

  
  float ex= enemy.getX()+enemy.getFrame()->getWidth()/2;
  float ey= enemy.getY()+enemy.getFrame()->getHeight()/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );
  
  
  //int speed = 10;
  

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < interactionDistance) currentMode = EVADE;
  }
  else if  ( currentMode == EVADE ) {
    if(distanceToEnemy > interactionDistance) currentMode=NORMAL;
    else {
      if ( x < ex ) moveDirection(moveSpeed, 4);
      if ( x > ex ) moveDirection(moveSpeed, 6);
      if ( y < ey ) moveDirection(moveSpeed, 8);
      if ( y > ey ) moveDirection(moveSpeed, 2);
    }
  }
  else if ( currentMode == CHASE ){
	  if(distanceToEnemy > interactionDistance) currentMode=NORMAL;
    else {
      //if ( x > ex ) moveDirection(moveSpeed, 4);
      //if ( x < ex ) moveDirection(moveSpeed, 6);
      if ( y > ey ) moveDirection(moveSpeed, 8);
      if ( y < ey ) moveDirection(moveSpeed, 2);
    }
  }
  

    
    if(getX() < 0){
		  setX(0);
		  moveDirection(0,4);
	}    
    if(getX() > getWorldWidth() - getFrameWidth() ){
		  setX(getWorldWidth() - getFrameWidth());
		  moveDirection(0,6);
	} 
	if(getY() < 0){
		  setY(0);
		  moveDirection(0,8);
	} 
    if(getY() > getWorldHeight() - getFrameHeight() ){
		  setY(getWorldHeight() - getFrameHeight());
		  moveDirection(0,2);
	}
    }
    catch(...){}
    
}

