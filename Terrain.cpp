#include "Terrain.h"

Terrain::Terrain(int xSize, int ySize, HGE* hge)
{
	width = xSize;
	height = ySize;
	collisionMap = hge->Texture_Create(xSize, ySize);
	groundColor = 0xFFFFFFFF;
	emptyColor = 0x00000000;
	highlightColor = 0xFF00FFFF;
	this->hge = hge;
	DWORD* collisionMapArray = hge->Texture_Lock(collisionMap);
	generate(hge, collisionMapArray);
	
	//GENERATE TERRAIN BOUNDING LINE HERE
	for(int i = 1; i < xSize - 1; i++)
	{
		for(int j = 1; j < ySize - 1; j++)
		{
			if(getColor(i, j, collisionMapArray) == groundColor)
			{
				if(isEdgePixel(i,j,collisionMapArray))
				{
					float angle = calculatePixelNormal(i,j,collisionMapArray);
					angle += M_PI;
					angle = (angle/(2*M_PI)) * 255;
					//hge->System_Log("TERRAIN NORMAL : %lu", highlightColor - (unsigned int)angle); 
					setColor(i,j,collisionMapArray,highlightColor - (unsigned int)angle);
				}
			}
		}
	}

	hge->Texture_Unlock(collisionMap);
	terrainSprite = new hgeSprite(collisionMap,0.0,0.0,xSize,ySize);


}

void Terrain::generate(HGE* hge, DWORD* lockedArray)
{
	//configuration variables; boundaries are 'soft'			
	float maxDeltaH = 2;				//how much the terrain is allowed to move in y across one x (i.e. max slope)
	int leftOrigin = height * 0.75;	//how far down the left end of the terrain 'starts'
	int maxHeight = height * 0.9;	//how far down the screen the terrain is allowed to go
	int minHeight = height * 0.5;	//how high up the screen the terrain is allowed to go <^-coords are reversed
	int keepDeltaOdds = 80;			//% chance that deltaH won't change

	float deltaH = hge->Random_Float(-1, 1);
	float deltaHAggregate = 0;
	int currentH = leftOrigin;	

	for(int x = 0; x < width; x++)
	{

		int randomNum = hge->Random_Int(0,100);
		if(randomNum > keepDeltaOdds)
		{
			deltaH += hge->Random_Float(-1, 1);
			//keepDeltaOdds = hge->Random_Int(80,99);
		}

		if(abs(deltaH)<1)
		{
			deltaHAggregate += deltaH;
			if(abs(deltaHAggregate) > 1)
			{
				deltaHAggregate = 0;
				currentH++;
			}
			else
			{
				deltaHAggregate += deltaH;
			}
		}
		else
			currentH += deltaH;


		//check steepness bound and correct
		if(deltaH > maxDeltaH)
			deltaH = maxDeltaH;
		else if(deltaH < -maxDeltaH)
			deltaH = -maxDeltaH;
		
		if(currentH >= maxHeight)
			deltaH -= 1;
		else if(currentH <= minHeight)
			deltaH += 1;

		for(int j = currentH; j < height; j++)
		{
			setColor(x, j, lockedArray, groundColor);
		}
	}
}
float Terrain::calculatePixelNormal(int x, int y, DWORD* lockedArray)
{
	int xSum = 0;
	int ySum = 0;
	hgeVector* normal;

	if(	getColor(x - 1,	y - 1,	lockedArray) == emptyColor)
	{
		xSum--;
		ySum--;
	}

	if( getColor(x,		y - 1,	lockedArray) == emptyColor)
	{
		ySum--;
	}
	if(getColor(x + 1, y - 1,	lockedArray) == emptyColor)
	{
		xSum++;
		ySum--;
	}
	if(getColor(x - 1, y,		lockedArray) == emptyColor)
	{
		xSum--;
	}
	if(getColor(x + 1, y,		lockedArray) == emptyColor)
	{
		xSum++;
	}
	if(getColor(x- 1,	y + 1,	lockedArray) == emptyColor)
	{
		xSum--;
		ySum++;
	}
	if(getColor(x,		y + 1,	lockedArray) == emptyColor)
	{
		ySum++;
	}
	if(getColor(x + 1, y + 1,	lockedArray) == emptyColor)
	{
		xSum++;
		ySum++;
	}
	normal = new hgeVector(xSum, ySum);
	return normal->Angle();
}
bool Terrain::isEdgePixel(int x, int y, DWORD* lockedArray)
{
	if(getColor(x, y, lockedArray) == emptyColor)
		return false;

	if(	getColor(x - 1,	y - 1,	lockedArray) == emptyColor || 
		getColor(x,		y - 1,	lockedArray) == emptyColor ||
		getColor(x + 1, y - 1,	lockedArray) == emptyColor ||
		getColor(x - 1, y,		lockedArray) == emptyColor ||
		getColor(x + 1, y,		lockedArray) == emptyColor ||
		getColor(x- 1,	y + 1,	lockedArray) == emptyColor ||
		getColor(x,		y + 1,	lockedArray) == emptyColor ||
		getColor(x + 1, y + 1,	lockedArray) == emptyColor)
	{
		return true;
	}
	else
		return false;
}
Terrain::~Terrain(void)
{

}

