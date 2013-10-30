#pragma once
#include "Entity.h"
#include "Terrain.h"

class Enemy : public Entity
{
public:
	Enemy(float _xPos, float _yPos, float _mass, HTEXTURE texture);
	~Enemy(void);

	void runAI(float deltaT, Terrain* terrain);
	void setTarget(int targetX, int targetY){this->targetX = targetX; this->targetY = targetY;}
	void turnToAngle(float targetAngle, float deltaT);

private:
	int targetX, targetY;
};

