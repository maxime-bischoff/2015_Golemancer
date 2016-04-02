#include "../const.h"

extern TGame * g_pGame;

// --------------------------------------------------------
// PAUSE MENU
// -------------------------------------------------------

TPauseMenu::TPauseMenu(void)
{
}
TPauseMenu::~TPauseMenu(void)
{
}

void TPauseMenu::Initialize()
{
	TButton
		tCurrentButton;

	sprintf(m_cNameMenu, "PauseMenu");
	m_iSelectedMenu = 0;

	sprintf(tCurrentButton.m_cName, "Resume");
	tCurrentButton.m_eButtonType = BT_RESUME;
	m_tButtonArray.push_back(tCurrentButton);

	sprintf(tCurrentButton.m_cName, "Restart");
	tCurrentButton.m_eButtonType = BT_RESTART;
	m_tButtonArray.push_back(tCurrentButton);

	sprintf(tCurrentButton.m_cName, "Back to home");
	tCurrentButton.m_eButtonType = BT_HOME;
	m_tButtonArray.push_back(tCurrentButton);

	sprintf(tCurrentButton.m_cName, "Back to main menu");
	tCurrentButton.m_eButtonType = BT_QUIT;
	m_tButtonArray.push_back(tCurrentButton);

	CreateMenu();


}

void TPauseMenu::Update()
{
	if (m_fActionCountDown > 0)
	{
		m_fActionCountDown -= UnscaleDeltaTime();
		if (m_fActionCountDown < 0) DelayAction();
	}

	CheckControls();
}
void TPauseMenu::StartButton()
{
	BackButton();
}

void TPauseMenu::BackButton()
{

	g_pGame->m_eGameState = GAMESTATE_GAME;
	this->Show(false);
}

void TPauseMenu::DelayAction(){

	switch (m_tButtonArray[m_iSelectedMenu].m_eButtonType   )
	{
		case BT_RESUME :  
			break;

		case BT_RESTART : 				
			g_pGame->SetPostFadeAction(POSTFADE_WARPMAP, g_pGame->m_pWorld->m_iCurrentMap);
			#ifdef TFA_FRESH
				LuaCallGlobal(GfxFreshLuaState(), "StopFxTrigger");
			#endif
			break;

		case BT_HOME:
			g_pGame->SetPostFadeAction(POSTFADE_WARPHOME, 0);
		#ifdef TFA_FRESH
				LuaCallGlobal(GfxFreshLuaState(), "StopFxTrigger");
		#endif

			break;

		case BT_QUIT : 	
			g_pGame->SetPostFadeAction(POSTFADE_MENU, 0);
			g_pGame->m_tMainMenu.MakeActive();
			this->Show(false);
			#ifdef TFA_FRESH
				LuaCallGlobal(GfxFreshLuaState(), "StopFxTrigger");
			#endif
	
#ifdef TFA_FRESH
	LuaCallGlobal(GfxFreshLuaState(), "ClearScene");
	LuaCallGlobal(GfxFreshLuaState(), "LoadMenuMap");
#endif

	break;
	}
		
	
}

void TPauseMenu::ActionButton()
{
	switch (m_tButtonArray[m_iSelectedMenu].m_eButtonType )
	{
		case BT_RESUME :  
			g_pGame->m_eGameState = GAMESTATE_GAME;
			this->Show(false);
		
#ifdef TFA_FRESH
		LuaCallGlobal(GfxFreshLuaState(), "Resume");
#endif		
		break;
	

		case BT_RESTART :	
			m_fActionDelay = 0.5f;		
			m_fActionCountDown = m_fActionDelay;
			EnableInteraction(false);

		break;

		case BT_HOME:
			m_fActionDelay = 0.5f;
			m_fActionCountDown = m_fActionDelay;
			EnableInteraction(false);

			break;

		case BT_QUIT :
			g_pGame->SetPostFadeAction(POSTFADE_MENU, 0);
			EnableInteraction(false);		
		break;
	}	
}

// --------------------------------------------------------
// WIN MENU
// -------------------------------------------------------

TWinMenu::TWinMenu(void)
{
}

TWinMenu::~TWinMenu(void)
{
}

