#include "Entity.h"
#include "Level.h"
#include <hge.h>

HGE* hge;
Level* level;

bool FrameFunc()
{
	// quit on escape
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;

	//step the level
	level->logicStep();

	// Continue execution
	return false;
}

bool RenderFunc()
{
	hge->Gfx_BeginScene();
	hge->Gfx_Clear(0);

	level->render();
	
	hge->Gfx_EndScene();

	return false;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);

	static int XRES = 1024;
	static int YRES = 800;

	// Set up log file, frame function, render function and window title
	hge->System_SetState(HGE_LOGFILE, "void.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "The Void");

	// Set up video mode
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, XRES);
	hge->System_SetState(HGE_SCREENHEIGHT, YRES);
	hge->System_SetState(HGE_SCREENBPP, 32);

	if(hge->System_Initiate())
	{
		level = new Level(hge, XRES, YRES);
		hge->System_Log("Level Created!");
		//level->createPlayer(400,300);
		hge->System_Start();
	}
	else MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);

	// Clean up and shutdown
	hge->System_Shutdown();
	hge->Release();
	return 0;
}
