#include "Level.h"


Level::Level(HGE* _hge, int xRes, int yRes)
{
	hge = _hge;
	this->xRes = xRes;
	this->yRes = yRes;
	gravity = new hgeVector(0,5);
	terrain = new Terrain(xRes,yRes,_hge);
	font = new hgeFont("font.fnt", true);

	particleManager = new hgeParticleManager();

	HTEXTURE texture = hge->Texture_Load("ships.png");
	hgeSprite* sprite = new hgeSprite(texture, 0,0,30,30);
	sprite->SetHotSpot(15, 15);
	collisionLarge = new hgeParticleSystem("particle9.psi", sprite);
}

Level::~Level(void)
{
}

/*Collides entity with terrain and rebounds it*/
bool Level::collideWithTerrain(Entity* entity)
{
	float normal = terrain->findSurfaceCollisionNormal(	entity->getXPos() + entity->bbox0->x, 
														entity->getYPos() + entity->bbox0->y, 
														entity->getXPos() + entity->bbox1->x, 
														entity->getYPos() + entity->bbox1->y, 
														hge);
	if(normal == -1)
		normal = terrain->findSurfaceCollisionNormal(	entity->getXPos() + entity->bbox1->x, 
														entity->getYPos() + entity->bbox1->y, 
														entity->getXPos() + entity->bbox2->x, 
														entity->getYPos() + entity->bbox2->y, 
														hge);
	if(normal == -1)
		normal = terrain->findSurfaceCollisionNormal(	entity->getXPos() + entity->bbox2->x, 
														entity->getYPos() + entity->bbox2->y, 
														entity->getXPos() + entity->bbox3->x, 
														entity->getYPos() + entity->bbox3->y, 
														hge);
	if(normal == -1)
		normal = terrain->findSurfaceCollisionNormal(	entity->getXPos() + entity->bbox3->x, 
														entity->getYPos() + entity->bbox3->y, 
														entity->getXPos() + entity->bbox0->x, 
														entity->getYPos() + entity->bbox0->y, 
														hge);
	if(normal != -1)
	{
		//hge->System_Log("Collision Normal : %f", normal);
		//decode angle from bits
		float collisionAngle = normal/255 * M_PI * 2;
		//calculate the rebound velocity and apply it to the entity
		hgeVector* normalVector = new hgeVector(-cos(collisionAngle), sin(collisionAngle));
		hgeVector entityNormalizedVector = (player->getVelocity()) / player->getSpeed();
		float dotProduct = normalVector->Dot(&entityNormalizedVector);
		hgeVector reboundVector = player->getSpeed() * (-2 * dotProduct * (*normalVector) + entityNormalizedVector);
		entity->setVelocity(reboundVector * 0.65);

		terrain->makeHole(terrain->lastCollisionX, terrain->lastCollisionY, 4, hge);
		particleManager->SpawnPS(&(collisionLarge->info), terrain->lastCollisionX, terrain->lastCollisionY);
		return true;
	}
	return false;
}
bool Level::logicStep()
{
	deltaT = hge->Timer_GetDelta();
	processInput();
	player->updatePosition(deltaT);
	player->applyLinearForce(*gravity, deltaT);
	collideWithTerrain(player);

	enemy->updatePosition(deltaT);
	enemy->applyLinearForce(*gravity, deltaT);
	enemy->runAI(deltaT, terrain);
	enemy->setTarget(player->getXPos(), player->getYPos());
	collideWithTerrain(enemy);

	return true;
}

bool Level::createPlayer(int xPos, int yPos)
{
	HTEXTURE playerTexture = hge->Texture_Load("ships.png");
	player = new Entity(xPos, yPos, 0.1f, playerTexture);

	enemy = new Enemy( xPos + 100, yPos, 0.1f, playerTexture);
	return true;
}

bool Level::render()
{
	player->drawSelf(deltaT, hge);
	enemy->drawSelf(deltaT, hge);
	terrain->render(hge);
	particleManager->Update(deltaT);
	particleManager->Render();

	char fps[10];
	itoa((1/deltaT), fps, 10);
	font->Render(0,0,HGETEXT_LEFT,fps);

	float playerX = player->getXPos();
	float playerY = player->getYPos();
	itoa(player->getSpeed(), fps, 10);
	font->Render(0,50,HGETEXT_LEFT, fps);

	return true;
}
void Level::processInput()
{
	if (hge->Input_GetKeyState(HGEK_LEFT))
	{
		player->applyAngularForce(-1.4f, deltaT);
	}
	if (hge->Input_GetKeyState(HGEK_RIGHT))
	{
		player->applyAngularForce(1.4f, deltaT);
	}
	if (hge->Input_GetKeyState(HGEK_UP))
	{
		player->thrust(deltaT);
	}
}
