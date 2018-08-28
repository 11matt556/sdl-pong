#include "player.h"
#include "ioMod.h"
#include "gamedata.h"

Player::Player(const std::string& name) : TwoWayMultiSprite(name), needToFlip(false), moveSpeed(Gamedata::getInstance().getXmlInt(name+"/moveSpeed"))  {}

Player::Player(const Player& p) : TwoWayMultiSprite(p), needToFlip(p.needToFlip), moveSpeed(p.moveSpeed) {}

//Player& Player::getInstance(const std::string& name) {
  //static Player player1(name);
 // return player1;
//}


//Direction can be 8,2,4,6 for UP, DOWN, LEFT, RIGHT corresponding to the numpad
void Player::moveDirection(float speed, int direction){ 
	if(direction == 8){
		setVelocityY(-speed);
	}
	else if(direction == 2){
		setVelocityY(speed);
		
	}
	else if(direction == 4){
		setVelocityX(-speed);
		if(speed != 0){
			needToFlip = true;
		}
	}
	else if(direction == 6){
		setVelocityX(speed);
		if(speed != 0){
			needToFlip = false;
		}	
	}
	else{
		setVelocityX(0);
		setVelocityY(0);
		std::cout<<"Invalid Direction"<<std::endl;
	}
	
}

void Player::update(Uint32 ticks){
	advanceFrame(ticks);

	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	setPosition(getPosition() + incr);
  
	if(needToFlip == true){
		TwoWayMultiSprite::flip = SDL_FLIP_HORIZONTAL;
	}
	else{
	    TwoWayMultiSprite::flip = SDL_FLIP_NONE;
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

void Player::draw()const{
	frames[currentFrame]->draw(getX(),getY(),flip);
}

