#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <iomanip>
#include <random>
#include "sprite.h"
#include "multisprite.h"
#include "snowSprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include <typeinfo>
#include "explodingSprite.h"
#include "ioMod.h"
#include "shootingSprite.h"


			//clock.toggleSloMo();
			
class SpriteLess {
public:
  bool operator()(const Drawable* lhs, const Drawable* rhs) const {
    return ( ( lhs->getScale()  * ( lhs->getFrame()->getWidth() ) + ( lhs->getFrame()->getHeight() ) ) <   ( rhs->getScale() * ( rhs->getFrame()->getWidth() ) + ( rhs->getFrame()->getHeight()) ) );
  }
};			

Engine::~Engine() {
  std::cout << "Terminating program" << std::endl;
  //std::vector<Drawable*>::iterator it = backgroundSprites.begin();
  //while ( it != backgroundSprites.end() ) {
    //delete *it;
    //++it;
  //}
  
  for(auto it : backgroundSprites){
	  //if(typeid(it).name() != typeid(Player).name()){
			delete it;
		//}
		
		//std::cout << typeid(it).name() <<std::endl;
  }
  
    for(auto it : foregroundSprites){
	  //if(typeid(it).name() != typeid(Player).name()){
			delete it;
		//}
		
		//std::cout << typeid(it).name() <<std::endl;
  }
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IOmod::getInstance() ),
  clock( Clock::getInstance() ),
  ball(new Sprite("ball")),
  player(new Player("player")),
  ai( new SmartSprite("player2", Vector2f( 
											Gamedata::getInstance().getXmlInt("player2/startLoc/x"), 
											Gamedata::getInstance().getXmlInt("player2/startLoc/y")  
										), 
											*ball
					)
	),				    
  hud(new Hud("HUD")),
  strategy( new RectangularCollisionStrategy ),
  collisions(0),
  renderer( rc->getRenderer() ),
  world("back", Gamedata::getInstance().getXmlInt("back/factor") ),
  viewport( Viewport::getInstance() ),
  backgroundSprites(),
  foregroundSprites(),
  currentSprite(-1),
  makeVideo( false ),
  score{0,0},
  gameEnd( false ),
  winScore(3),
  sound(),
  boom(NULL)
{
  //player = new Player("sun");
  //backgroundSprites.push_back( new TwoWayMultiSprite("runner"));
  //backgroundSprites.push_back( new MultiSprite("sun"));
  foregroundSprites.push_back( player );	
  foregroundSprites.push_back( ai );
  
  foregroundSprites.push_back( ball );
  
  //foregroundSprites.push_back( hud );
  
  //foregroundSprites.push_back( new Sprite("sun"));
 
	
  constexpr float u = 1.0f; //Mean size
  constexpr float d = 0.5f; //Std deviation

  std::random_device rd;
  std::mt19937 mt(rd());
  std::normal_distribution<float> dist(u,d);
  
  //unsigned int n = Gamedata::getInstance().getXmlInt("numberOfSnowFlakes");
  
  
  backgroundSprites.push_back(new ShootingSprite("greyRect3"));
          
  backgroundSprites.push_back(new ShootingSprite("orangeRect"));
  
  backgroundSprites.push_back(new ShootingSprite("redRect"));
          
  backgroundSprites.push_back(new ShootingSprite("greyRect2"));

  backgroundSprites.push_back(new ShootingSprite("greyRect1"));


  
  //for(int i=0;i<10;i++){
	  static_cast<ShootingSprite*> (backgroundSprites[0])->shoot();
	  	  static_cast<ShootingSprite*> (backgroundSprites[1])->shoot();
	  	  	  static_cast<ShootingSprite*> (backgroundSprites[2])->shoot();
	  	  	  
	  	  	  
				
  //}
  
  /*
  for(auto i : backgroundSprites){
	  static_cast<ShootingSprite*>(backgroundSprites[0])->getFreeCount();
  }
  */
  
  /*
  for ( unsigned int i = 0; i < n; ++i ) {
    auto* a = new SnowSprite("greyRect1");
    float newscale = dist(mt) * a->getScale();
    while(newscale < 0.1f) newscale = dist(mt);
    a->setScale(newscale);
    backgroundSprites.push_back(a);
    
    auto* b = new SnowSprite("greyRect2");
    newscale = dist(mt) * b->getScale();
    while(newscale < 0.1f) newscale = dist(mt);
    b->setScale(newscale);
    backgroundSprites.push_back(b);
    
    auto* c = new SnowSprite("greyRect3");
    newscale = dist(mt) * c->getScale();
    while(newscale < 0.1f) newscale = dist(mt);
    c->setScale(newscale);
    backgroundSprites.push_back(c);
    
    auto* d = new SnowSprite("starbrush");
    newscale = dist(mt) * d->getScale();
    while(newscale < 0.1f) newscale = dist(mt);
    d->setScale(newscale);
    backgroundSprites.push_back(d);
  }
  */
  
  std::vector<Drawable*>::iterator ptr = backgroundSprites.begin();
  //++ptr;
  sort(ptr, backgroundSprites.end(), SpriteLess());
  
  for ( Drawable* snowsprite : backgroundSprites ) {
    SnowSprite* thisone = dynamic_cast<SnowSprite*>(snowsprite);
    if ( thisone ) {
      std::cout << thisone->getScale() << std::endl;
    }
  }
  
  
  switchSprite();
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  world.draw(); //Black
  
    //Smaller backgroundSprites
  for(unsigned int i=0;i<backgroundSprites.size()/2;i++){
	  backgroundSprites[i]->draw();
  }
  
  
  //world2.draw(); //Trees,Ground
  
    //Bigger backgroundSprites
  for(unsigned int i=backgroundSprites.size()/2;i<backgroundSprites.size();i++){
	  backgroundSprites[i]->draw();
  }
  
  int totalFreeCount = 0;
  int totalBulletCount = 0;
  
  for(unsigned int i=0; i<backgroundSprites.size(); i++){
	  totalFreeCount = totalFreeCount + static_cast<ShootingSprite*>(backgroundSprites[i])->getFreeCount();
	  totalBulletCount = totalBulletCount + static_cast<ShootingSprite*>(backgroundSprites[i])->getBulletCount();
  }
  
   //Foreground (player, enemies, etc)
  for(unsigned int i=0;i<foregroundSprites.size();++i){
	  foregroundSprites[i]->draw();
  }
  
   std::stringstream txt1;
  std::stringstream txt2;
  
  txt1 << "free background objects  : " << totalFreeCount;
  txt2 << "active background objects: " << totalBulletCount;
  
  
  io.writeText(txt1.str(), player->getWorldWidth() - 200, 25);
  io.writeText(txt2.str(), player->getWorldWidth() - 200, 50);
  
  hud->draw();
  
  //std::stringstream strm;
  //std::stringstream strm2;
  
  //strm << "Fps: " << clock.getFps();
  //strm2 << "Avgfps: " << clock.getAvgFps();
  
  //SDL_Color custColor = {255,165,0,255};

  //io.writeText(strm.str(), 30, 60);
  //io.writeText(strm2.str(), 30, 120, custColor);
  
  //std::cout << collisions << std::endl;
  
  std::stringstream strm;
  std::stringstream strm2;
  
  strm << score[0];
  strm2 << score[1];
  
  //std::cout << "PLAYER 1 SCORE: " << score[0] << std::endl <<"PLAYER 2 SCORE: " << score[1] << std::endl;
    io.writeText("SCORE", player->getWorldWidth()/2 - 20, 25 );
  io.writeText(strm.str(), player->getWorldWidth()/2 - 50, 50 );
  io.writeText(strm2.str(), player->getWorldWidth()/2 + 50, 50);
  
  
    strm.str("");
    strm2.str("");
    
	strm << "u r win";
	strm2 << "u r lose";

	if(score[0] == winScore){
		io.writeText(strm.str(), player->getWorldWidth()/2, player->getWorldHeight()/2 );  
	}
	if(score[1] == winScore){
		io.writeText(strm2.str(), player->getWorldWidth()/2, player->getWorldHeight()/2);
	}
  
    if(boom){
		std::stringstream strm;
		//std::stringstream strm2;
  
		strm << boom->chunkCount();
		io.writeText("Explosion Pool: "+strm.str(), 10, 10);
		//std::cout << boom->chunkCount() << std::endl;
	  }
	else{
	  io.writeText("Explosion Pool: 0", 10, 10);
	}

  viewport.draw();
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
if(!gameEnd){
  for(auto* s : backgroundSprites) s->update(ticks);

  for(auto* s : foregroundSprites) s->update(ticks);

  hud->update(ticks);

  world.update();
  //world2.update();
  viewport.update(); // always update viewport last
}
else{
	//sound.stopMusic();
}

}

