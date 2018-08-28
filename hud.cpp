#include "hud.h"
#include "gamedata.h"
#include "renderContext.h"
#include "viewport.h"
#include "ioMod.h"
#include "clock.h"
#include <sstream>

Hud::Hud(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(
					Gamedata::getInstance().getXmlInt(name+"/slideInSpeedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/slideInSpeedY")) 
           ),
  hudName(name),
  showHUD( true ),        
  width(Gamedata::getInstance().getXmlInt(name+"/width")),
  height(Gamedata::getInstance().getXmlInt(name+"/height")),
  scale(1),
  r(Gamedata::getInstance().getXmlInt(name+"/red")),
  g(Gamedata::getInstance().getXmlInt(name+"/green")),
  b(Gamedata::getInstance().getXmlInt(name+"/blue")),
  a(Gamedata::getInstance().getXmlInt(name+"/alpha")),
  
  endLoc(Gamedata::getInstance().getXmlInt(name+"/endLoc/x"), 
         Gamedata::getInstance().getXmlInt(name+"/endLoc/y")),
  renderer(RenderContext::getInstance()->getRenderer())    
         
{ }

void Hud::update(Uint32 ticks){
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	setPosition(getPosition() + incr);
	//std::cout << getPosition() << std::endl;
}

void Hud::draw() const{
	if(showHUD){
		int x = getX(); 
		int y = getY();
		int bufferSpace=25;
	    //x -= Viewport::getInstance().getX();
		//y -= Viewport::getInstance().getY();
		int tempHeight = scale*height;
		int tempWidth = scale*width;
		SDL_Rect dest = {x, y, tempWidth, tempHeight};
  
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor( renderer, r, g, b, a );
		SDL_RenderFillRect (renderer, &dest);
  
		SDL_SetRenderDrawColor( renderer, r, g, b, a );
		SDL_RenderDrawRect( renderer, &dest );
		
		IOmod::getInstance().writeText(Gamedata::getInstance().getXmlStr("userinfo/name"), Gamedata::getInstance().getXmlInt("userinfo/posX"), Gamedata::getInstance().getXmlInt("userinfo/posY"));
		
		IOmod::getInstance().writeText(Gamedata::getInstance().getXmlStr("HUD/HUDTitle"),  getPosition()[0]+bufferSpace/2 , getPosition()[1]);
		IOmod::getInstance().writeText("Tracking "+Viewport::getInstance().getObjectToTrack()->getName(), getPosition()[0]+bufferSpace/2 , getPosition()[1]+bufferSpace);
		
		std::stringstream fps;
		std::stringstream avgfps;
  
		fps << "Fps: " << Clock::getInstance().getFps();
		avgfps << "Avgfps: " << Clock::getInstance().getAvgFps();
  
		//SDL_Color custColor = {255,165,0,255};

		IOmod::getInstance().writeText(fps.str(), getPosition()[0]+bufferSpace/2 , getPosition()[1]+bufferSpace*2);
		IOmod::getInstance().writeText(avgfps.str(), getPosition()[0]+bufferSpace/2 , getPosition()[1]+bufferSpace*3);
		
		IOmod::getInstance().writeText("Instructions:", getPosition()[0]+bufferSpace/2 , getPosition()[1]+bufferSpace*4);
		IOmod::getInstance().writeText("  FIRST TO SCORE 3 WINS", getPosition()[0]+bufferSpace/2 , getPosition()[1]+bufferSpace*5);
		IOmod::getInstance().writeText("  PRESS R TO RESET", getPosition()[0]+bufferSpace/2 , getPosition()[1]+bufferSpace*6);
		IOmod::getInstance().writeText("  PRESS G FOR GOD MODE (disable AI)", getPosition()[0]+bufferSpace/2 , getPosition()[1]+bufferSpace*7);
		IOmod::getInstance().writeText("  MOVE UP:   W", getPosition()[0]+bufferSpace/2 , getPosition()[1]+bufferSpace*8);
		IOmod::getInstance().writeText("  MOVE DOWN: S", getPosition()[0]+bufferSpace/2 , getPosition()[1]+bufferSpace*9);
		IOmod::getInstance().writeText("  TOGGLE HELP: F1", getPosition()[0]+bufferSpace/2 , getPosition()[1]+bufferSpace*10);
		IOmod::getInstance().writeText("  GENERATE FRAMES: F4", getPosition()[0]+bufferSpace/2 , getPosition()[1]+bufferSpace*11);

  
  } 
}
