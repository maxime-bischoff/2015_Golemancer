#include "Const.h"

extern TGame * g_pGame;

TMap::TMap()
:m_iRoomsCount(0),
m_fMinX(9999),
m_fMaxX(-9999),
m_fMinY(9999),
m_fMaxY(-9999)
{
	m_tPlayerSpawn = TGfxVec2(0, 0);
}

TMap::~TMap(){
	for (int i = 0; i < (int)m_pRooms.size(); i++)
	{
		delete m_pRooms[i];
	}
}

void TMap::AddWall(int iRoomIndex, TGfxVec2 tPos, TGfxVec2 tRadius, float fAngle, bool bCollideCamera, bool bCollidePlayer, bool bCollideProjectile, unsigned int iColor, bool bIsADoor){
	if (iRoomIndex >m_iRoomsCount) return;
	m_pRooms[iRoomIndex]->AddWall(tPos, tRadius, fAngle, bCollideCamera, bCollidePlayer, bCollideProjectile, iColor, bIsADoor);
}

void TMap::AddWall(int iRoomIndex, TGfxVec2 tPos, float fRadius, float fAngle, bool bCollideCamera, bool bCollidePlayer, bool bCollideProjectile, unsigned int iColor){
	if (iRoomIndex > m_iRoomsCount) return;
	m_pRooms[iRoomIndex]->AddWall(tPos, fRadius, fAngle, bCollideCamera, bCollidePlayer, bCollideProjectile, iColor);
}

void TMap::NotifyEnemyKill(const TEnemy * pEnemy)
{
	g_pGame->m_pWorld->m_tMainMission->Update();
	for (int iRoom = 0; iRoom < (int)m_pRooms.size(); ++iRoom)
	{
		if (m_pRooms[iRoom]->NotifyEnemyKill(pEnemy)) return;
	}
}
void TMap::PrepareMap()
{
	for (int iRoom = 0; iRoom < (int) m_pRooms.size(); ++iRoom)
	{
		TRoom * pRoom = m_pRooms[iRoom];

		for (int iTrigger = 0; iTrigger < pRoom->m_iTriggerCount; ++iTrigger)
		{
			pRoom->m_pTriggers[iTrigger]->m_bEnabled = true;
		}


	}



}

void TMap::ExportMapToFresh()
{
#ifdef TFA_FRESH
	for (int iRoom = 0; iRoom < (int)m_pRooms.size(); iRoom++)
	{
		lua_pushinteger(GfxFreshLuaState(), m_pRooms[iRoom]->id);
		lua_pushnumber(GfxFreshLuaState(), m_pRooms[iRoom]->m_tPos.x);
		lua_pushnumber(GfxFreshLuaState(), m_pRooms[iRoom]->m_tPos.y);
		lua_pushnumber(GfxFreshLuaState(), m_pRooms[iRoom]->m_tPos.z);
		LuaCallGlobal(GfxFreshLuaState(), "CreateRoom");

		TRoom * pCurrentRoom;

		pCurrentRoom = m_pRooms[iRoom];

		for (int i = 0; i < pCurrentRoom->m_iTriggerCount; i++)
		{
			lua_pushnumber(GfxFreshLuaState(), pCurrentRoom->m_pTriggers[i]->Position().x);
			lua_pushnumber(GfxFreshLuaState(), pCurrentRoom->m_pTriggers[i]->Position().y);
			lua_pushnumber(GfxFreshLuaState(), pCurrentRoom->m_tPos.y);
			LuaCallGlobal(GfxFreshLuaState(), "AddFxTrigger");
		}
	}

	for (int iDoor = 0; iDoor < (int)m_vpDoors.size(); iDoor++)
	{
		TGfxVec3 tDoorCoord = m_vpDoors[iDoor]->m_tPos;
		lua_pushnumber(GfxFreshLuaState(), tDoorCoord.x);
		lua_pushnumber(GfxFreshLuaState(), tDoorCoord.y);
		lua_pushnumber(GfxFreshLuaState(), tDoorCoord.z);
		lua_pushnumber(GfxFreshLuaState(),m_vpDoors[iDoor]->m_fAngle * RadToDeg);

		LuaCallGlobal(GfxFreshLuaState(), "CreateDoor");

		if(m_vpDoors[iDoor]->m_eDoorState == DOOR_SEALED)
		{
			lua_pushinteger(GfxFreshLuaState(), m_vpDoors[iDoor]->m_iId);
			LuaCallGlobal(GfxFreshLuaState(), "SealDoor");
		}
	}

	LuaCallGlobal(GfxFreshLuaState(), "PrepareMap");
#endif
}
void TMap::ReinitMap()
{
	for (int iRoom = 0; iRoom < (int)m_pRooms.size(); ++iRoom)
	{
		m_pRooms[iRoom]->ReinitRoom();
	}

	for (int iDoor = 0; iDoor < m_vpDoors.size(); ++iDoor)
	{
		m_vpDoors[iDoor]->Init();
	}
}

