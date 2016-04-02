#pragma once

enum ERoomType{
	EROOMTYPE_STANDARD,
	EROOMTYPE_EMPTY,
	EROOMTYPE_CORRIDOR,
	EROOMTYPE_ENTRANCE,
	EROOMTYPE_HOME,
	EROOMTYPE_EXIT,
	EROOMTYPE_TUTO,
	EROOMTYPE_BOSS
};


enum ERoomActionToUnlock
{
	UNLOCK_ONSTART,
	UNLOCK_ONCLEAN,
	UNLOCK_NEVER,
	UNLOCK_OTHER
};


struct TRoom
{
	ERoomType m_eType;
	TGfxVec3 m_tPos;
	string m_sName;
	TBoundingBox m_tBoundingBox;
	ERoomActionToUnlock m_eActionToUnlock;
	TGfxVec2
		m_tSizeBoundingBox,
		m_tPositionBoundingBox;
	int
		m_iDoorCanBeOpen;

	int id;

	int idInArray;

	TEntrance m_tEntrances[4];

	TGround m_tGround;

	bool m_bRoomCleaned;

	static const int MAX_WALLS = 200;
	class TWall * m_pWalls[MAX_WALLS];
	int m_iWallCount;

	static const int MAX_DOORS = 4;
	class TWall * m_pDoors[MAX_DOORS];
	int m_iDoorCount;

	static const int MAX_TRIGGER = 10;
	class TTrigger * m_pTriggers[MAX_TRIGGER];
	int m_iTriggerCount;

	bool m_bVisible;

	vector<TSpawner> m_tSpawners;

	vector<TEnemy *> m_vpEnemies;

	TGfxVec2 tPlayerSpawn;
	float fAnglePlayerSpawn;

	TGfxVec3 m_tBossSpawn;
	float m_fAngleBossSpawn;

	TRoom() {
		m_bVisible = true;
		m_iTriggerCount = 0;
		m_iWallCount = 0;
		m_tPos = TGfxVec3(0, 0, 0);
		m_tBoundingBox.bl = TGfxVec2(-10, -10);
		m_tBoundingBox.ur = TGfxVec2(10, 10);
		m_iDoorCanBeOpen = -1;
		m_bRoomCleaned = false;
		m_eActionToUnlock = UNLOCK_ONCLEAN;
		idInArray = 0;
	}

	TRoom(const TRoom & other)
	{
		
		m_iWallCount = 0;
		for (int i = 0; i < other.m_iWallCount; i++)
		{
			if (other.m_pWalls[i] != 0)
			{
				m_pWalls[m_iWallCount++] = new TWall(*other.m_pWalls[i]);
			}
		}

		m_iTriggerCount = 0;
		for (int i = 0; i < other.m_iTriggerCount; i++)
		{
			if (other.m_pTriggers[i] != 0)
			{
				m_pTriggers[m_iTriggerCount++] = new TTrigger(*other.m_pTriggers[i]);
			}
		}

		m_tSpawners = other.m_tSpawners;

		m_eType = other.m_eType;
		m_tPos = other.m_tPos;
		m_sName = other.m_sName;
		tPlayerSpawn = other.tPlayerSpawn;
		fAnglePlayerSpawn = other.fAnglePlayerSpawn;

		m_bVisible = other.m_bVisible;

		m_tBossSpawn = other.m_tBossSpawn;
		m_fAngleBossSpawn = other.m_fAngleBossSpawn;

		id = other.id;
		m_tBoundingBox = other.m_tBoundingBox;
		m_tSizeBoundingBox = other.m_tSizeBoundingBox;
		m_tPositionBoundingBox = other.m_tPositionBoundingBox;
		m_iDoorCount = 0;

		m_eActionToUnlock = other.m_eActionToUnlock;

		m_bRoomCleaned = other.m_bRoomCleaned;

		m_tGround = other.m_tGround;
		for (int i = 0; i < 4; ++i)
		{
			//m_pDoors[i] = 0;
		}
		for (int i = 0; i < 4; ++i)
		{
			m_tEntrances[i] = other.m_tEntrances[i];
		}
	};

	~TRoom()
	{
		for (int i = 0; i < m_iWallCount; ++i)
		{
			if (m_pWalls[i] != 0) delete m_pWalls[i];
		}
	};

	void ReinitRoom()
	{	
		m_vpEnemies.clear();
		m_bRoomCleaned = false;
		for (int i = 0; i < 4; i++)
		{
			if (m_tEntrances[i].m_pDoor != 0)
			{
				m_tEntrances[i].m_pDoor->Lock();
			}
		}
	}

	

