#ifndef FRAME__H
#define FRAME__H

#include <iostream>
#include <string>
#include <SDL.h>

class Frame {
public:
  Frame( SDL_Texture* );
  Frame(const Frame&);
  Frame& operator=(const Frame&);
  
  ~Frame()=default;

  void draw(int x, int y) const;
  void draw(int x, int y, float scale) const;
  void draw(int sx, int sy, int dx, int dy) const;
  void draw(int x, int y, SDL_RendererFlip f) const;
  void draw(int x, int y, SDL_RendererFlip f, float scale) const;

  SDL_Texture* getTexture() const { return texture; }
  //inline int getWidth()  const { return width; }
  //inline int getHeight() const { return height; }
  inline int getWidth()  const { return rect.w; }
  inline int getHeight() const { return rect.h; }
  
    Frame* crop(SDL_Rect)const;
  
private:
  SDL_Renderer * renderer;
  SDL_Texture * texture;
  //int width;
  //int height;
  
  SDL_Rect rect;
  
  Frame();
};

#endif
