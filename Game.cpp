#include "Const.h"

TGame::TGame()
{
	m_pWorld = new TWorld();
	#ifdef TFA_FRESH
		m_eGameState = GAMESTATE_SPLASHSCREENS;
		PlaySplashscreens();
#else
	m_eGameState = GAMESTATE_MAINMENU;
#endif
	m_ePostFadeAction = POSTFADE_NOTHING;
	m_iPostFadeParameter = 0;
#ifndef TFA_FRESH
	m_pTitleTexture = GfxTextureLoad("Data/wombat.tga");
	m_pTitleSprite = GfxSpriteCreate(m_pTitleTexture);
	float fScaleX = (GfxSpriteGetSizeY(m_pTitleSprite)*1.0f) / (GfxGetDisplaySizeY()*1.0f);

	GfxSpriteSetScale(m_pTitleSprite, 1.0f / fScaleX, 1.0f / fScaleX);

	float  offset = GfxSpriteGetSizeX(m_pTitleSprite)*(1.0f / fScaleX) - GfxGetDisplaySizeX();
	GfxSpriteSetPosition(m_pTitleSprite, -offset / 2.0f, 0);

	m_pControllerTexture = GfxTextureLoad("Data/controller.tga");
	m_pControllerSprite = GfxSpriteCreate(m_pControllerTexture);
	fScaleX = (GfxSpriteGetSizeY(m_pControllerSprite)*1.0f) / (GfxGetDisplaySizeY()*1.0f);

	GfxSpriteSetScale(m_pControllerSprite, 1.0f / fScaleX, 1.0f / fScaleX);

	offset = GfxSpriteGetSizeX(m_pControllerSprite)*(1.0f / fScaleX) - GfxGetDisplaySizeX();
	GfxSpriteSetPosition(m_pControllerSprite, -offset / 2.0f, 0);

#endif
}

TGame::~TGame()
{
}

void TGame::Initialize()
{
	m_tMainMenu.Initialize();

	m_tPauseMenu.Initialize();
	m_tGameOverMenu.Initialize();
	m_pWorld->Initialize();

	m_tPauseMenu.Show(false);

#ifdef TFA_FRESH
	m_tMainMenu.Show(false);
	LuaCallGlobal(GfxFreshLuaState(), "LoadTitleScreen");

#endif
}

void TGame::SetPostFadeAction(EPostFadeAction ePostFadeAction, int iPostFadeParameter)
{
	TFade::Instance().FadeToBlack();
	m_ePostFadeAction = ePostFadeAction;
	m_iPostFadeParameter = iPostFadeParameter;
}

void TGame::ProcessPostFadeAction()
{
	switch (m_ePostFadeAction)
	{
	case POSTFADE_WARPHOME:
		m_pWorld->WarpHome();
		m_eGameState = GAMESTATE_GAME;
		HideMenus();
		ShowHud(true);
		break;

	case POSTFADE_WARPTUTO:
		m_pWorld->WarpTuto();
		m_eGameState = GAMESTATE_GAME;
		HideMenus();
		ShowHud(true);
		break;

	case POSTFADE_WARPMAP:
		m_pWorld->WarpMap(m_iPostFadeParameter);
		m_eGameState = GAMESTATE_GAME;
		HideMenus();
		ShowHud(true);
		break;

	case POSTFADE_RESPAWN:
		break;

	case POSTFADE_CREDITS:
		m_eGameState = GAMESTATE_CREDITS;
#ifdef TFA_FRESH
		lua_pushboolean(GfxFreshLuaState(), true);
		LuaCallGlobal(GfxFreshLuaState(), "ShowCredits");
#endif
		HideMenus();
		ShowHud(false);
		break;

	case POSTFADE_MENU:
		if ( m_eGameState == GAMESTATE_CREDITS )
		{
#ifdef TFA_FRESH
			lua_pushboolean ( GfxFreshLuaState(), false ) ;
			LuaCallGlobal(GfxFreshLuaState(), "ShowCredits");
#endif
		}
		m_eGameState = GAMESTATE_MAINMENU;
		HideMenus();
		m_tMainMenu.Show(true);
		ShowHud(false);

#ifdef TFA_FRESH

		lua_pushboolean(GfxFreshLuaState(), false);
		LuaCallGlobal(GfxFreshLuaState(), "ShowVitoryScreen");

		lua_pushinteger(GfxFreshLuaState(), true);
		LuaCallGlobal(GfxFreshLuaState(), "ShowMainMenu");

		lua_pushinteger(GfxFreshLuaState(), false);
		LuaCallGlobal(GfxFreshLuaState(), "ShowSplashscreens");
#endif

		break;

	case POSTFADE_STARTGAME :
		m_pWorld->Initialize();
		break;
	case POSTFADE_VICTORY:
		m_eGameState = GAMESTATE_VICTORY;
#ifdef TFA_FRESH
		lua_pushboolean(GfxFreshLuaState(), true);
		LuaCallGlobal(GfxFreshLuaState(), "ShowVitoryScreen");

#endif
		break;
	}

	

}

void TGame::PauseGame()
{
	m_eGameState = GAMESTATE_PAUSE;
	m_tPauseMenu.Show(true);
	m_tPauseMenu.ResetSelected();
}