	void AddWall(TGfxVec2 tPos, TGfxVec2 tRadius, float fAngle, bool bCollideCamera = true, bool bCollidePlayer = true, bool bCollideProjectile = true, unsigned int iColor = EGfxColor_White, bool bIsADoor = false)
	{
		bIsADoor = bIsADoor;
		if (m_iWallCount < MAX_WALLS)
			m_pWalls[m_iWallCount++] = new TWall(tPos, tRadius, fAngle, bCollideCamera, bCollidePlayer, bCollideProjectile, iColor);

	}

	void AddWall(TGfxVec2 tPos, float fRadius, float fAngle, bool bCollideCamera = true, bool bCollidePlayer = true, bool bCollideProjectile = true, unsigned int iColor = EGfxColor_White)
	{
		if (m_iWallCount < MAX_WALLS)
			m_pWalls[m_iWallCount++] = new TWall(tPos, fRadius, fAngle, bCollideCamera, bCollidePlayer, bCollideProjectile, iColor);
	}

	void UnlockDoors()
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_tEntrances[i].m_pDoor != 0)
			{
				m_tEntrances[i].m_pDoor->Unlock();
			}
		}	
	}

	void LockDoors()
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_tEntrances[i].m_pDoor != 0)
			{
				m_tEntrances[i].m_pDoor->Lock();
			}
		}
	}

	void Update()
	{
		if (!m_bRoomCleaned)
		{
			if (m_vpEnemies.size() == 0 && m_eActionToUnlock == UNLOCK_ONCLEAN)
			{				
				m_bRoomCleaned = true;
				//GfxDbgPrintf("Room CLEANED");
				UnlockDoors();
			}
			else if( m_eActionToUnlock == UNLOCK_ONSTART)
			{
				m_bRoomCleaned = true;
				UnlockDoors();
			}
		}	
	}

	void SpreadHideRoom(TRoom * pRoomSrc, int iMarge)
	{
		//GfxDbgPrintf("Spreading\n");
		iMarge -= 1;	
		ShowRoom(iMarge >= 0);
		
		for (int iEntrance = 0; iEntrance < 4; ++iEntrance)
		{
			if (m_tEntrances[iEntrance].bExist && 
				m_tEntrances[iEntrance].tDestRoom != 0 && 
				m_tEntrances[iEntrance].tDestRoom != pRoomSrc)
			{				
				m_tEntrances[iEntrance].tDestRoom->SpreadHideRoom(this, iMarge);
			}
		}
		
	}


	void ShowRoom(bool bShow)
	{		
		//if (bShow) GfxDbgPrintf("ShowRoomtrue  %d \n", idInArray);
		//else GfxDbgPrintf("showRoomfalse %d \n", idInArray);
		
		if (m_bVisible != bShow)
		{
#ifdef TFA_FRESH
			lua_pushinteger(GfxFreshLuaState(), idInArray);
			lua_pushboolean(GfxFreshLuaState(), bShow);
			LuaCallGlobal(GfxFreshLuaState(), "ShowRoom");
#endif

			
			//if (bShow) GfxDbgPrintf("Showing Room %d \n", idInArray);
		//	else GfxDbgPrintf("Hidding Room %d \n", idInArray);
			m_bVisible = bShow;
		}	
	}


	bool NotifyEnemyKill(const TEnemy * pEnemy)
	{
		
		for (int iEnemy = 0; iEnemy < (int)m_vpEnemies.size(); ++iEnemy)
		{
			if (m_vpEnemies[iEnemy] == pEnemy)
			{
				m_vpEnemies.erase( m_vpEnemies.begin() + iEnemy );
				return true;
			}
		}

		return false;	
	
	}

	void Translate(const TGfxVec3 & tPos)
	{
		m_tPos += tPos;

		for (int i = 0; i< m_iWallCount; ++i)
		{
			m_pWalls[i]->Translate(TGfxVec2(tPos.x, tPos.z));
		}

		for (int i = 0; i < (int) m_tSpawners.size(); ++i)
		{
			m_tSpawners[i].m_tPos += tPos;
		}

		for (int i = 0; i < m_iTriggerCount; ++i)
		{
			m_pTriggers[i]->Translate(TGfxVec2(tPos.x, tPos.z));
		}

		m_tBossSpawn += tPos;
		

	}
};

