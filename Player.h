#pragma once
#include "Entity.h"
class Player : public Entity
{
public:
	Player(int xPos, int yPos, HTEXTURE texture);
	~Player(void);

private:
	bool loadSprite(HTEXTURE texture);
};

