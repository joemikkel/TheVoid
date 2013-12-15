#pragma once
#include "Entity.h"
class Player : public Entity
{
public:
	Player(int xPos, int yPos, HGE* hge);
	~Player(void);

private:
	bool loadSprite(HTEXTURE texture);
};

