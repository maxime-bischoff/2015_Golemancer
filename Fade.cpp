#include "Const.h"
extern TGame * g_pGame;
TFade *  TFade::m_pFade = 0;

TFade::TFade()
{
#ifndef TFA_FRESH

	TGfxTexture * m_pFadeTexture = GfxTextureLoad("Data/Hud/Fade_Panel.tga");
	m_pFadeSprite = GfxSpriteCreate(m_pFadeTexture);

	float fScaleX = (GfxSpriteGetSizeY(m_pFadeSprite)*1.0f) / (GfxGetDisplaySizeY()*1.0f);

	GfxSpriteSetScale(m_pFadeSprite, 1.0f / fScaleX, 1.0f / fScaleX);

	float  offset = GfxSpriteGetSizeX(m_pFadeSprite)*(1.0f / fScaleX) - GfxGetDisplaySizeX();
	GfxSpriteSetPosition(m_pFadeSprite, -offset / 2.0f, 0);


#endif
	m_fFadingDurationToBlack = 0.75f;
	m_fFadingDurationToClear = 0.5f;
	m_eFadeState = FADE_CLEAR;
	m_fOpacity = 0;
}


TFade::~TFade()
{
#ifndef TFA_FRESH
	GfxSpriteDestroy(m_pFadeSprite);
#endif
}


 TFade &  TFade::Instance()
{
	 if (m_pFade == 0) m_pFade = new TFade();

	 return *m_pFade;
}

void TFade::Update()
{
	if (m_eFadeState == FADING_TO_BLACK)
	{
		m_fOpacity += UnscaleDeltaTime() / m_fFadingDurationToBlack;
		if (m_fOpacity > 1.0f){
		
			m_fOpacity = 1.0f;
			m_eFadeState = FADE_BLACK;	
			g_pGame->ProcessPostFadeAction();		
		}

	}
	else if (m_eFadeState == FADING_TO_CLEAR)
	{
		m_fOpacity -= UnscaleDeltaTime() / m_fFadingDurationToClear ;
		if (m_fOpacity < 0.0f){

			m_fOpacity = 0.0f;
			m_eFadeState = FADE_CLEAR;
		
		}
	}
	else if(m_eFadeState == FADE_BLACK)
	{
		m_eFadeState = FADING_TO_CLEAR;
	}

#ifndef TFA_FRESH
	GfxSpriteSetColor(m_pFadeSprite, GfxColor(255, 255, 255, m_fOpacity * 255.0f));
#else

	lua_pushnumber(GfxFreshLuaState(), m_fOpacity);
	LuaCallGlobal(GfxFreshLuaState(), "SetFadeOpacity");

#endif

}


void TFade::FadeToBlack()
{
	if (m_eFadeState != FADING_TO_BLACK || m_eFadeState != FADE_BLACK)
	{
		m_eFadeState = FADING_TO_BLACK;
	}
}


void TFade::FadeToClear()
{
	if (m_eFadeState != FADING_TO_CLEAR || m_eFadeState != FADE_CLEAR)
	{
		m_eFadeState = FADING_TO_CLEAR;
	}
}

void TFade::SetBlack()
{
	m_fOpacity = 1.0f;
	m_eFadeState = FADE_BLACK;
}

void TFade::SetClear()
{
	m_fOpacity = 0.0f;
}

void TFade::Render()
{

#ifndef TFA_FRESH
	GfxSpriteRender(m_pFadeSprite);
#endif

}