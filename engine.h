#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "hud.h"
#include "smartSprite.h"
#include "collisionStrategy.h"
#include "vector2f.h"
#include "sound.h"
#include "explodingSprite.h"
//#include "snowSpritePool.h"
//#include "shootingSprite.h"

class Engine {
public:
  Engine ();
  Engine(const Engine&)=delete;
  Engine& operator=(const Engine&)=delete;
  ~Engine ();
  void play();
  void switchSprite();
	
private:
  const RenderContext* rc;
  const IOmod& io;
  Clock& clock;
  Sprite* ball;
  Player* player;
  SmartSprite* ai;
  Hud* hud;
  
  CollisionStrategy* strategy;
  int collisions;

  SDL_Renderer * const renderer;
  World world;
  //World world2;
  Viewport& viewport;

  std::vector<Drawable*> backgroundSprites;
  
  std::vector<Drawable*> foregroundSprites;

  int currentSprite;
  bool makeVideo;

  void draw() const;
  void update(Uint32);
  float score[2];

  Drawable* checkForCollisions();
  
  bool gameEnd;
  int winScore;
  SDLSound sound;
  
  ExplodingSprite* boom;
};
