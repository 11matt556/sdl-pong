// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"
#include "smartSprite.h"

FrameFactory*  FrameFactory::instance = NULL;
RenderContext* RenderContext::instance = NULL;
float SmartSprite::interactionDistance  = Gamedata::getInstance().getXmlInt("interactionDistance"); 

int main(int, char*[]) {
   try {
      Engine engine;
      engine.play();
      delete RenderContext::getInstance();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