void TWinMenu::Initialize()
{
	TButton
		tCurrentButton;

	sprintf(m_cNameMenu, "WinMenu");
	m_iSelectedMenu = 0;

	sprintf(tCurrentButton.m_cName, "Continue");
	tCurrentButton.m_eButtonType = BT_CONTINUE;
	m_tButtonArray.push_back(tCurrentButton);

	sprintf(tCurrentButton.m_cName, "Restart");
	tCurrentButton.m_eButtonType = BT_RESTART;
	m_tButtonArray.push_back(tCurrentButton);

	sprintf(tCurrentButton.m_cName, "Return To Main Menu");
	tCurrentButton.m_eButtonType = BT_QUIT;
	m_tButtonArray.push_back(tCurrentButton);

	CreateMenu();

	Show(false);
}

void TWinMenu::Update()
{
	if (m_fActionCountDown > 0)
	{
		m_fActionCountDown -= UnscaleDeltaTime();
		if (m_fActionCountDown < 0) DelayAction();
	}

	CheckControls();
}

void TWinMenu::BackButton()
{
}

void TWinMenu::DelayAction(){

	switch (m_tButtonArray[m_iSelectedMenu].m_eButtonType   )
	{
	case BT_CONTINUE : 
		break;

	case BT_RESTART:
		g_pGame->m_eGameState = GAMESTATE_GAME;
		Show(false);
	break;

	case BT_QUIT: 
		g_pGame->SetPostFadeAction(POSTFADE_MENU, 0);
		g_pGame->m_tMainMenu.MakeActive();
		this->Show(false);
#ifdef TFA_FRESH
		LuaCallGlobal(GfxFreshLuaState(), "ClearScene");
		LuaCallGlobal(GfxFreshLuaState(), "LoadMenuMap");
#endif
	break;
	}	
	
}

void TWinMenu::ActionButton()
{
	switch (m_tButtonArray[m_iSelectedMenu].m_eButtonType   )
	{
		case BT_CONTINUE :
			g_pGame->m_eGameState = GAMESTATE_GAME;
			Show(false);
		break;

		case BT_RESTART:		
			m_fActionDelay = 1.0f;		
			EnableInteraction(false);
			m_bActionPressed = true;
		break;

		case BT_QUIT :			
			m_fActionDelay = 1.0f;		
			EnableInteraction(false);
			m_bActionPressed = true;
		break;
	}	
}

// --------------------------------------------------------
// GAME OVER MENU
// -------------------------------------------------------

TGameOverMenu::TGameOverMenu(void)
{
}

TGameOverMenu::~TGameOverMenu(void)
{
}

void TGameOverMenu::Initialize()
{
	TButton	tCurrentButton;

	m_iSelectedMenu = 0;
	sprintf(m_cNameMenu, "GameOverMenu");
	m_iSelectedMenu = 0;

	sprintf(tCurrentButton.m_cName, "Restart");
	tCurrentButton.m_eButtonType = BT_RESTART;
	m_tButtonArray.push_back(tCurrentButton);

	sprintf(tCurrentButton.m_cName, "Back to home");
	tCurrentButton.m_eButtonType = BT_HOME;
	m_tButtonArray.push_back(tCurrentButton);

	CreateMenu();

	Show(false);
}

void TGameOverMenu::Update()
{
	if (m_fActionCountDown > 0)
	{
		m_fActionCountDown -= UnscaleDeltaTime();
		if (m_fActionCountDown < 0) DelayAction();
	}

	CheckControls();
}

void TGameOverMenu::BackButton()
{
}

void TGameOverMenu::StartButton()
{
}

void TGameOverMenu::DelayAction(){
		switch (m_tButtonArray[m_iSelectedMenu].m_eButtonType   )
		{
			case BT_RESTART :			
				g_pGame->SetPostFadeAction(POSTFADE_WARPMAP, g_pGame->m_pWorld->m_iCurrentMap);
			break;

			case BT_HOME : 				
				g_pGame->SetPostFadeAction(POSTFADE_WARPHOME, 0);
			break;
		}
			
	
}

void TGameOverMenu::ActionButton()
{
	switch (m_tButtonArray[m_iSelectedMenu].m_eButtonType   )
	{
		case BT_RESTART : 	
			m_fActionDelay = 0.5f;
			m_fActionCountDown = m_fActionDelay;
			EnableInteraction(false);
		
		break;

		case BT_HOME :		
			m_fActionDelay = 0.5f;
			m_fActionCountDown = m_fActionDelay;
			EnableInteraction(false);
		

		break;
	}
}