#include <string>
#include <iostream>
#include "sprite.h"
#include "snowSpritePool.h"

class ShootingSprite : public Sprite {
public:
  ShootingSprite(const std::string& n);
  ShootingSprite(const ShootingSprite& s);

  virtual void update(Uint32 ticks);
  virtual void draw() const;
  void shoot();
  void emit();
  //virtual bool collidedWith(const Drawable*) const;
  
  unsigned int getBulletCount(){return bullets.bulletCount();}
  unsigned int getFreeCount(){return bullets.freeCount();}

private:
  std::string bulletName;
  SnowSpritePool bullets;
  float minSpeed;
  ShootingSprite& operator=(const ShootingSprite&);
};
