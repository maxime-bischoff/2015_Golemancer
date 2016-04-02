#include "Const.h"







TGame * g_pGame;



void Initialize()
{


	g_pGame = new TGame();
	g_pGame->Initialize();
}


void Update()
{
	g_pGame->Update();
	
}

void Render()
{
#ifndef TFA_FRESH
	GfxClear(GfxColor(25 - 25 * TTime::GetTimeScale(), 25 - 25 * TTime::GetTimeScale(), 25 - 25 * TTime::GetTimeScale(), 100));
#endif
	g_pGame->Render();
}



void GfxMain( int, char * [] )
{
	GfxDefaultResolution(1280, 720);
	GfxCallbacks(Initialize, Update, Render);
}
