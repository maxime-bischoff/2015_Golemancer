#include "Const.h"

extern TGame * g_pGame;

const float CLOSING_TIME = 0.7f;
const float OPENING_DIST = 2.0f;
TDoor::TDoor(TGfxVec3 tPos, EDoor eOrient) : tDestRoom(0), pParentRoom(0), m_tRadius(TGfxVec2(2.5f, 0.5f)), m_eDoorState(DOOR_OPEN), m_pWallLeft(0), m_pWallRight(0)
{
	m_fDistance = 0;
	m_tPos = tPos;
	m_eOrientation = eOrient;

	switch (m_eOrientation)
	{
	case EDOOR_RIGHT:m_fAngle = PI_2; break;
	case EDOOR_BOTTOM:m_fAngle = -PI; break;
	case EDOOR_LEFT:m_fAngle = -PI_2; break;
	case EDOOR_TOP:m_fAngle = PI; break;
	default: m_fAngle = 0; break;
	}
	TGfxVec2 offset = TGfxVec2(cosf(m_fAngle), sinf(m_fAngle)) *( m_tRadius.x /2.0f - 0.2f);


	m_pWallLeft = new TWall(TGfxVec2(m_tPos.x, m_tPos.z) + offset, m_tRadius / 2.0f, m_fAngle);
	m_pWallRight = new TWall(TGfxVec2(m_tPos.x, m_tPos.z) - offset, m_tRadius / 2.0f, m_fAngle);




}

void TDoor::Open(){

	if (m_eDoorState == DOOR_CLOSED)
	{

		GfxDbgPrintf("OpenDoor \n");

		m_eDoorState = DOOR_OPENING;
		m_fCountDown = CLOSING_TIME;
#ifdef TFA_FRESH
		lua_pushinteger(GfxFreshLuaState(), m_iId);
		LuaCallGlobal(GfxFreshLuaState(), "OpenDoor");
#endif
	}

}


void TDoor::Close()
{
	
	if (m_eDoorState == DOOR_OPEN)
	{
		m_fCountDown = CLOSING_TIME;
		m_eDoorState = DOOR_CLOSING;

#ifdef TFA_FRESH
		lua_pushinteger(GfxFreshLuaState(), m_iId);
		LuaCallGlobal(GfxFreshLuaState(), "CloseDoor");
#endif
	}

}

void TDoor::Init()
{
	float fDistance = m_tRadius.x / 2.0f - 0.2f;
	TGfxVec2 offset = TGfxVec2(cosf(m_fAngle), sinf(m_fAngle)) * fDistance;
	m_pWallLeft = new TWall(TGfxVec2(m_tPos.x, m_tPos.z) + offset, m_tRadius / 2.0f, m_fAngle);
	m_pWallRight = new TWall(TGfxVec2(m_tPos.x, m_tPos.z) - offset, m_tRadius / 2.0f, m_fAngle);

}
void TDoor::Update()
{
	TPlayer * pPlayer = g_pGame->m_pWorld->m_pPlayer;

	if (m_fCountDown > 0)
	{

		if (m_eDoorState == DOOR_CLOSING)
		{
			float fDistance = m_fCountDown / CLOSING_TIME  * OPENING_DIST+ m_tRadius.x/2.0f -0.2f ;
			TGfxVec2 offset = TGfxVec2(cosf(m_fAngle), sinf(m_fAngle)) * fDistance;
			m_pWallLeft = new TWall(TGfxVec2(m_tPos.x, m_tPos.z) + offset, m_tRadius / 2.0f, m_fAngle);
			m_pWallRight = new TWall(TGfxVec2(m_tPos.x, m_tPos.z) - offset, m_tRadius / 2.0f, m_fAngle);
			
		}
		else if (m_eDoorState == DOOR_OPENING)
		{
			float fDistance = (CLOSING_TIME - m_fCountDown) / CLOSING_TIME  * OPENING_DIST + m_tRadius.x / 2.0f - 0.2f ;
			TGfxVec2 offset = TGfxVec2(cosf(m_fAngle), sinf(m_fAngle)) * fDistance;
			m_pWallLeft = new TWall(TGfxVec2(m_tPos.x, m_tPos.z) + offset, m_tRadius / 2.0f, m_fAngle);
			m_pWallRight = new TWall(TGfxVec2(m_tPos.x, m_tPos.z) - offset, m_tRadius / 2.0f, m_fAngle);

		}

		m_fCountDown -= DeltaTime();
		if (m_fCountDown <= 0)
		{
			if (m_eDoorState == DOOR_CLOSING)
				m_eDoorState = DOOR_CLOSED;
			else if (m_eDoorState == DOOR_OPENING)
				m_eDoorState = DOOR_OPEN;
		}


	}


	if (m_eDoorState != DOOR_SEALED || m_eDoorState != DOOR_LOCKED)
	{
		if ((pPlayer->Position() - TGfxVec2(m_tPos.x, m_tPos.z)).SquaredLength() < 36.0f)
		{
			Open();
		}
		else{
			Close();
		}
	}
}

void TDoor::Lock(){
	if (m_eDoorState != DOOR_SEALED)
	{
		m_eDoorState = DOOR_LOCKED;
#ifdef TFA_FRESH
		lua_pushinteger(GfxFreshLuaState(), m_iId);
		LuaCallGlobal(GfxFreshLuaState(), "LockDoor");
#endif
	}

}

void TDoor::Seal(){

		m_eDoorState = DOOR_SEALED;
	
#ifdef TFA_FRESH
	//lua_pushinteger(GfxFreshLuaState(), m_iId);
	//LuaCallGlobal(GfxFreshLuaState(), "SealDoor");
#endif
}


void TDoor::Unlock()
{

	if (m_eDoorState == DOOR_LOCKED)
	{
		m_eDoorState = DOOR_CLOSED;

#ifdef TFA_FRESH
		lua_pushinteger(GfxFreshLuaState(), m_iId);
		LuaCallGlobal(GfxFreshLuaState(), "UnlockDoor");
#endif
	}


}

void TDoor::Draw(TGfxSprite * pLineSprite)
{

#ifndef TFA_FRESH
	unsigned int iColor = GfxColor(255, 255, 0, 255);
	switch (m_eDoorState)
	{
	case DOOR_CLOSING:  iColor = GfxColor(255, 255, 0, 255);
		break;
	case DOOR_CLOSED:   iColor = GfxColor(255, 128, 0, 255);
		break;
	case DOOR_OPENING:  iColor = GfxColor(0, 255, 128, 255);
		break;
	case DOOR_OPEN:     iColor = GfxColor(0, 255, 0, 255);
		break;
	case DOOR_SEALED:   iColor = GfxColor(0, 255, 255, 255);
		break;
	case DOOR_LOCKED:   iColor = GfxColor(255, 0, 0, 255);
		break;
	default:
		break;
	}
	
	DrawCrossBox(pLineSprite, TGfxVec2( m_tPos.x, m_tPos.z), m_fAngle,  m_tRadius, iColor);

	m_pWallLeft->Draw(pLineSprite);
	m_pWallRight->Draw(pLineSprite);


#endif
	//else 
		//DrawCrossBox(pLineSprite, TGfxVec2(m_tPos.x, m_tPos.z), fAngle, TGfxVec2( m_tRadius.y, m_tRadius.x) , iColor);

}






