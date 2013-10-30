#include "Entity.h"
#include "Enemy.h"
#include "Terrain.h"
#include <hgefont.h>
#include <string>
#include <iostream>
#pragma once
class Level
{
public:
	Level(HGE* _hge, int xRes, int yRes);
	~Level(void);
	bool createPlayer(int xPos, int yPos);
	bool logicStep();
	bool render();

private:
	hgeFont* font;

	int xRes, yRes;

	HGE* hge;
	Entity* player;
	Enemy* enemy;
	hgeVector* gravity;
	Terrain* terrain;
	float deltaT;

	void processInput();
	bool collideWithTerrain(Entity* ent);

	hgeParticleManager* particleManager;
	hgeParticleSystem* collisionLarge;
};

