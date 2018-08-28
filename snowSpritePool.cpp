#include <iostream>
#include <sstream>
#include <cmath>
#include "ioMod.h"
#include "gamedata.h"
#include "snowSpritePool.h"

SnowSpritePool::SnowSpritePool(const std::string& n) :
  name(n),
  frameInterval(Gamedata::getInstance().getXmlInt(name+"/interval")),
  timeSinceLastFrame( 0 ),
  bulletList(),
  freeList()
{ }

SnowSpritePool::SnowSpritePool(const SnowSpritePool& b) :
  name(b.name),
  frameInterval(b.frameInterval),
  timeSinceLastFrame( b.timeSinceLastFrame ),
  bulletList(b.bulletList), 
  freeList(b.freeList)
{ }

SnowSpritePool::~SnowSpritePool() {
  //delete strategy;
}
/*
bool BulletPool::collidedWith(const Drawable* obj) const {
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    if ( strategy->execute(*ptr, *obj) ) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}
*/

void SnowSpritePool::shoot(const Vector2f& position, const Vector2f& velocity) {
	if (timeSinceLastFrame > frameInterval) {
    // If no bullets in pool, make one:
    if ( freeList.empty() ) {
      SnowSprite b(name);
      b.setPosition( position );
      b.setVelocity( velocity );
      bulletList.push_back( b );
    }
    else {
      SnowSprite b = freeList.front();
      freeList.pop_front();
      b.reset();
      b.setVelocity(velocity);
      b.setPosition(position);
      bulletList.push_back( b );
    }
		timeSinceLastFrame = 0;
	}
}

void SnowSpritePool::draw() const { 
  std::stringstream stream;
  stream << "Active bullets: " << bulletList.size();
  //IOmod::getInstance().writeText(stream.str(), 500, 30);
  stream.clear();
  stream.str("");
  stream << "Bullet pool: " << freeList.size();
  //IOmod::getInstance().writeText(stream.str(), 500, 60);
  for ( SnowSprite bullet : bulletList ) {
    bullet.draw();
  }
}

void SnowSpritePool::update(Uint32 ticks) { 
	timeSinceLastFrame += ticks;
  std::list<SnowSprite>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a Bullet
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }   
    else ++ptr;
  }
  
  	if (timeSinceLastFrame > frameInterval) {
    // If no bullets in pool, make one:
    if ( freeList.empty() ) {
      SnowSprite b(name);
      //b.setPosition( position );
      //b.setVelocity( velocity );
      bulletList.push_back( b );
    }
    else {
      SnowSprite b = freeList.front();
      freeList.pop_front();
      b.reset();
      //b.setVelocity(velocity);
      //b.setPosition(position);
      bulletList.push_back( b );
    }
		timeSinceLastFrame = 0;
	}
  
}
