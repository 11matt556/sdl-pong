#include <cmath>
#include "sprite.h"
#include "gamedata.h"
#include "renderContext.h"

Sprite::Sprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) 
           ),
  frame( RenderContext::getInstance()->getFrame(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  isExplode(false)
{ }

Sprite::Sprite(const std::string& name, const Vector2f& pos, 
               const Vector2f& vel):
  Drawable(name, pos, vel), 
  frame( RenderContext::getInstance()->getFrame( name ) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  isExplode(false)
{ }

Sprite::Sprite(const Sprite& s) :
  Drawable(s), 
  frame(s.frame),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  isExplode(s.isExplode)
{ }

Sprite& Sprite::operator=(const Sprite& rhs) {
  Drawable::operator=( rhs );
  frame = rhs.frame;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  return *this;
}

void Sprite::draw() const { 
  frame->draw(getX(), getY()); 
}

void Sprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  

  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-frameHeight) {
    setVelocityY( -std::abs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( std::abs( getVelocityX() ) );
        isExplode = true;
        
             //std::cout << isExplode << std::endl;
  }
  if ( getX() > worldWidth-frameWidth) {
    setVelocityX( -std::abs( getVelocityX() ) );
     isExplode = true;
     
     //std::cout << isExplode << std::endl;
  }  
  
  
}

void Sprite::bounce(){
	
	std::cout << getVelocityX() << std::endl;

	if ( getVelocityY() <= 0) {
		setVelocityY( -std::abs( getVelocityY() ) );
		
	}
	else if ( getVelocityY() >=0) {
		setVelocityY( std::abs( getVelocityY() ) );
	}
	if(getVelocityX() <= 0){
		setVelocityX( std::abs( getVelocityX() ) );
	}
	else if(getVelocityX() >= 0){
		setVelocityX( -std::abs( getVelocityX() ) );
	}
}

