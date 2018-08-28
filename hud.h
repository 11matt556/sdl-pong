#include <iostream>
#include <SDL2/SDL.h>
#include "drawable.h"


class Hud : public Drawable{
	public:
	
	Hud(const std::string& n);
		
		//virtual const Frame* getFrame() const { return frame; }
	virtual void draw() const;
	virtual void update(Uint32 ticks);
		
	inline int getWidth()  const { return width; }
	inline int getHeight() const { return height; }
	virtual const Frame* getFrame() const { return NULL; }
	
	bool getHUDVisibility() const {return showHUD;}
	void setHUDVisibility(bool b) {showHUD = b;} 
	
	private:
	Hud() = delete;
	Hud(const Hud&) = delete;
    Hud& operator=(const Hud&) = delete;
		const std::string& hudName;
		bool showHUD;
		//const Frame * frame; //Why does it complain aboutthis here buyt not in sprite?
		//int worldWidth;
		//int worldHeight;
		//int frameWidth;
		//int frameHeight;
		int width;
		int height;
		float scale;
		//SDL_Color color;
		int r,g,b,a;
		Vector2f endLoc;
		SDL_Renderer* renderer;
		
};
