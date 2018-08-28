#include "twoWayMultiSprite.h"

TwoWayMultiSprite::TwoWayMultiSprite(const std::string& name) : MultiSprite(name), flip(SDL_FLIP_NONE)  {}

TwoWayMultiSprite::TwoWayMultiSprite(const TwoWayMultiSprite& s) : MultiSprite(s), flip(s.flip) {}

void TwoWayMultiSprite::draw() const{
	frames[currentFrame]->draw(getX(),getY(),flip);
}

void TwoWayMultiSprite::update(Uint32 ticks){
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-frameHeight) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    TwoWayMultiSprite::flip = SDL_FLIP_NONE;
        //std::cout<<"FLIP NONE"<<std::endl;
  }
  if ( getX() > worldWidth-frameWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
    TwoWayMultiSprite::flip = SDL_FLIP_HORIZONTAL;
    //std::cout<<"FLIP H"<<std::endl;
  }  

}
