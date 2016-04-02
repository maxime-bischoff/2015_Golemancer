#pragma once

#include "Menu/Menu.h"
#include "Menu/Menu_main.h"
#include "Menu/Menu_popup.h"

enum EGameState
{
	GAMESTATE_SPLASHSCREENS,
	GAMESTATE_TITLE,
	GAMESTATE_GAME,
	GAMESTATE_CONTROLLER,
	GAMESTATE_MAINMENU,
	GAMESTATE_PAUSE,
	GAMESTATE_CREDITS,	
	GAMESTATE_GAMEOVER,
	GAMESTATE_VICTORY
};

enum EPostFadeAction
{
	POSTFADE_STARTGAME,
	POSTFADE_WARPHOME,
	POSTFADE_WARPMAP,
	POSTFADE_WARPTUTO,
	POSTFADE_RESPAWN,
	POSTFADE_MENU,
	POSTFADE_CREDITS,
	POSTFADE_NOTHING,
	POSTFADE_VICTORY
};

class TGame
{
public:
	// :: PUBLIC

	// -- VARIABLES
#ifndef TFA_FRESH
	TGfxSprite * m_pTitleSprite;

	TGfxTexture * m_pTitleTexture;
	TGfxTexture * m_pControllerTexture;
	TGfxSprite * m_pControllerSprite;
#endif
	class TWorld * m_pWorld;

	TMainMenu
		m_tMainMenu;
	TPauseMenu
		m_tPauseMenu;
	TGameOverMenu
		m_tGameOverMenu;

	EPostFadeAction
		m_ePostFadeAction;
	EGameState
		m_eGameState;

	int
		m_iPostFadeParameter;

	// -- FUNCTIONS
	TGame();
	~TGame();

	void
		Initialize(),
		Update(),
		Render(),

		SetPostFadeAction(EPostFadeAction ePostFadeAction, int iPostFadeParameter),
		ProcessPostFadeAction(),
		PauseGame(),
		GameOver(),
		Victory(),
		PlaySplashscreens(),

		HideMenus(),
		ShowHud(bool bShow);

private:
	// :: PRIVATE
	bool
		m_bPause;
};
