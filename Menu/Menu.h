#pragma once

#include "../SpellEnum.h"
#include <vector>
class TMenu
{
	// :: PUBLIC
public:
	// :: TYPES
	enum EButtonType{
		BT_RESUME,
		BT_RESTART,
		BT_MAINMENU,
		BT_NEWGAME,
		BT_CONTINUE,
		BT_HOME,
		BT_BACK,		
		BT_SPELL,
		BT_OPTIONS,
		BT_CREDITS,

		BT_QUIT,
	};

	struct TButton
	{
#ifndef TFA_FRESH
		TGfxSprite * m_pTextSprite;
#endif
		char m_cName[32];
		EButtonType m_eButtonType;		
		ESpell m_eSpell;
	};

	// :: VARIABLES
	char m_cNameMenu[32];

	unsigned int m_iSelectedColor;
	unsigned int m_iUnselectedColor;

	bool m_bUpAllowedToPressed;
	bool m_bDownAllowedToPressed;
	bool m_bUpJustPressed;
	bool m_bDownJustPressed;
	float m_fLastChangeTime;
	float m_fChangeDelay;

	bool m_bInteractionAllowed;

	int	m_iSelectedMenu;
	int m_iNewButton;

	bool m_bActionPressed;

	float m_fActionDelay;
	float m_fActionCountDown;



	std::vector<TButton> m_tButtonArray;

#ifndef TFA_FRESH
	TGfxTexture * m_pBackgroundTexture;
	TGfxSprite * m_pBackgroundSprite;
#endif

	// :: METHODES
	TMenu(void);
	virtual ~TMenu(void);

	virtual void ActionButton(){ return; };
	virtual void BackButton(){ return; };
	virtual void StartButton(){ return; }

	void CreateMenu();
	void ResetSelected();
	virtual void Update();
	void DelayAction();
	void CheckControls();
	void MakeActive();
	void Render();
	
	

	virtual void Initialize() = 0;

	void EnableInteraction(bool bValue);
	void Show(bool bValue);

	// :: PRIVATE
private:

	// :: PROTECTED
protected:
};
