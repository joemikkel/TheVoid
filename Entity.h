#pragma once
#include <iostream>
#include <hge.h>
#include <hgevector.h>
#include <hgesprite.h>
#include <hgeparticle.h>
  

class Entity
{
public:
	Entity(float _xPos, float _yPos, float mass, HTEXTURE texture);
	Entity();
	~Entity(void);

	void setGFX(HTEXTURE texture);

	void applyLinearForce(hgeVector vector, float time);
	void applyLinearForce(float xForce, float yForce, float time);

	void applyAcceleration(hgeVector vector, float time);
	void applyAngularForce(float force, float time);

	void thrust(float time);

	void updatePosition(float time);
	void drawSelf(float time, HGE* hge);
	
	hgeVector getVelocity(){return velocity;}
	float getSpeed(){return velocity.Length();}

	void setVelocity(hgeVector vel){velocity = vel;}

	float getXPos(){return this->position.x;}
	float getYPos(){return this->position.y;}
	float getPrevXPos(){return this->previousPosition.x;}
	float getPrevYPos(){return this->previousPosition.y;}

	hgeRect* getBoundingBox(){return boundingBox;}

	hgeSprite* getSprite(){return sprite;}

	hgeVector* bbox0, *bbox1, *bbox2, *bbox3;

	bool isColliding;

protected:
	float mass;

	bool thrustOn;
	HGE* hge;
	hgeVector position;
	hgeVector previousPosition;

	hgeVector velocity;
	hgeVector thrustVector;

	hgeSprite* sprite;
	hgeSprite* thrustSprite;

	hgeRect* boundingBox;

	hgeParticleSystem* particle;

	float angle;
	float angularVelocity;

	bool loadSprite(HTEXTURE texture);
};

