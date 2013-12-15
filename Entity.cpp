#include "Entity.h"

Entity::Entity()
{
}

Entity::Entity(float _xPos, float _yPos, float _mass, HTEXTURE texture)
{
	this->position.x = _xPos;
	this->position.y = _yPos;
	this->mass = _mass;

	thrustVector.x = 0;
	thrustVector.y = 10;

	angularVelocity = 0;
	angle = M_PI;

	setGFX(texture);
}

Entity::~Entity(void)
{
}

void Entity::setGFX(HTEXTURE texture)
{
	sprite = new hgeSprite(texture,144,6,16,40);
	sprite->SetFlip(false, true);
	sprite->SetHotSpot(8,20);

	boundingBox = new hgeRect();
	sprite->GetBoundingBox(position.x, position.y, boundingBox);

	bbox0 = new hgeVector(boundingBox->x1 - position.x, boundingBox->y1 - position.y);
	bbox1 = new hgeVector(boundingBox->x2 - position.x, boundingBox->y1 - position.y);
	bbox2 = new hgeVector(boundingBox->x2 - position.x, boundingBox->y2 - position.y);
	bbox3 = new hgeVector(boundingBox->x1 - position.x, boundingBox->y2 - position.y);

	thrustSprite = new hgeSprite(texture, 0,0,30,30);
	thrustSprite->SetHotSpot(15,15);
	
	particle = new hgeParticleSystem("particle1.psi", thrustSprite);
	particle->info.sprite->SetBlendMode(BLEND_ALPHAADD);
	particle->info.fDirection = M_PI_2;
}

void Entity::drawSelf(float time, HGE* hge)
{
	sprite->RenderEx(position.x, position.y, angle,1,1);
	/*
	hge->Gfx_RenderLine(position.x, position.y, position.x + bbox0->x, position.y + bbox0->y);
	hge->Gfx_RenderLine(position.x, position.y, position.x + bbox1->x, position.y + bbox1->y);
	hge->Gfx_RenderLine(position.x, position.y, position.x + bbox2->x, position.y + bbox2->y);
	hge->Gfx_RenderLine(position.x, position.y, position.x + bbox3->x, position.y + bbox3->y);
	*/
	if(thrustOn)
	{
		hgeVector* thrustParticleVector = new hgeVector(0, -30);
		(*thrustParticleVector).Rotate(angle);
		particle->info.fDirection = angle - M_PI_2;
		particle->FireAt(position.x + (*thrustParticleVector).x, position.y + (*thrustParticleVector).y);
		thrustOn = false;
	}
	particle->Update(time);
	particle->Render();
}

void Entity::updatePosition(float time)
{
	
	previousPosition = position;
	position += velocity * time;
	angle += angularVelocity * time;

	bbox0->Rotate(angularVelocity * time);
	bbox1->Rotate(angularVelocity * time);
	bbox2->Rotate(angularVelocity * time);
	bbox3->Rotate(angularVelocity * time);

	sprite->GetBoundingBoxEx(position.x, position.y,angle,1,1, boundingBox);
	
}

void Entity::applyAngularForce(float force, float time)
{
	angularVelocity += (force/mass) * time;
}

void Entity::applyLinearForce(hgeVector vector, float time)
{
	velocity += (vector/mass)*time;
}

void Entity::applyAcceleration(hgeVector vector, float time)
{
	velocity += vector * time;
}

void Entity::applyLinearForce(float xForce, float yForce, float time)
{
	velocity.x += (xForce/mass) * time;
	velocity.y += (yForce/mass) * time;
}

void Entity::thrust(float time)
{
	velocity += ((*thrustVector.Rotate(angle))/mass)*time;
	thrustVector.Rotate(-angle);
	thrustOn = true;
}

