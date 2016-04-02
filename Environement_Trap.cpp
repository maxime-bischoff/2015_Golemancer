#include "Const.h"

extern TGame * g_pGame;


TEnvironment_Trap::TEnvironment_Trap( TGfxVec2 tPos, int iIndex )
	:m_fTimer ( 60 ),
	m_bActivated( true ), 
	m_iIndexTrigger(0),
	m_iCountActivable(0)
{
	m_tPos = tPos; 
	m_iIndexTrigger =  iIndex;
}


// :: Constructor par default pour les childs :: // 
TEnvironment_Trap::TEnvironment_Trap() 
	:m_fTimer ( 500 ),
	m_bActivated( true ), 
	m_iIndexTrigger(0),
	m_iCountActivable(0)
{
	m_iColor = GfxColor ( 255,125,125,125 ) ; 

}

void TEnvironment_Trap::Update()
{
	static float fCurrentTimer = 0.0f ; 
	// :: TIMER DECREASE :: // 
	if ( m_bActivated )
	{
		fCurrentTimer += DeltaTime() ; 
		if  ( fCurrentTimer >= m_fTimer ) 
		{
			m_bActivated = false ; 
			// :: il sera réactivé apres l'avoir tapé :: // 
			fCurrentTimer = 0.0f ; 
			m_iColor = GfxColor ( 0,255,255,255) ; 
		}
	}
}

void TEnvironment_Trap::Render ()
{
#ifndef TFA_FRESH
	DrawBox(g_pGame->m_pWorld->m_pLineSprite, m_tPos, m_fAngle, m_tRadius, m_iColor);
#endif 
}