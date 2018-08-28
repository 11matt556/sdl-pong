#include "drawable.h"
#include "frame.h"
#include "ioMod.h"
#include "viewport.h"
#include "renderContext.h"

Frame::Frame( SDL_Texture* tex ) : 
  renderer(RenderContext::getInstance()->getRenderer()),
  texture( tex ),
  //width(0), 
  //height(0)
  rect{0,0,0,0}
{ SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h); }

Frame::Frame( const Frame& frame ) :
  renderer(frame.renderer),
  texture(frame.texture), 
  //width(rect.width), 
  //height(frame.height)
  rect(frame.rect)
{ }


Frame& Frame::operator=(const Frame& rhs) {
  renderer = rhs.renderer;
  texture = rhs.texture;
  //width = rhs.width;
  //height = rhs.height;
  rect = rhs.rect;
  return *this;
}

void Frame::draw(int x, int y) const {
  draw(x,y,1.0f);
}

void Frame::draw(int x, int y, SDL_RendererFlip flip) const{
  draw(x,y,flip,1.0f);
}

void Frame::draw(int x, int y, SDL_RendererFlip flip, float scale) const{
  x -= Viewport::getInstance().getX();
  y -= Viewport::getInstance().getY();
  int tempHeight = scale*rect.h;
  int tempWidth = scale*rect.w;
  SDL_Rect dest = {x, y, tempWidth, tempHeight };
  SDL_RenderCopyEx(renderer, texture, NULL, &dest,0,NULL,flip);
}

void Frame::draw(int x, int y, float scale) const {
  x -= Viewport::getInstance().getX();
  y -= Viewport::getInstance().getY();
  int tempHeight = scale*rect.h;
  int tempWidth = scale*rect.w;
  SDL_Rect dest = {x, y, tempWidth, tempHeight};
  SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void Frame::draw(int sx, int sy, int dx, int dy) const {
  //SDL_Rect src = { sx, sy, width, height };    
  //SDL_Rect dst = { dx, dy, width, height };
  SDL_Rect src = { sx, sy, rect.w, rect.h };    
  SDL_Rect dst = { dx, dy, rect.w, rect.h };  
  
  SDL_RenderCopy(renderer, texture, &src, &dst);
}

Frame* Frame::crop(SDL_Rect sub)const{
  if(sub.x+sub.w > rect.w || sub.y+sub.h > rect.h){
    std::cerr << "Attempted to crop image with invalid geometry."
              << std::endl
              << "(0,0 + "<<rect.w << "x"<<rect.h<<")"
              << " --> "
              << "("<<sub.x<<","<<sub.y<<" + "<<sub.w << "x"<<sub.h<<")"
              << std::endl;
    return nullptr;
  }

  Frame* cloned = new Frame(*this);
  cloned->rect.w = sub.w;
  cloned->rect.h = sub.h;

  return cloned;
}

//HUD
/*
void Frame::draw(int x, int y, float scale, int r, int g, int b, int a) const{
  x -= Viewport::getInstance().getX();
  y -= Viewport::getInstance().getY();
  int tempHeight = scale*height;
  int tempWidth = scale*width;
  SDL_Rect dest = {x, y, tempWidth, tempHeight};
  
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, r, g, b, a );
  SDL_RenderFillRect (renderer, &dest);
  
  SDL_SetRenderDrawColor( renderer, r, g, b, a );
  SDL_RenderDrawRect( renderer, &dest );
  
}
*/

