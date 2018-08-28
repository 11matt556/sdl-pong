#ifndef SPRITE__H
#define SPRITE__H
#include <string>
#include "drawable.h"

class Sprite : public Drawable {
public:
  Sprite(const std::string&);
  Sprite(const Sprite&);
  Sprite(const std::string& name, const Vector2f&, const Vector2f&);
  virtual ~Sprite() { } 
  Sprite& operator=(const Sprite&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  
  void setFrame(const Frame* f) {frame = f; }
  
  		int getFrameWidth() {return frameWidth;}
		int getFrameHeight() {return frameHeight;}
		int getWorldWidth() {return worldWidth;}
		int getWorldHeight() {return worldHeight;}
		
  void bounce();
  
  bool getExplode(){return isExplode; }
  void setExplode(bool t){ isExplode = t;}

  
private:
  const Frame * frame;
  int worldWidth;
  int worldHeight;
  int frameWidth;
  int frameHeight;
  int getDistance(const Sprite*) const;
  bool isExplode;

};
#endif
