#pragma once
#include <hge.h>
#include <hgesprite.h>
#include <hgevector.h>
#include <xtgmath.h>
#include "Entity.h"
class Terrain
{
public:
	Terrain(int xWidth, int yWidth, HGE* hge);
	~Terrain(void);
	void generate(HGE* hge, DWORD* lockedArray);
	void render(HGE* hge);
	bool pointCollides(int x, int y, HGE* hge);
	void makeHole(int x, int y, int diameter, HGE* hge);
	float findSurfaceCollisionNormal(int x0, int y0, int x1, int y1, HGE* hge);
	HTEXTURE* getCollisionMap(){return &collisionMap;}
	void setColor(int x, int y, DWORD* lockedArray, DWORD color);
	DWORD getColor(int x, int y, DWORD* lockedArray);
	int lastCollisionX, lastCollisionY;
	
	int findAltitudeFrom(int x, int y);
	int findCeilingFrom(int x, int y);

private:
	int width, height;

	float calculatePixelNormal(int x, int y, DWORD* lockedArray);
	bool isEdgePixel(int x, int y, DWORD* lockedArray);
	
	DWORD groundColor;
	DWORD emptyColor;
	DWORD highlightColor;

	HGE* hge;

	hgeSprite* terrainSprite;
	HTEXTURE collisionMap;	
};

