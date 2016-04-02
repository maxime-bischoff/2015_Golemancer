#pragma once

#include "Spellbook.h"
#include <vector>

enum EPlayer_Location
{
	LOCATION_HOME,
	LOCATION_MAP,
	LOCATION_TUTO,
};

class TWorld
{
	// :: PUBLIC
public:
	TWorld();
	~TWorld();

	// VARIABLES
	// -- CONST

	static const int
		MAX_DAMAGE = 130,
		MAX_ENEMY = 80,
		MAX_ENVIRONMENT = 20,
		MAX_DESTRUCTIBLE = 30,
		MAX_COLLECTIBLE = 30,
		MAX_MAPS = 20;

	// --
	class TSpellBook
		m_tSpellBook;
	class TParticle_Manager
		* m_pParticle;
	class THome
		* m_pHome;
	class TTuto
		* m_pTuto;
	class TMapGenerator
		* m_pMapGenerator;
	class THud
		* m_pHud;
	class TMission
		* m_tMainMission;
	class TPlayer
		* m_pPlayer;
	class  TMap
		* m_pMaps[MAX_MAPS];
	class TEnemy
		* m_pEnemyList[MAX_ENEMY];
	class TDamage
		* m_pDamages[MAX_DAMAGE];
	class TCollectible
		* m_pCollectibleList[MAX_DESTRUCTIBLE];

	EPlayer_Location
		m_ePlayerLocation;
	TGfxVec2
		m_tMapPos;

	float
		m_fMapScale,
		m_fPostDeathDelay,
		m_fPostDeathCountDown;

	int
		m_tDamageDestroyed[MAX_DAMAGE],
		m_iCurrentMap,
		m_iMapCount,

		m_iCountDamage,
		m_iEnemyCount,
		m_iEnvironmentCount,
		m_iDestrucCount,
		m_iCollectibleCount,

		m_iDestroyCount,
		m_iGameSeed;

#ifndef TFA_FRESH
	TGfxSprite
		* m_pLineSprite,
		* m_pMapLineSprite,
		* m_pGridLineSprite,
		* m_pHudLineSprite,
		* m_pRoomLineSprite,
		* m_pParticleSprite,

		* m_pHudMap,
		* m_pDynamicHudMap,
		* m_pTextMission;
#endif

	// FUNCTIONS

	void
		Initialize(),
		Update(),
		Render();

	TDamage
		* AddDamage(struct TDamageData &tDataDmg, enum ETypeDamage eType, const TGfxVec2 & tPos, const float fAngle, enum  EDamageFaction eFaction, unsigned int iColor = EGfxColor_Red, float  fDiffAngle = 0, class TEnemy * tTarget = 0 );
	void
		DestroyDamage(int iIndex),
		DestroyDamage(TDamage * pDamage),

		AddEnemy(TGfxVec2 tPos, float fAngle, enum EEnemyType eType, struct TRoom * pRoom),
		DestroyEnemy(int iIndex),

		AddCollectible(TGfxVec2 tPos, float fHeight = 0),
		DestroyCollectible(int iIndex),

		ClearWorld(),
		DrawMap(TGfxSprite * pLineSprite),
		WarpHome(),
		WarpMap(int id),
		WarpTuto(),
		ProcessDeath();

	int
		GetEnemyID(const TEnemy * pEnemy);

	class
		TEnemy * GetClosestEnemy(TGfxVec2 tPos, float fMaxRange);

	std::vector<TEnemy *>
		GetAllClosestEnemy ( float fMaxRange ) ;

	TMap
		* GetCurrentMap();
	int
		GetNumberEnemyInRoom(int iIDRoom);
	bool
		RoomIsCleaned(int iIDRoom);

	// :: PRIVATE
private:

	void
		DetectCollision(),
		UpdateRoomsVisibility(),
		SpawnEnemies(int iDifficulty),
		SpawnEnemiesTutorial(TMap * pTuto),
		GarbageCleaner();

	int
		m_iPlayerCurrentRoom,
		m_iIdTuto;
};