void TMap::Update()
{
	for (int iRoom = 0; iRoom < (int)m_pRooms.size(); ++iRoom)
	{
		if (m_pRooms[iRoom] != 0)
		{
			m_pRooms[iRoom]->Update();

			for (int iTrigger = 0; iTrigger < m_pRooms[iRoom]->m_iTriggerCount; ++iTrigger)
			{
				m_pRooms[iRoom]->m_pTriggers[iTrigger]->Update();
			}
		}
	}

	for (int iDoor = 0; iDoor < (int)m_vpDoors.size(); iDoor++)
	{
		if (m_vpDoors[iDoor] != 0)
		{
			m_vpDoors[iDoor]->Update();
		}
	}
}

bool TMap::CollideOnMap(TTransform * pObject, bool & bContinue, TCollideResult & tResult)
{
	static TTransform * pPreviousObject = 0;
	static int iWall = 0;
	static int index = 0;

	bContinue = true;
	if (pPreviousObject != pObject)
	{
		pPreviousObject = pObject;
		iWall = 0;
		index = 0;
	}

	tResult.self = pObject;
	tResult.fLength = 0;

	if (m_pRooms.size() > 0)
	{
		if (index >= 0)
		{
			if (iWall < m_pRooms[index]->m_iWallCount)
			{
				bool bCollide = false;
				if ((m_pRooms[index]->m_pWalls[iWall]->Position() - pObject->Position()).SquaredLength() < 300)
				{
					tResult.other = m_pRooms[index]->m_pWalls[iWall];
					bCollide = Collide(pObject, m_pRooms[index]->m_pWalls[iWall], tResult);
				}
				if (iWall < 4 && m_pRooms[index]->m_pDoors[iWall] != 0)
				{
					tResult.other = m_pRooms[index]->m_pDoors[iWall];
					bCollide = Collide(pObject, m_pRooms[index]->m_pDoors[iWall], tResult);
				}
				iWall++;

				if (iWall >= m_pRooms[index]->m_iWallCount)
				{
					index++;
					if (index >= (int)m_pRooms.size())
					{
						bContinue = false; pPreviousObject = 0;
					}
					else
					{
						iWall = 0;
					}
				}
				return bCollide;
			}
			else
			{
				bContinue = false;
				pPreviousObject = 0;
			}
		}
		else
		{
			bContinue = false;
			pPreviousObject = 0;
		}
	}
	else
	{
		bContinue = false;
		pPreviousObject = 0;
	}

	return  false;
}

void TMap::CollideOnMap(TDamage * pDamage)
{
	if (!pDamage->m_bCollisionActive) return;
	pDamage->m_bInterruptCollision = false;

	for (int iRoom = 0; iRoom < (int)m_pRooms.size(); ++iRoom)
	{
		for (int iWall = 0; iWall < (int)m_pRooms[iRoom]->m_iWallCount; ++iWall)
		{
			TCollideResult tResult;
			TGfxVec2 tResultsegment;
			TGfxVec2 tPos((pDamage->m_tOrigin + pDamage->m_tDirectionLaser* (pDamage->GetLaserLenght())));
			if (CollideSegmentWall(pDamage->m_tOrigin, tPos, m_pRooms[iRoom]->m_pWalls[iWall], tResultsegment))
			{
				pDamage->SetLaserLenght( 2.0f ) ;
				pDamage->m_tData.m_tRadius.y = pDamage->GetLaserLenght() / 2;
			}
			else
			{
				pDamage->SetLaserLenght( LASER_LENGTH);
				pDamage->m_tData.m_tRadius.y = LASER_LENGTH / 2;
			}
		}
	}
}
void  TMap::CollideOnMap2(TTransform * pObject)
{
	if (!pObject->m_bCollisionActive) return;
	if (pObject->m_pHitbox->eCollisionType == Box) return;// Temporary patch need to investigate further to find where the probleme is ( BUG! )
	pObject->m_bInterruptCollision = false;

	for (int iRoom = 0; iRoom < (int)m_pRooms.size(); ++iRoom)
	{
		for (int iWall = 0; iWall < (int)m_pRooms[iRoom]->m_iWallCount; ++iWall)
		{
			TCollideResult tResult;
			if (Collide(pObject, m_pRooms[iRoom]->m_pWalls[iWall], tResult))
			{
				pObject->CollideWall(tResult);
				if (pObject->m_bInterruptCollision) return;
			}
		}
	}

	for (int iDoor = 0; iDoor < (int)m_vpDoors.size(); iDoor++)
	{
		TCollideResult tResult;
		if (Collide(pObject, m_vpDoors[iDoor]->m_pWallLeft, tResult))
		{
			pObject->CollideWall(tResult);
			if (pObject->m_bInterruptCollision) return;
		}

		if (Collide(pObject, m_vpDoors[iDoor]->m_pWallRight, tResult))
		{
			pObject->CollideWall(tResult);
			if (pObject->m_bInterruptCollision) return;
		}
	}
}

