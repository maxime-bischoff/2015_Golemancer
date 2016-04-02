#include "../const.h"
extern TGame * g_pGame;

TMenu::TMenu(void)
{
	m_iSelectedColor = EGfxColor_Red;
	m_iUnselectedColor = EGfxColor_White;

	m_fLastChangeTime = 0.0f;
	m_fChangeDelay = 0.2f;
	m_bActionPressed = false;
	m_bInteractionAllowed = false;

#ifndef TFA_FRESH
	m_pBackgroundSprite = 0;
#endif
}

TMenu::~TMenu(void)
{
}

void TMenu::Initialize()
{
	return;
}

void TMenu::EnableInteraction(bool bValue){
	m_bInteractionAllowed = bValue;
}

void TMenu::MakeActive(){
	Show(true);
	ResetSelected();

	m_bActionPressed = false;
	m_fActionCountDown = 0;
	m_bInteractionAllowed = false;
	if (fabs(GfxInputGetPadStickLeftY()) < 0.5f)
	{
		EnableInteraction(true);
	}
}

void TMenu::Show(bool bValue){

#ifdef TFA_FRESH	
	lua_pushstring(GfxFreshLuaState(), m_cNameMenu);
	lua_pushboolean(GfxFreshLuaState(), bValue);
	LuaCallGlobal(GfxFreshLuaState(), "ShowMenu");
#endif

	if( bValue )
	m_bInteractionAllowed = true;
}

void TMenu::ResetSelected()
{
	for (int i = 0; i < (int)m_tButtonArray.size() ; i++)
	{
#ifndef TFA_FRESH
		GfxSpriteSetColor(m_tButtonArray[i].m_pTextSprite, m_iUnselectedColor);
#else

		lua_pushstring(GfxFreshLuaState(), m_cNameMenu);
		lua_pushinteger(GfxFreshLuaState(), i );
		lua_pushboolean(GfxFreshLuaState(), false);

		LuaCallGlobal(GfxFreshLuaState(), "SelectButton");
#endif
	}

	m_iSelectedMenu = 0;

#ifndef TFA_FRESH
	GfxSpriteSetColor(m_tButtonArray[m_iSelectedMenu].m_pTextSprite, m_iSelectedColor);

#else

	lua_pushstring(GfxFreshLuaState(), m_cNameMenu);
	lua_pushinteger(GfxFreshLuaState(), 0);
	lua_pushboolean(GfxFreshLuaState(), true);

	LuaCallGlobal(GfxFreshLuaState(), "SelectButton");
#endif
}

void TMenu::CreateMenu()
{
	

	for (int i = 0; i < (int)m_tButtonArray.size() ; i++)
	{
#ifndef TFA_FRESH
		float fTextScale = 2.5f;
		m_tButtonArray[i].m_pTextSprite = GfxTextSpriteCreate();
		GfxTextSpriteSetText(m_tButtonArray[i].m_pTextSprite, m_tButtonArray[i].m_cName);
		GfxSpriteSetScale(m_tButtonArray[i].m_pTextSprite, fTextScale, fTextScale);
		GfxSpriteSetFilteringEnabled(m_tButtonArray[i].m_pTextSprite, false);
		GfxSpriteSetColor(m_tButtonArray[i].m_pTextSprite, EGfxColor_Red);
		GfxSpriteSetPosition(m_tButtonArray[i].m_pTextSprite, GfxGetDisplaySizeX() / 2.0f - GfxSpriteGetSizeX(m_tButtonArray[i].m_pTextSprite)*fTextScale / 2.0f, 50.0f + i *35.0f);

#else

		lua_pushstring(GfxFreshLuaState(), m_cNameMenu);
		lua_pushstring(GfxFreshLuaState(), m_tButtonArray[i].m_cName);
		lua_pushinteger(GfxFreshLuaState(), i + 1);	
		LuaCallGlobal(GfxFreshLuaState(), "CreateButton");

#endif

	}

	ResetSelected();
}

void TMenu::Update()
{
	return;
}

void TMenu::DelayAction(){
	return;
}

