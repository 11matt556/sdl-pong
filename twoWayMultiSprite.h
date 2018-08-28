#include "multisprite.h"
class TwoWayMultiSprite : public MultiSprite{
 public:
    TwoWayMultiSprite(const std::string&);
    TwoWayMultiSprite(const TwoWayMultiSprite&);

   virtual void draw() const;
   virtual void update(Uint32 ticks);
   
   protected:
   SDL_RendererFlip flip;
};