void TMap::DrawMap(TGfxSprite * pLineSprite)
{
#ifndef TFA_FRESH
	//TMapGenerator * mapgn = g_pGame->m_pWorld->m_pMapGenerator;
	for (int i = 0; i < (int)m_pRooms.size(); i++)
	{
		DrawCircle(pLineSprite, TGfxVec2(m_pRooms[i]->m_tPos.x, m_pRooms[i]->m_tPos.z), 0.3f, GfxColor(255, 255, 0, 255));

		for (int j = 0; j < m_pRooms[i]->m_iWallCount; j++)
		{
			m_pRooms[i]->m_pWalls[j]->Draw(pLineSprite);
		}
		//	if ( i < 3 )
		DrawBox(pLineSprite, m_pRooms[i]->m_tPositionBoundingBox + TGfxVec2(m_pRooms[i]->m_tPos.x, m_pRooms[i]->m_tPos.z), 0, m_pRooms[i]->m_tSizeBoundingBox, GfxColor(175, 125, 0, 255));

		for (int j = 0; j < m_pRooms[i]->m_iTriggerCount; ++j)
		{
			m_pRooms[i]->m_pTriggers[j]->Draw(pLineSprite);
		}

		for (int j = 0; j < 4; ++j)
		{
			if (m_pRooms[i]->m_tEntrances[j].bExist){
				unsigned int iColor = 0;
				if (m_pRooms[i]->m_tEntrances[j].tDestRoom != 0)
				{
					iColor = GfxColor(255, 255, 0, 255);
				}
				else{
					iColor = iColor = GfxColor(50, 50, 50, 255);
				}
				EDoor eDoor = (EDoor)j;

				switch (eDoor)
				{
				case EDOOR_LEFT: DrawPartialCircle(pLineSprite, TGfxVec2(m_pRooms[i]->m_tPos.x, m_pRooms[i]->m_tPos.z) + TGfxVec2(m_pRooms[i]->m_tEntrances[j].m_tPos.x, m_pRooms[i]->m_tEntrances[j].m_tPos.z), 0, 1.0f, PI, iColor, 10);
					break;
				case EDOOR_RIGHT:DrawPartialCircle(pLineSprite, TGfxVec2(m_pRooms[i]->m_tPos.x, m_pRooms[i]->m_tPos.z) + TGfxVec2(m_pRooms[i]->m_tEntrances[j].m_tPos.x, m_pRooms[i]->m_tEntrances[j].m_tPos.z), PI, 1.0f, PI, iColor, 10);
					break;
				case EDOOR_TOP:DrawPartialCircle(pLineSprite, TGfxVec2(m_pRooms[i]->m_tPos.x, m_pRooms[i]->m_tPos.z) + TGfxVec2(m_pRooms[i]->m_tEntrances[j].m_tPos.x, m_pRooms[i]->m_tEntrances[j].m_tPos.z), 3 * PI_2, 1.0f, PI, iColor, 10);
					break;
				case EDOOR_BOTTOM:DrawPartialCircle(pLineSprite, TGfxVec2(m_pRooms[i]->m_tPos.x, m_pRooms[i]->m_tPos.z) + TGfxVec2(m_pRooms[i]->m_tEntrances[j].m_tPos.x, m_pRooms[i]->m_tEntrances[j].m_tPos.z), PI_2, 1.0f, PI, iColor, 10);
					break;
				default:
					break;
				}
			}
		}
	}

	const int MAX_COLOR = 10;
	unsigned int iRandomColor[MAX_COLOR];

	for (int i = 0; i < MAX_COLOR; i++)
	{
		float  fValue1 = static_cast <float> (GfxMathGetRandomInteger(20, 60));
		float  fValue2 = static_cast <float> (GfxMathGetRandomInteger(20, 60));
		float  fValue3 = static_cast <float> (GfxMathGetRandomInteger(20, 60));

		iRandomColor[i] = GfxColor(fValue1, fValue2, fValue3, 255);
	}

#endif
}