void Engine::switchSprite(){
  currentSprite++;
  currentSprite = currentSprite % foregroundSprites.size();
  Viewport::getInstance().setObjectToTrack(foregroundSprites[currentSprite]);
}


Drawable* Engine::checkForCollisions() {
	/*
  std::vector<Drawable*>::const_iterator it = foregroundSprites.begin();
  //Drawable* player = sprites[0];
  //++it;
  
  while ( it != foregroundSprites.end() ) {
    if ( strategy->execute(*player, **it) ) {
      std::cout << "collision: " << ++collisions << std::endl;
      ++collisions;
    }
    ++it;
  }
  */
  
  if ( strategy->execute(*player, *ball) ){
	  std::cout << "collision: " << ++collisions << std::endl;
      ++collisions;
      ball->bounce();
      return player;
  }
  
  if ( strategy->execute(*ai, *ball) ){
	  std::cout << "collision: " << ++collisions << std::endl;
      ++collisions;
      ball->bounce();
      return ai;
  }
  
  return NULL;
  
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;
  float speed = 300;
  bool godMode = false;

   Drawable* temp = foregroundSprites[2];
   //ExplodingSprite* boom = NULL; //new ExplodingSprite(*static_cast<Sprite*>(foregroundSprites[2]));

 
  //float score[2] = {0,0};
  


  while ( !done ) {
 
		if(godMode){
			ai->setVelocityY(0);
		}
	  		
	   if( (score[0] == winScore) || (score[1] == winScore) ){
			gameEnd = true;
		}
	    if(ball->getExplode()){
			if(!gameEnd){
				boom = new ExplodingSprite(*static_cast<Sprite*>(foregroundSprites[2]));

				foregroundSprites[2] = boom;
				ball->setExplode(false);
            
            //std::cout << ball->getX() << " " << ball->getWorldWidth() << std::endl;;
            
				if(ball->getX() >= player->getWorldWidth()/2){
					score[0] = score[0] + 1;
				}
				if(ball->getX() <= 0){
					score[1] = score[1] + 1;
				}
			}
            //std::cout << "PLAYER 1 SCORE: " << score[0] << std::endl <<"PLAYER 2 SCORE: " << score[1] << std::endl;
		}
	  
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      //if(boom) std::cout << boom->chunkCount() << std::endl;
	  //const Uint8 *state = SDL_SCANCODE_D;
      
      if (event.type ==  SDL_QUIT) { done = true; break; }
      
      //Edge of world cases
      /*
      if(player->getX() < 0){
		  player->setX(0);
		  player->moveDirection(0,4);
	  }
      
      if(player->getX() > player->getWorldWidth() - player->getFrameWidth() ){
		  player->setX(player->getWorldWidth() - player->getFrameWidth());
		  player->moveDirection(0,6);
	  }
	  
	  
	  if(player->getY() < 0){
		  player->setY(0);
		  player->moveDirection(0,8);
	  }
      
      if(player->getY() > player->getWorldHeight() - player->getFrameHeight() ){
		  player->setY(player->getWorldHeight() - player->getFrameHeight());
		  player->moveDirection(0,2);
	  }
	  */
	  //End edge of world cases
	  
	  //Direction Handling

      
      if( keystate[SDL_SCANCODE_R] ){
		  Vector2f resetPlayerPos(Gamedata::getInstance().getXmlInt("player/startLoc/x"),Gamedata::getInstance().getXmlInt("player/startLoc/y"));
		  player->setPosition(resetPlayerPos);
		  Vector2f resetAIPos(Gamedata::getInstance().getXmlInt("player2/startLoc/x"),Gamedata::getInstance().getXmlInt("player2/startLoc/y"));
		  ai->setPosition(resetAIPos);
		  
		  	int offsetX = rand()%250;
			if(rand()%2){offsetX=offsetX*-1;}
			int offsetY = rand()%250;
			if(rand()%2){offsetY=offsetY*-1;}
			
			ball->setX(Gamedata::getInstance().getXmlInt(temp->getName()+"/startLoc/x") + offsetX);
			ball->setY(Gamedata::getInstance().getXmlInt(temp->getName()+"/startLoc/y") + offsetY);

		  score[0] = 0; score[1] = 0;
		  
		  gameEnd = false;
		  
		  //sound.startMusic();
	  }
	  
      if( keystate[SDL_SCANCODE_A] && keystate[SDL_SCANCODE_D] ){
		  player->moveDirection(0,4); //stop hor
	  }
	  else if(keystate[SDL_SCANCODE_A]){
		  //player->moveDirection(speed,4);
	  }
	  else if(keystate[SDL_SCANCODE_D]){
		  //player->moveDirection(speed,6);
	  }
	  else{
		  player->moveDirection(0,4);
	  }
	  
	  if( keystate[SDL_SCANCODE_W] && keystate[SDL_SCANCODE_S] ){
		  player->moveDirection(0,8); //stop vert
	  }
	  else if(keystate[SDL_SCANCODE_W]){
		  player->moveDirection(speed,8);
	  }
	  else if(keystate[SDL_SCANCODE_S]){
		  player->moveDirection(speed,2);
	  }
	  else{
		      player->moveDirection(0,8);
	  }
      
      if(event.type == SDL_KEYDOWN){
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        
        
		if( keystate[SDL_SCANCODE_G] ){
			if(!godMode){
			godMode=true;
			}
			else{
				godMode=false;
			}
			
		}
        
         if ( keystate[SDL_SCANCODE_E] ) {

			boom = new ExplodingSprite(*static_cast<Sprite*>(foregroundSprites[2]));
            foregroundSprites[2] = boom;
        }	
        		
        if (keystate[SDL_SCANCODE_F1]){
			if(hud->getHUDVisibility()){
				hud->setHUDVisibility(false);
			}
			else{
				hud->setHUDVisibility(true);
			}
		}
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
      if(event.type == SDL_KEYUP){

	  }

    }
    
    if(boom){
		/*
		std::stringstream strm;
		//std::stringstream strm2;
  
		strm << boom->chunkCount();
		io.writeText(strm.str(), 10, 10);
		std::cout << boom->chunkCount() << std::endl;
		*/
		if(boom->chunkCount() == 0){
			int offsetX = rand()%250;
			if(rand()%2){offsetX=offsetX*-1;}
			int offsetY = rand()%250;
			if(rand()%2){offsetY=offsetY*-1;}
			
			temp->setX(Gamedata::getInstance().getXmlInt(temp->getName()+"/startLoc/x") + offsetX);
			temp->setY(Gamedata::getInstance().getXmlInt(temp->getName()+"/startLoc/y") + offsetY);
		  foregroundSprites[2] = temp;
		  delete boom;
		  boom = NULL;
		  
		}
		
	  }
    
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      draw();
      update(ticks);
      checkForCollisions();
      if(makeVideo){
        frameGen.makeFrame();
      }

    }
  }
}
