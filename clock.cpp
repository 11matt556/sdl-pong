#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"
#include "ioMod.h"

Clock& Clock::getInstance() {
  static Clock clock;
  return clock;
}

Clock::Clock() :
  started(false),
  paused(false),
  FRAME_CAP_ON(Gamedata::getInstance().getXmlBool("frameCapOn")),
  PERIOD(Gamedata::getInstance().getXmlInt("period")),
  FRAMEMAX(Gamedata::getInstance().getXmlInt("frameMax")),
  frames(0),
  timeAtStart(0),
  timeAtPause(0),
  currTicks(0), prevTicks(0), ticks(0),
  frameArray()


  {
  startClock();
}

Clock::Clock(const Clock& c) :
  started(c.started),
  paused(c.paused),
  FRAME_CAP_ON(c.FRAME_CAP_ON),
  PERIOD(c.PERIOD),
  FRAMEMAX(c.FRAMEMAX),
  frames(c.getFrames()),
  timeAtStart(c.timeAtStart), timeAtPause(c.timeAtPause),
  currTicks(c.currTicks), prevTicks(c.prevTicks), ticks(c.ticks),
  frameArray(c.frameArray)
  {
  startClock();
}

void Clock::toggleSloMo() {
  throw( std::string("Slow motion is not implemented yet") );
}

unsigned int Clock::getTicks() const {
  if (paused) return timeAtPause;
  else return SDL_GetTicks() - timeAtStart;
}

unsigned int Clock::getElapsedTicks() {
  if (paused) return 0;

  currTicks = getTicks();
  ticks = currTicks-prevTicks;

  if ( FRAME_CAP_ON ) {
    if ( ticks < PERIOD ) return 0;
    prevTicks = currTicks;
    return ticks;
  }
  else {
    prevTicks = currTicks;
    return ticks;
  }

}

int Clock::getFps() const {
  if ( getSeconds() > 0 ) return frames/getSeconds();
  else if ( getTicks() > 5000 and getFrames() == 0 ) {
    throw std::string("Can't getFps if you don't increment the frames");
  }
  else return 0;
}

int Clock::getAvgFps() {




  
  static int i=0;
  
  static int frameAvg = 0;
  
  frameArray.reserve(FRAMEMAX);

  if ( getTicks() > 5000 and getFrames() == 0 ) {
    throw std::string("Can't getFps if you don't increment the frames");
  }
	else{

	int fps = getFps();
	
	if((int)frameArray.size() < FRAMEMAX){
		frameAvg = (frameAvg + fps);
		//std::cout<<frameArray.size()<<std::endl;
		frameArray.push_back(fps);
	}
	else{
		frameAvg = (frameAvg + fps - frameArray[i%FRAMEMAX]);
		frameArray[i%FRAMEMAX] = fps;
	}
	i++;
}

  return frameAvg/(frameArray.size()+1);
}

  /*
  else{

	if((frames-lastFrame) < FRAMEMAX){
		sumFrames = sumFrames + getFps();
		numberOfAverages++;
	}
		avgFrames = sumFrames/numberOfAverages;
	if((frames-lastFrame) >= FRAMEMAX){
		lastFrame = frames;
		sumFrames = 0;
		numberOfAverages=1;
	}
	*/

void Clock::incrFrame() {
  if ( !paused ) {
    ++frames;
  }
}

void Clock::startClock() {
  started = true;
  paused = false;
  frames = 0;
  timeAtPause = timeAtStart = SDL_GetTicks();
  prevTicks = 0;
}
void Clock::pause() {
  if( started && !paused ) {
    timeAtPause = SDL_GetTicks() - timeAtStart;
    paused = true;
  }
}
void Clock::unpause() {
  if( started && paused ) {
    timeAtStart = SDL_GetTicks() - timeAtPause;
    paused = false;
  }
}

