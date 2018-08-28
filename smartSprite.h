#ifndef SSPRITE__H
#define SSPRITE__H
#include <iostream>
#include "sprite.h"
#include "ioMod.h"


class SmartSprite : public Sprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, const Drawable& p);
  void draw() const;
  void update(Uint32 ticks);
  //void goLeft();
  //void goRight();
  //void goUp();
  //void goDown();

  static void incrInteractionDistance() {++interactionDistance; }
  static void decrInteractionDistance() { --interactionDistance; }
  static float getInteractionDistance() { return interactionDistance; }
  
  void moveDirection(float speed, int direction); 

private:	
  enum MODE {NORMAL, EVADE, CHASE};
  IOmod& io;	
  const Drawable & enemy;
  static float interactionDistance;
  //float safeDistance;
  MODE currentMode;
  SmartSprite(const SmartSprite &);
  SmartSprite& operator=(const SmartSprite &rhs);
  float moveSpeed;
};
#endif
