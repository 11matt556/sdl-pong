#include <list>
#include "snowSprite.h"
//#include "collisionStrategy.h"

class SnowSpritePool {
public:
  SnowSpritePool(const std::string&);
  SnowSpritePool(const SnowSpritePool&);
  ~SnowSpritePool();
  void draw() const;
  void update(Uint32 ticks);
  void shoot(const Vector2f& pos, const Vector2f& vel);

  unsigned int bulletCount() const { return bulletList.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
  //bool shooting() const { return bulletList.empty(); }
  //bool collidedWith(const Drawable* obj) const;
  
private:
  std::string name;
  //CollisionStrategy* strategy;
  float frameInterval;
  float timeSinceLastFrame;
  mutable std::list<SnowSprite> bulletList; 
  mutable std::list<SnowSprite> freeList;   // when a Bullet is out of range

  SnowSpritePool& operator=(const SnowSpritePool&);   // (Item 6)
};
