#pragma once
#include "flib.h"


enum EDoor{
	EDOOR_LEFT = 0,
	EDOOR_RIGHT = 1,
	EDOOR_TOP = 2,
	EDOOR_BOTTOM = 3,
};

enum EDoorState
{
	DOOR_CLOSING,
	DOOR_CLOSED,
	DOOR_OPENING,
	DOOR_OPEN,
	DOOR_SEALED,
	DOOR_LOCKED
};


enum ESocketType
{
	SOCKET_NORMAL,
	SOCKET_EMPTY
};

struct TDoor{

	ESocketType m_eSocketType;

	TDoor(TGfxVec3 tPos, EDoor eOrient); 
	int m_iId;
	struct TRoom * tDestRoom;
	struct TRoom * pParentRoom;
	TGfxVec3 m_tPos;
	float m_fAngle;
	TGfxVec2 m_tRadius;
	EDoorState m_eDoorState; 
	EDoor m_eOrientation;
	TWall * m_pWallLeft;
	TWall * m_pWallRight;
	float m_fCountDown;
	float m_fDistance;
	
	
	

	void Open();
	void Close();
	void Init();
	void Lock();
	void Unlock();

	void Seal();

	void Update();

	void Draw(TGfxSprite * pLineSprite);	

};

struct TEntrance{

	TEntrance() : bExist(false), m_pDoor(0), tDestRoom(0) { tDestRoom = 0; }

	TDoor * m_pDoor;
	struct TRoom * tDestRoom;
	EDoor m_eOrientation;
	ESocketType m_eSocketType;

	TGfxVec3 m_tPos;
	bool bExist;




};