DWORD Terrain::getColor(int x, int y, DWORD* lockedArray)
{
	if(x < 0 || y < 0)
		return groundColor;
	return lockedArray[y*width+x];
}
void Terrain::setColor(int x, int y, DWORD* lockedArray, DWORD color)
{
	if(x < 0 || y < 0)
		return;
	lockedArray[y*width+x] = color;
}

void Terrain::render(HGE* hge)
{
	terrainSprite->Render(0,0);
}
bool Terrain::pointCollides(int x, int y, HGE* hge)
{
	if(x < 0 || x >= width || y < 0 || y >= height)
		return false;

	DWORD* collisionMapArray = hge->Texture_Lock(collisionMap, true, x, y, 1, 1);
	if(collisionMapArray[0] == emptyColor)
	{
		hge->Texture_Unlock(collisionMap);
		return false;
	}
	else
	{
		hge->Texture_Unlock(collisionMap);
		return true;
	}
	
}

void Terrain::makeHole(int x, int y, int radius, HGE* hge)
{
	if(x < 0 || x > width || y < 0 || y > height)
		return;
	
	int xSelection = x - radius;
	int ySelection = y - radius;
	int widthSelection = 2 * radius;
	int heightSelection = 2 * radius;
	if(xSelection < 0) {xSelection = 1;}
	if(xSelection + widthSelection >= width) {widthSelection = width - xSelection - 1 ;}
	if(ySelection < 0) {ySelection = 1;}
	if(ySelection + heightSelection >= height) {heightSelection = height - ySelection - 1;}

	//hge->System_Log("Selection size : %d * %d = %d", widthSelection, heightSelection, widthSelection * heightSelection);
	hge->System_Log("Locking resource...");
	DWORD* collisionMapArray = hge->Texture_Lock(collisionMap, false , xSelection, ySelection, widthSelection, heightSelection);
	//hge->System_Log("Making hole in terrain...");

	//creating the hole
	for(int i = 1; i < widthSelection - 1; i++)
	{
		for(int j = 1; j < heightSelection - 1; j++)
		{
			if(pow(radius - i, 2) + pow(radius - j, 2) < pow(radius, 2)) 
				setColor(i,j,collisionMapArray, emptyColor);
		}
	}

	//updating the terrain collision boundary
	for(int i = 0; i < widthSelection; i++)
	{
		for(int j = 0; j < heightSelection; j++)
		{
			if(isEdgePixel(i, j, collisionMapArray))
			{
				float angle = calculatePixelNormal(i,j,collisionMapArray);
				angle += M_PI;
				angle = (angle/(2*M_PI)) * 255;
				//hge->System_Log("TERRAIN NORMAL : %lu", highlightColor - (unsigned int)angle); 
				setColor(i,j,collisionMapArray,highlightColor - (unsigned int)angle);
			}
		}
	}
	hge->System_Log("Unlocking resource...");
	hge->Texture_Unlock(collisionMap);
	return;
	
}

float Terrain::findSurfaceCollisionNormal(int x0, int y0, int x1, int y1, HGE* hge)
{
	DWORD* lockedArray = hge->Texture_Lock(collisionMap, true);
	//bersenham's line algorithm
	int dx = abs(x1-x0);			//change in x direction
	int sx = (x0 < x1 ? 1 : -1);	//slope in x direction (neg. or pos.)

	int dy = abs(y1-y0);
	int sy = (y0 < y1 ? 1 : -1);

	int err = (dx > dy ? dx : -dy)/2;
	int e2;

	while(true)
	{
		if(getColor(x0, y0, lockedArray) != emptyColor)
		{
			hge->Texture_Unlock(collisionMap);
			lastCollisionX = x0;
			lastCollisionY = y0;
			return getColor(x0, y0, lockedArray) & 0x000000FF;
		}
		
		//setColor(x0,y0, lockedArray, highlightColor);
		if(x0 == x1 && y0 == y1)
			break;
		e2 = err;
		if(e2 > -dx)
		{
			err -= dy;
			x0 += sx;
		}
		if(e2 < dy)
		{
			err += dx;
			y0 += sy;
		}
	}
	hge->Texture_Unlock(collisionMap);
	return -1;
}

int Terrain::findAltitudeFrom(int x, int y)
{
	DWORD* lockedArray = hge->Texture_Lock(collisionMap, true, x, y, 1, height - y);
	for(int i = 0; i < height - y; i++)
	{
		if(getColor(x, i, lockedArray) != emptyColor)
		{
			hge->Texture_Unlock(collisionMap);
			return i;
		}
	}
	hge->Texture_Unlock(collisionMap);
	return -1;
}