void TGame::GameOver()
{
	m_eGameState = GAMESTATE_GAMEOVER;
	m_tGameOverMenu.Show(true);
	m_tGameOverMenu.ResetSelected();
}

void TGame::HideMenus()
{
	m_tMainMenu.Show(false);
	m_tPauseMenu.Show(false);
	m_tGameOverMenu.Show(false);
#ifdef TFA_FRESH
	lua_pushboolean(GfxFreshLuaState(), false);
	LuaCallGlobal(GfxFreshLuaState(), "ShowMainMenu");

#endif
}

void TGame::PlaySplashscreens()
{
#ifdef TFA_FRESH
	LuaCallGlobal(GfxFreshLuaState(), "PlaySplashcreens");

#endif
}

void TGame::ShowHud(bool bShow)
{
	bShow = bShow;
#ifdef TFA_FRESH
	LuaShowHud(bShow);
#endif
}
void TGame::Victory()
{
	SetPostFadeAction(POSTFADE_VICTORY, 0);
}

void TGame::Update()
{
	TFade::Instance().Update();

#ifdef TFA_FRESH
	if(GfxInputIsPressed( EGfxInputID_AnyPadShoulderL) && GfxInputIsPressed(EGfxInputID_AnyPadLeft))
	{
		LuaCallGlobal(GfxFreshLuaState(), "DecreaseGamma");
	}

	if (GfxInputIsPressed(EGfxInputID_AnyPadShoulderL) && GfxInputIsPressed(EGfxInputID_AnyPadRight))
	{
		LuaCallGlobal(GfxFreshLuaState(), "IncreaseGamma");
	}
#endif

	switch (m_eGameState)
	{
		case GAMESTATE_GAME:
		{
			if (GfxInputIsJustPressed(EGfxInputID_AnyPadSelect))
			{
				m_eGameState = GAMESTATE_CONTROLLER;
			}
			else
			{
				m_pWorld->Update();
			}
		}
		break;

		case GAMESTATE_CONTROLLER:
		{
			if (GfxInputIsJustPressed(EGfxInputID_AnyPadStart) || GfxInputIsJustPressed(EGfxInputID_AnyPadSelect) || GfxInputIsJustPressed(EGfxInputID_AnyPadCross) || GfxInputIsJustPressed(EGfxInputID_KeySpace) || GfxInputIsJustPressed(EGfxInputID_KeyReturn))
			{
				m_eGameState = GAMESTATE_GAME;
			}
		}
		break;

	case GAMESTATE_MAINMENU:	 
		m_tMainMenu.Update();
	 break;
	case GAMESTATE_PAUSE:		 m_tPauseMenu.Update();
								 break;
	case GAMESTATE_CREDITS :
		HideMenus();
		if (GfxInputIsPressed(EGfxInputID_AnyPadCircle) || GfxInputIsPressed(EGfxInputID_AnyPadTriangle))
		{
			SetPostFadeAction(POSTFADE_MENU, 0);
		}
		break;
	case GAMESTATE_GAMEOVER:
	{
		m_tGameOverMenu.Update();
		m_pWorld->Update();
	}
	break;

	case GAMESTATE_SPLASHSCREENS:
	{
		if(GfxInputIsJustPressed(EGfxInputID_AnyPadStart) || GfxInputIsJustPressed(EGfxInputID_AnyPadCross) || GfxInputIsJustPressed(EGfxInputID_AnyPadTriangle))
		{
			SetPostFadeAction(POSTFADE_MENU,0);
		}

#ifdef TFA_FRESH

		LuaCallGlobal(GfxFreshLuaState(), "IsSplashScreenFinished");
		int  isFinished = luaL_checkinteger(GfxFreshLuaState(), 1);
		lua_pop(GfxFreshLuaState(), 1);

		if (isFinished == 1)
		{
			m_eGameState =  GAMESTATE_MAINMENU;
			SetPostFadeAction(POSTFADE_MENU, 0);
		}
#endif
		break;
	}

	case GAMESTATE_VICTORY:
	{

		if (GfxInputIsJustPressed(EGfxInputID_AnyPadStart) || GfxInputIsJustPressed(EGfxInputID_AnyPadCross) || GfxInputIsJustPressed(EGfxInputID_AnyPadTriangle))
		{
			SetPostFadeAction(POSTFADE_MENU, 0);

		}
	}
	break;
	default:
		break;
	}
}
void TGame::Render()
{
#ifndef TFA_FRESH

	switch (m_eGameState)
	{
	case GAMESTATE_TITLE:GfxSpriteRender(m_pTitleSprite);

		break;
	case GAMESTATE_GAME:	m_pWorld->Render();
		break;

	case GAMESTATE_CONTROLLER: GfxSpriteRender(m_pControllerSprite); break;

	case GAMESTATE_MAINMENU:
		m_tMainMenu.Render();
		break;
	case GAMESTATE_PAUSE:
		m_pWorld->Render();
		m_tPauseMenu.Render();
		break;

	case GAMESTATE_GAMEOVER:
		m_pWorld->Render();
		m_tGameOverMenu.Render();
		break;

	default:
		break;
	}

	TFade::Instance().Render();
#endif
}