#include "Enemy.h"


Enemy::Enemy(float _xPos, float _yPos, float _mass, HTEXTURE texture)
{
	this->position.x = _xPos;
	this->position.y = _yPos;
	this->mass = _mass;

	thrustVector.x = 0;
	thrustVector.y = 10;
	angularVelocity = 0;
	angle = M_PI;

	sprite = new hgeSprite(texture,107,11,16,24);
	sprite->SetFlip(false, true);
	sprite->SetHotSpot(8,12);

	boundingBox = new hgeRect();
	sprite->GetBoundingBox(_xPos, _yPos, boundingBox);

	bbox0 = new hgeVector(boundingBox->x1 - _xPos, boundingBox->y1 - _yPos);
	bbox1 = new hgeVector(boundingBox->x2 - _xPos, boundingBox->y1 - _yPos);
	bbox2 = new hgeVector(boundingBox->x2 - _xPos, boundingBox->y2 - _yPos);
	bbox3 = new hgeVector(boundingBox->x1 - _xPos, boundingBox->y2 - _yPos);

	thrustSprite = new hgeSprite(texture, 0,0,30,30);
	thrustSprite->SetHotSpot(15,15);
	
	particle = new hgeParticleSystem("particle1.psi", thrustSprite);
	particle->info.sprite->SetBlendMode(BLEND_ALPHAADD);
	particle->info.fDirection = M_PI_2;
}


Enemy::~Enemy(void)
{
}

void Enemy::runAI(float deltaT, Terrain* terrain)
{
	//adjust y position
	if(velocity.y > targetY - position.y)
		thrust(deltaT);
	
	//if we're too low or falling too fast, thrust straight down to correct
	if(terrain->findAltitudeFrom(position.x, position.y) < 100 || velocity.y > 100)
	{
		turnToAngle(M_PI, deltaT);
		thrust(deltaT);
		return;
	}
	//adjust x position
	//check if we're moving too slowly or too quickly towards target
	float xDistance = targetX - position.x - (2 * velocity.x);		//distance to target (fudged a little to include velocity towards/away from target)
	float angleFactor =	abs(xDistance)/800 < 0.4 ? abs(xDistance)/800 : 0.4;		//how far to lean towards target based on distance
	if(velocity.x > xDistance / 2)
		turnToAngle(M_PI * (1.0 - angleFactor), deltaT);
	else
		turnToAngle(M_PI * (1.0 + angleFactor), deltaT);
}

void Enemy::turnToAngle(float targetAngle, float deltaT)
{
	//static int maxTurnRate = 1;
	float maxTurnRate = abs(targetAngle - angle);
	//static int turnAngularForce;
	if(angle > targetAngle && angularVelocity > -maxTurnRate)
	{
		applyAngularForce(-1, deltaT);
	}
	else if(angle < targetAngle && angularVelocity < maxTurnRate)
	{
		applyAngularForce(1, deltaT);
	}
}
