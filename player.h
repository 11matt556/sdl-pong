#include "twoWayMultiSprite.h"
class Player : public TwoWayMultiSprite{
	public:
		Player(const std::string&);
		Player(const Player&);
	
		//static Player& getInstance(const std::string&);
		void moveDirection(float speed, int direction);
		
		virtual void draw() const;
		virtual void update(Uint32 ticks);
		
		int getFrameWidth() {return frameWidth;}
		int getFrameHeight() {return frameHeight;}
		int getWorldWidth() {return worldWidth;}
		int getWorldHeight() {return worldHeight;}
	
	private:
		bool needToFlip;
		float moveSpeed;
};
