#include "../const.h"

extern TGame * g_pGame;

TMainMenu::TMainMenu()
{
}

TMainMenu::~TMainMenu()
{
}

void TMainMenu::BackButton()
{
}

void TMainMenu::ActionButton()
{
	switch (m_tButtonArray[m_iSelectedMenu].m_eButtonType)
	{
	case  BT_NEWGAME:
		g_pGame->SetPostFadeAction(POSTFADE_WARPHOME, 0);
		break;

	case  BT_CREDITS:
		g_pGame->SetPostFadeAction(POSTFADE_CREDITS, 0);
		g_pGame->m_tMainMenu.EnableInteraction(false);

	break;

	default:
		break;
	}
}

void TMainMenu::Initialize()
{
	TButton	tCurrentButton;

	sprintf(m_cNameMenu, "MainMenu");
	m_iSelectedMenu = 0;

	tCurrentButton.m_eButtonType = BT_NEWGAME;
	sprintf(tCurrentButton.m_cName, "Start");
	m_tButtonArray.push_back(tCurrentButton);

	tCurrentButton.m_eButtonType = BT_CREDITS;
	sprintf(tCurrentButton.m_cName, "Credits");
	m_tButtonArray.push_back(tCurrentButton);

	CreateMenu();

#ifdef TFA_FRESH
	LuaCallGlobal(GfxFreshLuaState(), "InitMainMenu");
#endif
}

void TMainMenu::Update()
{
	CheckControls();
}