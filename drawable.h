#ifndef DRAWABLE__H
#define DRAWABLE__H
#include <SDL.h>
#include <iostream>
#include <string>
#include "vector2f.h"
#include "frame.h"
 
// Drawable is an Abstract Base Class (ABC) that
// specifies the methods that derived classes may
// and must have.
class Drawable {
public:
  Drawable(const std::string& n, const Vector2f& pos, const Vector2f& vel, const float s): 
    name(n), position(pos), velocity(vel), scale(s) {}
    
  Drawable(const std::string& n, const Vector2f& pos, const Vector2f& vel): 
    name(n), position(pos), velocity(vel), scale(1) {}

  Drawable(const Drawable& s) : 
    name(s.name), position(s.position), velocity(s.velocity), scale(s.scale)
    { }

  virtual ~Drawable() {}

  virtual void draw() const = 0;
  virtual void update(Uint32 ticks) = 0;

  //virtual float getScale() const { return 0; }
  
  void setScale(float s) { scale = s; }
  virtual float getScale() const { return scale; }
  
  virtual void setScaleX(float x){ setX(getX()*x); }
  virtual void setScaleY(float y){ setY(getY()*y); }

  const std::string& getName() const { return name; }
  void setName(const std::string& n) { name = n;    }

  virtual const Frame* getFrame() const = 0;

  float getX() const  { return position[0]; }
  void  setX(float x) { position[0] = x;    }

  float getY() const  { return position[1]; }
  void  setY(float y) { position[1] = y;    }

  float getVelocityX() const   { return velocity[0]; }
  void  setVelocityX(float vx) { velocity[0] = vx;   }
  float getVelocityY() const   { return velocity[1]; }
  void  setVelocityY(float vy) { velocity[1] = vy;   }

  const Vector2f& getVelocity() const    { return velocity; }
  void  setVelocity(const Vector2f& vel) { velocity = vel;  }
  const Vector2f& getPosition() const    { return position; }
  void  setPosition(const Vector2f& pos) { position = pos;  }
  
  //Drawable& operator=(const Drawable&);
  /*
  Drawable& operator=(const Drawable& rhs){
  name = rhs.name;
  position = rhs.position;
  velocity = rhs.velocity;
  return *this;
	};
  */
  

private:
  std::string name;
  Vector2f position;
  Vector2f velocity;
  float scale;

};
#endif
