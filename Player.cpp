#include "Player.h"


Player::Player(int _xPos, int _yPos, HTEXTURE texture)
{

}

Player::~Player(void)
{
}

bool Player::loadSprite(HTEXTURE texture)
{
	sprite = new hgeSprite(texture,144,6,16,40);
	sprite->SetFlip(false, true);
	sprite->SetHotSpot(8,20);
	return true;
}
