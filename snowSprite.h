#ifndef SNOWSPRITE__H
#define SNOWSPRITE__H
#include <string>
#include "drawable.h"

class SnowSprite : public Drawable {
public:
  SnowSprite(const std::string&);
  SnowSprite(const SnowSprite&);
  virtual ~SnowSprite() { } 
  SnowSprite& operator=(const SnowSprite&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  
  //inline float getScale()const{ return scale; }
  //void  setScale(float s){ scale = s; }
  
  void setFrame(const Frame* f) {frame = f; }
  
  bool goneTooFar() const {return toofar;}
  
  void reset() {
    toofar = false;
    distance = 0;
  }
  
private:
  const Frame * frame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  //float scale;
  int getDistance(const SnowSprite*) const;
  Vector2f makeVelocity(int, int) const;
  Vector2f makeSnowVelocity(int, int) const;
  Vector2f makeSnowPosition(int, int) const;
  bool toofar;
  float distance;
};
#endif
