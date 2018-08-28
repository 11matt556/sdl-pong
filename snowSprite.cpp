#include <cmath>
#include <random>
#include <functional>
#include "snowSprite.h"
#include "gamedata.h"
#include "renderContext.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

const float PI = 4.0f*std::atan(1.0f);

using RADRAND_t = decltype(std::bind(std::declval<std::uniform_real_distribution<float> >(),std::declval<std::mt19937>()));
using NORRAND_t = decltype(std::bind(std::declval<std::normal_distribution<float> >(),std::declval<std::mt19937>()));

std::mt19937 getRand(){
  static std::random_device rd;
  static std::mt19937 twister(rd());
  return twister;
}

RADRAND_t getRadianDist(){
  return std::bind(std::uniform_real_distribution<float>(0.0f,2.0f*PI),getRand());
}

NORRAND_t getNormalDist(float u, float dev){
  return std::bind(std::normal_distribution<float>(u,dev),getRand());
}

Vector2f SnowSprite::makeVelocity(int vx, int vy) const {
  static auto rad = getRadianDist();
  auto nor = getNormalDist(vx,vy);

  float v_rad = rad();
  float v_mag = nor();

  return v_mag*Vector2f(std::cos(v_rad),std::sin(v_rad));
}

Vector2f SnowSprite::makeSnowVelocity(int vx, int vy) const{
	//static auto rad = getRadianDist();
	//auto nor = getNormalDist(vx,vy);
	
  //float v_rad = rad();
  //float v_mag = nor();
  
  //return v_mag*Vector2f(vx,std::sin(v_rad));
  
  int tempvy = Gamedata::getInstance().getRandInRange( vy/2 , vy );

  return Vector2f(vx,-tempvy);
	
}

Vector2f SnowSprite::makeSnowPosition(int vx, int vy) const{
	vx = Gamedata::getInstance().getRandInRange( 0 , (Gamedata::getInstance().getXmlInt("world/width")) );
	vy = Gamedata::getInstance().getRandInRange( 0 , (Gamedata::getInstance().getXmlInt("world/height")) );
	//std::cout << vy << std::endl;
	return Vector2f(vx,vy);
}

SnowSprite::SnowSprite(const std::string& name) :
  Drawable(name,
           makeSnowPosition(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeSnowVelocity(
					Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")),
           Gamedata::getInstance().getXmlInt(name+"/scale")
           ),
  frame( RenderContext::getInstance()->getFrame(name) ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  toofar(false),
  distance(0)
{ 

}


SnowSprite::SnowSprite(const SnowSprite& s) :
  Drawable(s), 
  frame(s.frame),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  //scale(s.getScale())
  toofar(s.goneTooFar()),
  distance(s.distance)
{ }

//SnowSprite::SnowSprite(const SnowSprite& s) : Sprite(s), scale(s.getScale()) {}

SnowSprite& SnowSprite::operator=(const SnowSprite& rhs) {
  Drawable::operator=( rhs );
  frame = rhs.frame;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  //scale = rhs.scale;
  return *this;
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void SnowSprite::draw() const { 
  if(getScale() < SCALE_EPSILON) return;
  frame->draw(getX(), getY(), getScale());
}

void SnowSprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    Vector2f pos = getPosition();
  setPosition(getPosition() + incr);
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );

  if ( getY() < 0) {
    //setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-frameHeight + 50) {
    //setVelocityY( -std::abs( getVelocityY() ) );
    Vector2f resetPos(getX(),-50);
    setPosition(resetPos);
  }

  if ( getX() < 0 - 128) {
    //setVelocityX( std::abs( getVelocityX() ) );
        Vector2f resetPos(worldWidth ,getY());
		setPosition(resetPos);
		
		toofar = true;
		
    
  }
  if ( getX() > worldWidth-frameWidth) {
    //setVelocityX( -std::abs( getVelocityX() ) );
        //Vector2f resetPos(-50,getY());
		//setPosition(resetPos);
  }  
}