void TMenu::CheckControls()
{
#ifndef TFA_FRESH
	TButton	tSelectedObject = m_tButtonArray[m_iSelectedMenu];
#endif 
	bool
		bInput = false;
	

	

	if (!m_bInteractionAllowed){
		if (fabs(GfxInputGetPadStickLeftY()) < 0.5f)
		{
			m_bInteractionAllowed = true;
		}
		else return;
	};

	m_fLastChangeTime += GfxTimeFrameGetCurrentDurationInSeconds();

	if (m_fLastChangeTime >= m_fChangeDelay){
		m_bUpAllowedToPressed = true;
		m_bDownAllowedToPressed = true;
	}

	float fLeftStickValueY = GfxInputGetPadStickLeftY();

	if (m_bUpJustPressed) m_bUpJustPressed = false;
	if (m_bDownJustPressed) m_bDownJustPressed = false;

	if (fLeftStickValueY >= 0.65f && m_bUpAllowedToPressed)
	{
		m_bUpJustPressed = true;
		m_bUpAllowedToPressed = false;
		m_fLastChangeTime = 0;
	}

	if (fLeftStickValueY <= 0.1f)
	{
		m_bUpAllowedToPressed = true;
	}

	if (fLeftStickValueY <= -0.65f  && m_bDownAllowedToPressed)
	{
		m_bDownJustPressed = true;
		m_bDownAllowedToPressed = false;
		m_fLastChangeTime = 0;
	}

	if (fLeftStickValueY >= -0.1f)
	{
		m_bDownAllowedToPressed = true;
	}

	if (GfxInputIsJustPressed(EGfxInputID_KeyArrowDown) || GfxInputIsJustPressed(EGfxInputID_AnyPadDown) || m_bDownJustPressed)
	{
		bInput = true;

		// :: With array
		if (m_iSelectedMenu < (int)m_tButtonArray.size() - 1)
		{
			m_iNewButton = m_iSelectedMenu + 1;
		}
		else
		{
			m_iNewButton = 0;
		}
	}
	else	if (GfxInputIsJustPressed(EGfxInputID_KeyArrowUp) || GfxInputIsJustPressed(EGfxInputID_AnyPadUp) || m_bUpJustPressed)
	{
		bInput = true;
		// :: With array
		if ( m_iSelectedMenu > 0 )
		{
			m_iNewButton = m_iSelectedMenu - 1;
		}
		else
		{
			m_iNewButton = m_tButtonArray.size() - 1;
		}
	}

	if ( bInput )
	{
	
#ifndef TFA_FRESH
		GfxSpriteSetColor(m_tButtonArray[m_iNewButton].m_pTextSprite, m_iSelectedColor);
		GfxSpriteSetColor(tSelectedObject.m_pTextSprite, m_iUnselectedColor);
#else

		lua_pushstring(GfxFreshLuaState(), m_cNameMenu);
		lua_pushinteger(GfxFreshLuaState(),  m_iNewButton);
		lua_pushboolean(GfxFreshLuaState(), true);
		LuaCallGlobal(GfxFreshLuaState(), "SelectButton");

		lua_pushstring(GfxFreshLuaState(), m_cNameMenu);
		lua_pushinteger(GfxFreshLuaState(), m_iSelectedMenu);
		lua_pushboolean(GfxFreshLuaState(), false);
		LuaCallGlobal(GfxFreshLuaState(), "SelectButton");

#endif
		m_iSelectedMenu = m_iNewButton;
	}

	if ( GfxInputIsJustPressed(EGfxInputID_KeySpace)  || GfxInputIsJustPressed(EGfxInputID_AnyPadCross))
	{
		ActionButton();
	}

	if (GfxInputIsJustPressed(EGfxInputID_KeyReturn) || GfxInputIsJustPressed(EGfxInputID_AnyPadStart))
	{
		StartButton();
	}

	if (GfxInputIsJustPressed(EGfxInputID_KeyBackspace) || GfxInputIsJustPressed(EGfxInputID_KeyEscape) ||
		GfxInputIsJustPressed(EGfxInputID_AnyPadCircle) || GfxInputIsJustPressed(EGfxInputID_AnyPadTriangle))
	{
		BackButton();
	}
}

void TMenu::Render()
{
#ifndef TFA_FRESH
	if (m_pBackgroundSprite != 0)
	{
		GfxSpriteRender(m_pBackgroundSprite);
	}

	for (int i = 0; i < (int)m_tButtonArray.size() ; i++)
	{
		if (m_tButtonArray[i].m_pTextSprite != 0)
		{
			GfxSpriteRender(m_tButtonArray[i].m_pTextSprite);
		}
	}

#endif
}

