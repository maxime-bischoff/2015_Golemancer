#pragma once


enum EFade_State
{
	FADING_TO_BLACK,
	FADING_TO_CLEAR,
	FADE_BLACK,
	FADE_CLEAR
};

class TFade
{
public:
	TFade();
	~TFade();
	static TFade & Instance();

	EFade_State GetState(){ return m_eFadeState; };
	void Update();
	void FadeToBlack();
	void FadeToClear();
	void SetBlack();
	void SetClear();

	void Render();

private:

#ifndef TFA_FRESH
	TGfxSprite * m_pFadeSprite;
#endif

	static TFade * m_pFade ;
	float m_fOpacity;
	float m_fFadingDurationToClear;
	float m_fFadingDurationToBlack;
	EFade_State m_eFadeState;
	


};