void TMap::DrawMapOnMiniMap(TGfxSprite * pLineSprite)
{
#ifndef TFA_FRESH
	for (int i = 0; i < m_iRoomsCount; i++)
	{
		for (int j = 0; j < m_pRooms[i]->m_iWallCount; j++)
		{
			DrawBox(pLineSprite,
				m_pRooms[i]->m_pWalls[j]->Position(),
				m_pRooms[i]->m_pWalls[j]->Rotation(),
				m_pRooms[i]->m_pWalls[j]->m_tRadius,
				m_pRooms[i]->m_pWalls[j]->m_iColor);
		}
	}

	// ****** Dessin de la zone total de la minimap ******* //
	/*
	GfxLineSpriteJumpTo(pLineSprite, 0, 0);
	GfxLineSpriteLineTo(pLineSprite, 0, -MAP_MAX_H );
	GfxLineSpriteLineTo(pLineSprite, MAP_MAX_W , -MAP_MAX_H );
	GfxLineSpriteLineTo(pLineSprite, MAP_MAX_W , 0 );
	GfxLineSpriteLineTo(pLineSprite, 0 , 0 );
	*/

	// ****** Dessin de la zone effective de la minimap ******* //
	/*
	GfxLineSpriteSetDrawingColor(pLineSprite, EGfxColor_Red);
	GfxLineSpriteJumpTo(pLineSprite, m_fMinX, m_fMinY);
	GfxLineSpriteLineTo(pLineSprite, m_fMinX, m_fMaxY);
	GfxLineSpriteLineTo(pLineSprite, m_fMaxX, m_fMaxY);
	GfxLineSpriteLineTo(pLineSprite, m_fMaxX, m_fMinY);
	GfxLineSpriteLineTo(pLineSprite, m_fMinX, m_fMinY);
	*/

#endif
}

void TMap::Clean()
{
	for (int i = 0; i < (int)m_pRooms.size(); i++)
	{
		delete m_pRooms[i];
	}
	m_pRooms.clear();
}

void TMap::PlaceDoors(ERoomType /*eGameType*/)
{
	for (int iRoom = 0; iRoom < (int)m_pRooms.size(); iRoom++)
	{
		m_pRooms[iRoom]->idInArray = iRoom;

		for (int iEntrance = 0; iEntrance < 4; iEntrance++)
		{
			TEntrance & tEntrance = m_pRooms[iRoom]->m_tEntrances[iEntrance];
			if (tEntrance.bExist)
			{
				if (tEntrance.m_pDoor == 0)
				{
					tEntrance.m_pDoor = new TDoor(m_pRooms[iRoom]->m_tPos + tEntrance.m_tPos, tEntrance.m_eOrientation);

					m_vpDoors.push_back(tEntrance.m_pDoor);
					tEntrance.m_pDoor->m_iId = m_vpDoors.size() - 1;
					if (tEntrance.tDestRoom != 0)
					{
						tEntrance.tDestRoom->m_tEntrances[TMapGenerator::GetOppositeDoor(tEntrance.m_eOrientation)].m_pDoor = tEntrance.m_pDoor;
						switch (tEntrance.tDestRoom->m_eActionToUnlock)
						{
						case UNLOCK_ONCLEAN:tEntrance.m_pDoor->m_eDoorState = DOOR_LOCKED;
							break;
						case UNLOCK_ONSTART: tEntrance.m_pDoor->m_eDoorState = DOOR_CLOSED;
							break;
						default: tEntrance.m_pDoor->m_eDoorState = DOOR_CLOSED;
							break;
						}
					}
					else
					{
						tEntrance.m_pDoor->Seal();
					}
				}
			}
		}
	}
}

int TMap::GetRoom(const TGfxVec2 & tPos)
{
	for (int i = 0; i < (int)m_pRooms.size(); ++i)
	{
		if (IsInBoundingBox(tPos - TGfxVec2(m_pRooms[i]->m_tPos.x, m_pRooms[i]->m_tPos.z), m_pRooms[i]->m_tBoundingBox))
		{
			return i;
		}
	}
	return -1;
}

float TMap::GetHeight(const TGfxVec2 & tPos, float fCurrentHeight)
{
	int iCurrentRoom = GetRoom(tPos);

	if (iCurrentRoom >= 0)
	{
		TRoom * tCurrentRoom = m_pRooms[iCurrentRoom];
		TGfxVec2 NormalizePos = (tPos - TGfxVec2(tCurrentRoom->m_tPos.x, tCurrentRoom->m_tPos.z));
		float fHeight = tCurrentRoom->m_tGround.GetHeight(NormalizePos.x, -NormalizePos.y) + tCurrentRoom->m_tPos.y;
		return fHeight;
	}

	return fCurrentHeight;
}