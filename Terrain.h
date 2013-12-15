#pragma once
#include <hge.h>
#include <hgesprite.h>
#include <hgevector.h>
#include <xtgmath.h>
//#include "Entity.h"
class Terrain
{
public:
	Terrain(int xWidth, int yWidth, HGE* hge);
	~Terrain(void);
	/* Creates a new unique terrain in the given texture pointer */
	void generate(HGE* hge, DWORD* lockedArray);
	/* Draws the terrain to screen */
	void render(HGE* hge);
	/* Checks if a point collides with the terrain */
	bool pointCollides(int x, int y, HGE* hge);
	/* Makes a hole in the terrain of a given size */
	void makeHole(int x, int y, int diameter, HGE* hge);
	/* Finds the normal (i.e. angle) of the surface where it intersects the line
	   from x0,y0 to x1,y1
	   */
	float findSurfaceCollisionNormal(int x0, int y0, int x1, int y1, HGE* hge);
	/* Returns a texture representing the terrain collision map */
	HTEXTURE* getCollisionMap(){return &collisionMap;}
	/* Setter, getter for a terrain pixel color */
	void setColor(int x, int y, DWORD* lockedArray, DWORD color);
	DWORD getColor(int x, int y, DWORD* lockedArray);
	/* Keeps track of where the last collision occurred */
	int lastCollisionX, lastCollisionY;
	
	/* Finds how far down the ground is from a given position*/
	int findAltitudeFrom(int x, int y);
	/* Finds how far up the edge of the screen is from a given position */
	int findCeilingFrom(int x, int y);

	/* Terrain map - stores ground, air pixels and terrain surface normals */
	HTEXTURE collisionMap;	

private:
	int width, height;

	float calculatePixelNormal(int x, int y, DWORD* lockedArray);
	bool isEdgePixel(int x, int y, DWORD* lockedArray);
	
	DWORD groundColor;
	DWORD emptyColor;
	DWORD highlightColor;

	HGE* hge;

	hgeSprite* terrainSprite;
};

