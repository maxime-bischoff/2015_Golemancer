#include "Const.h"

#include <cstring>
#include <vector>

using std::string;
using std::vector;

float g_fScaleScene = 45;
bool g_bActionEnvi = false;
TGfxSprite * g_Text;
char * text[30];

extern TGame * g_pGame;

TWorld::TWorld()
{
}

TWorld::~TWorld()
{
}

void TWorld::Initialize()
{
	TTime::SetTimeScale(1.0f);

	for (int i = 0; i < MAX_MAPS; i++)
	{
		m_pMaps[i] = 0;
	}

	m_iCurrentMap = 0;
	m_iMapCount = 0;

	m_ePlayerLocation = LOCATION_HOME;

	m_iCountDamage = 0;
	m_iEnemyCount = 0;
	m_iEnvironmentCount = 0;
	m_iDestrucCount = 0;
	m_iCollectibleCount = 0;

	m_iDestroyCount = 0;
	m_pHome = new THome();

	m_pMapGenerator = new TMapGenerator();

	m_pPlayer = new TPlayer();
	m_pHud = new THud();

	m_iPlayerCurrentRoom = -2;

	m_fPostDeathDelay = 3.0f;
	m_fPostDeathCountDown = 0;

	m_iGameSeed = 14;
	GfxMathResetRandomSeed(m_iGameSeed);
#ifndef TFA_FRESH
	m_pLineSprite = GfxLineSpriteCreate();
	m_pMapLineSprite = GfxLineSpriteCreate();
	m_pGridLineSprite = GfxLineSpriteCreate();
	m_pRoomLineSprite = GfxLineSpriteCreate();
	m_pHudMap = GfxLineSpriteCreate();
	m_pDynamicHudMap = GfxLineSpriteCreate();
	m_pParticleSprite = GfxLineSpriteCreate();
	m_pTextMission = GfxTextSpriteCreate();
#endif

#ifndef TFA_FRESH
	m_pParticle = new TParticle_Manager(m_pParticleSprite);
#endif

	m_fMapScale = 2.0f;

#ifndef TFA_FRESH

	GfxSpriteSetScale(m_pLineSprite, g_fScaleScene, -g_fScaleScene);
	GfxSpriteSetScale(m_pMapLineSprite, g_fScaleScene, -g_fScaleScene);
	GfxSpriteSetScale(m_pGridLineSprite, g_fScaleScene, -g_fScaleScene);
	GfxSpriteSetScale(m_pRoomLineSprite, g_fScaleScene, -g_fScaleScene);
	GfxSpriteSetScale(m_pParticleSprite, g_fScaleScene, -g_fScaleScene);

	GfxSpriteSetScale(m_pHudMap, m_fMapScale, -m_fMapScale);
	GfxSpriteSetPosition(m_pHudMap, m_tMapPos.x, m_tMapPos.y);
	GfxSpriteSetScale(m_pDynamicHudMap, m_fMapScale, -m_fMapScale);
	GfxSpriteSetPosition(m_pDynamicHudMap, m_tMapPos.x, m_tMapPos.y);

	g_Text = GfxTextSpriteCreate();

#endif

#ifdef TFA_FRESH
//	lua_pushboolean(GfxFreshLuaState(), false);
//	LuaCallGlobal(GfxFreshLuaState(), "ShowFocus");
#endif

	m_tMainMission = new TMission();
	for (int i = 0; i < MAX_DAMAGE; i++)
	{
		m_pDamages[i] = 0;
	}

	m_pMapGenerator->ImportRoomPatterns();
	m_pMapGenerator->GenerateHome(m_pHome);

	//43 -200
	TMap * pMap = m_pMapGenerator->GenerateTuto();
	m_pTuto = new TTuto(pMap);
	if (pMap != 0){ m_iIdTuto = m_iMapCount; m_pMaps[m_iMapCount++] = pMap; }
	else{ GfxDbgPrintf("Error while generating map"); }

	pMap = m_pMapGenerator->GenerateMap(43, 1);

	if (pMap != 0){ m_pMaps[m_iMapCount++] = pMap; }
	else{ GfxDbgPrintf("Error while generating map"); }

	pMap = m_pMapGenerator->GenerateBoss();

	if (pMap != 0){ m_pMaps[m_iMapCount++] = pMap; }
	else{ GfxDbgPrintf("Error while generating map"); }
#ifndef TFA_FRESH
	DrawGrid(m_pGridLineSprite, 10, 1, 5);

#else
	m_pHud->Initialize();
#endif
}

void TWorld::Update()
{
	UpdateRoomsVisibility();
	static bool bInitialize = false;

	if (GfxInputIsJustPressed(EGfxInputID_KeyCharW))
	{
		WarpMap(0);
	}
	if (m_fPostDeathCountDown > 0)
	{
		m_fPostDeathCountDown -= DeltaTime();
		if (m_fPostDeathCountDown <= 0)
		{
			m_fPostDeathCountDown = 0;
			g_pGame->GameOver();
		}
	}

#ifndef TFA_FRESH
	GfxLineSpriteReset(m_pLineSprite);
	GfxLineSpriteReset(m_pHud->m_pHudOverlay);
#endif

	if ((GfxInputIsJustPressed(EGfxInputID_KeyReturn) || GfxInputIsJustPressed(EGfxInputID_AnyPadStart))
		&& g_pGame->m_eGameState != GAMESTATE_GAMEOVER && m_pPlayer->m_ePlayerState != PLAYER_STATE_TELEPORTING)
	{
		g_pGame->PauseGame();
	}

	if (TFade::Instance().GetState() == FADE_CLEAR)
	{
		m_pPlayer->Update();

		m_tSpellBook.Update();
	}

	GetCurrentMap()->Update();
	for (int i = 0; i < m_iEnemyCount; i++)
	{
		m_pEnemyList[i]->Update();
#ifdef TFA_FRESH
		if (m_pEnemyList[i]->m_eEnemyType != EEnemyType_Boss){
			if (!m_pEnemyList[i]->m_bIsDead)
			{
				LuaUpdateEnemy(i, m_pEnemyList[i]->Position(), m_pEnemyList[i]->Rotation(), m_pEnemyList[i]->GetState(), m_pEnemyList[i]->m_tColor, m_pEnemyList[i]->m_fHeight);
			}
			else
			{
				lua_pushinteger(GfxFreshLuaState(), i);
				lua_pushnumber(GfxFreshLuaState(), m_pEnemyList[i]->m_fDisintegrationPower);
				LuaCallGlobal(GfxFreshLuaState(), "UpdateEnemyDead");
			}

			lua_pushinteger(GfxFreshLuaState(), i);
			lua_pushnumber(GfxFreshLuaState(), m_pEnemyList[i]->m_fFresnelStrength);
			lua_pushnumber(GfxFreshLuaState(), m_pEnemyList[i]->m_fFresnelWidth);
			lua_pushnumber(GfxFreshLuaState(), m_pEnemyList[i]->m_tColor.x);
			lua_pushnumber(GfxFreshLuaState(), m_pEnemyList[i]->m_tColor.y);
			lua_pushnumber(GfxFreshLuaState(), m_pEnemyList[i]->m_tColor.z);
			LuaCallGlobal(GfxFreshLuaState(), "UpdateEnemyColor");
		}

#endif
	}

	DetectCollision();

#ifdef TFA_FRESH
	LuaUpdatePlayer(m_pPlayer->Position(), m_pPlayer->Rotation(), m_pPlayer->m_fHeight, (int)m_pPlayer->m_eMovementAnimState);

	if (m_pPlayer->m_pTarget != 0)
		LuaUpdateTarget(m_pPlayer->m_pTarget->Position(), true);
	else
		LuaUpdateTarget(TGfxVec2(0, 0), false);
#endif

	m_pPlayer->Render();

	for (int i = 0; i < m_iEnemyCount; i++)
	{
		m_pEnemyList[i]->Draw();
	}

	if (GfxInputIsJustPressed(EGfxInputID_KeyCharE))
	{
		TGfxVec2 tTemp = m_pPlayer->Position();
		tTemp.x += 2.f;
	}

	// :: Update Damage :: //
	for (int i = 0; i < MAX_DAMAGE; i++)
	{
		if (m_pDamages[i] != 0)
		{
			m_pDamages[i]->Update();
		}
	}

	// :: Render Damage :: //
	for (int i = 0; i < m_iCountDamage; i++)
	{
		if (m_pDamages[i] != 0)
		{
			m_pDamages[i]->Render();
#ifdef TFA_FRESH
			if (m_pDamages[i]->m_tData.m_fDelay <= 0)
				LuaUpdateDamage(i, m_pDamages[i]->Position(), m_pDamages[i]->m_tData.m_tRadius, -m_pDamages[i]->Rotation(), true, m_pDamages[i]->m_tData.m_iStyle);
			else
				LuaUpdateDamage(i, m_pDamages[i]->Position(), m_pDamages[i]->m_tData.m_tRadius, -m_pDamages[i]->Rotation(), false, m_pDamages[i]->m_tData.m_iStyle);
#endif
		}
	}

	// :: Time Over Damage :: //
	for (int i = m_iCountDamage; i > 0; i--)
	{
		if (m_pDamages[i - 1] != 0)
		{
			if (m_pDamages[i - 1]->m_tData.m_fLifeSpan < 0.0f)
			{
				DestroyDamage(i - 1);
			}
		}
	}

	// :: Render :: //

	if (m_pPlayer->m_tCam.m_eMode == CAMERA_MODE_TOPDOWN)
	{
#ifndef TFA_FRESH
		GfxSpriteSetAngle(m_pLineSprite, 0);
		GfxSpriteSetPivot(m_pLineSprite, 0, 0);
		GfxSpriteSetAngle(m_pMapLineSprite, 0);
		GfxSpriteSetPivot(m_pMapLineSprite, 0, 0);
		GfxSpriteSetAngle(m_pRoomLineSprite, 0);
		GfxSpriteSetPivot(m_pRoomLineSprite, 0, 0);
		GfxSpriteSetAngle(m_pParticleSprite, 0);
		GfxSpriteSetPivot(m_pParticleSprite, 0, 0);
		GfxSpriteSetAngle(m_pGridLineSprite, 0);
		GfxSpriteSetPivot(m_pGridLineSprite, 0, 0);

		TGfxVec2 tSpritePosition = TGfxVec2(GfxGetDisplaySizeX() / 2.0f - m_pPlayer->m_tCam.Position().x * g_fScaleScene,
			GfxGetDisplaySizeY() / 2.0f + m_pPlayer->m_tCam.Position().y * g_fScaleScene);

		tSpritePosition.x += (-m_tSpellBook.m_fClosedPanelPos + m_tSpellBook.m_fCurrentPanelPos) / 2.0f;

		GfxSpriteSetPosition(m_pLineSprite, tSpritePosition.x, tSpritePosition.y);
		GfxSpriteSetPosition(m_pMapLineSprite, tSpritePosition.x, tSpritePosition.y);
		GfxSpriteSetPosition(m_pGridLineSprite, tSpritePosition.x, tSpritePosition.y);
		GfxSpriteSetPosition(m_pRoomLineSprite, tSpritePosition.x, tSpritePosition.y);
		GfxSpriteSetPosition(m_pParticleSprite, tSpritePosition.x, tSpritePosition.y);
#endif
	}
	else if (m_pPlayer->m_tCam.m_eMode == CAMERA_MODE_TPS){
#ifdef TFA_FRESH
		LuaUpdateCamera();
#endif

#ifndef TFA_FRESH
		TGfxVec2 tCamPivot;
		if (m_pPlayer->m_pTarget == 0)
			tCamPivot = TGfxVec2(m_pPlayer->Position().x, m_pPlayer->Position().y);
		else
			tCamPivot = m_pPlayer->Position() + (m_pPlayer->m_pTarget->Position() - m_pPlayer->Position()) / 2.0f;

		TGfxVec2 fCamDir = m_pPlayer->m_tCam.m_tLookat - m_pPlayer->m_tCam.Position();

		float fCamRotation = atan2f(-fCamDir.y, fCamDir.x) + PI_2;

		GfxSpriteSetAngle(m_pMapLineSprite, 0);
		GfxSpriteSetPivot(m_pMapLineSprite, 0, 0);
		GfxSpriteSetPivot(m_pMapLineSprite, tCamPivot.x, tCamPivot.y);
		GfxSpriteSetAngle(m_pMapLineSprite, fCamRotation);

		GfxSpriteSetAngle(m_pLineSprite, 0);
		GfxSpriteSetPivot(m_pLineSprite, 0, 0);
		GfxSpriteSetPivot(m_pLineSprite, tCamPivot.x, tCamPivot.y);
		GfxSpriteSetAngle(m_pLineSprite, fCamRotation);

		GfxSpriteSetAngle(m_pRoomLineSprite, 0);
		GfxSpriteSetPivot(m_pRoomLineSprite, 0, 0);
		GfxSpriteSetPivot(m_pRoomLineSprite, tCamPivot.x, tCamPivot.y);
		GfxSpriteSetAngle(m_pRoomLineSprite, fCamRotation);

		GfxSpriteSetAngle(m_pParticleSprite, 0);
		GfxSpriteSetPivot(m_pParticleSprite, 0, 0);
		GfxSpriteSetPivot(m_pParticleSprite, tCamPivot.x, tCamPivot.y);
		GfxSpriteSetAngle(m_pParticleSprite, fCamRotation);

		GfxSpriteSetAngle(m_pGridLineSprite, 0);
		GfxSpriteSetPivot(m_pGridLineSprite, 0, 0);
		GfxSpriteSetPivot(m_pGridLineSprite, tCamPivot.x, tCamPivot.y);
		GfxSpriteSetAngle(m_pGridLineSprite, fCamRotation);

		TGfxVec2 tCamPos;

		TGfxVec2 tSpritePosition = TGfxVec2(GfxGetDisplaySizeX() / 2.0f - tCamPivot.x * g_fScaleScene,
			GfxGetDisplaySizeY() / 2.0f + tCamPivot.y * g_fScaleScene);

		tSpritePosition.x += (-m_tSpellBook.m_fClosedPanelPos + m_tSpellBook.m_fCurrentPanelPos) / 2.0f;

		tCamPos = TGfxVec2(tSpritePosition.x + tCamPivot.x * g_fScaleScene,
			tSpritePosition.y - tCamPivot.y * g_fScaleScene);

		GfxSpriteSetPosition(m_pMapLineSprite, tCamPos.x, tCamPos.y);
		GfxSpriteSetPosition(m_pRoomLineSprite, tCamPos.x, tCamPos.y);
		GfxSpriteSetPosition(m_pLineSprite, tCamPos.x, tCamPos.y);
		GfxSpriteSetPosition(m_pGridLineSprite, tCamPos.x, tCamPos.y);
		GfxSpriteSetPosition(m_pParticleSprite, tCamPos.x, tCamPos.y);

#endif
	}
#ifndef TFA_FRESH
	GfxLineSpriteReset(m_pDynamicHudMap);
	DrawPartialCircle(m_pDynamicHudMap, m_pPlayer->Position(), m_pPlayer->Rotation(), 3.0f, PI / 2.0f, GfxColor(120, 120, 120, 255), 20);

	DrawCircle(m_pDynamicHudMap, m_pPlayer->Position(), 0.5f, EGfxColor_Red, 5);
#endif
#ifndef TFA_FRESH

	m_pParticle->Update();
	m_pParticle->Draw();

#endif
	for (int i = m_iCollectibleCount; i > 0; i--)
	{
		if (m_pCollectibleList[i - 1]->m_fLifeSpan > 0.f){
			m_pCollectibleList[i - 1]->Update();
#ifdef TFA_FRESH
			LuaUpdateCollectible(i - 1, m_pCollectibleList[i - 1]->Position(), m_pCollectibleList[i - 1]->m_fHeight);
#endif
			m_pCollectibleList[i - 1]->Draw();
		}
		else{
			DestroyCollectible(i - 1);
		}
	}
	m_pHud->Update();

#ifdef TFA_FRESH //---!!!!!!!! MOVE TO PLAYER UPDATE!!!!!!!---!//
	if (m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState != FOCUS_STATE_NOTTAKEN)
	{
		if (m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState == FOCUS_STATE_AWAITING)
			LuaUpdateFocus(m_pPlayer->m_pSpellManager->m_pFocus->Position(), m_pPlayer->m_pSpellManager->m_pFocus->Rotation(), FOCUS_ANIM_IDLE);
		else
			LuaUpdateFocus(m_pPlayer->m_pSpellManager->m_pFocus->Position(), m_pPlayer->m_pSpellManager->m_pFocus->Rotation(), FOCUS_ANIM_ATTACK);
	}

	LuaCallGlobal(GfxFreshLuaState(), "UpdateFX");
#endif
	if (m_ePlayerLocation == LOCATION_TUTO)
	{
		m_pTuto->Update();
	}

	// Door drawing

	TMap * pMap = GetCurrentMap();

	for (int iDoor = 0; iDoor < (int)pMap->m_vpDoors.size(); iDoor++)
	{
#ifndef TFA_FRESH
		pMap->m_vpDoors[iDoor]->Draw(m_pLineSprite);
#endif
	}

	GarbageCleaner();
}

void TWorld::UpdateRoomsVisibility()
{
	TMap * pMap = GetCurrentMap();
	int m_iPlayerRoom = pMap->GetRoom(m_pPlayer->Position());
	if (m_iPlayerRoom != m_iPlayerCurrentRoom && m_iPlayerRoom != -1)
	{
		m_iPlayerCurrentRoom = m_iPlayerRoom;
		pMap->m_pRooms[m_iPlayerRoom]->SpreadHideRoom(0, 2);
	}
}

void TWorld::ClearWorld()
{
	m_iPlayerCurrentRoom = -2;
	for (int i = m_iEnemyCount; i >= 0; i--)
	{
		if (m_pEnemyList[i] != 0)
		{
			DestroyEnemy(i);
		}
	}
#ifdef TFA_FRESH
	LuaCallGlobal(GfxFreshLuaState(), "CleanEnemy");
#endif
	m_iEnemyCount = 0;

	for (int i = m_iCountDamage; i >= 0; i--)
	{
		DestroyDamage(i);
	}
	m_iCountDamage = 0;

	for (int i = m_iCollectibleCount; i >= 0; i--)
	{
		if (m_pCollectibleList[i] != 0)
		{
			DestroyCollectible(i);
		}
	}
	m_iCollectibleCount = 0;
	for (int i = 0; i < m_pMaps[m_iCurrentMap]->m_iRoomsCount; ++i)
	{
		m_pMaps[m_iCurrentMap]->m_pRooms[i]->LockDoors();
	}

	TMap * pMap = GetCurrentMap();
	for (int iRoom = 0; iRoom < (int)pMap->m_pRooms.size(); ++iRoom)
	{
		pMap->m_pRooms[iRoom]->m_bVisible = true;
	}
}

void TWorld::WarpHome()
{
	ClearWorld();
	m_pPlayer->m_pTarget = 0;
	m_pPlayer->m_ePlayerMode = PLAYER_MODE_WALK;
	if (m_pPlayer->IsDead()) m_pPlayer->Respawn();

# ifndef TFA_FRESH
	GfxLineSpriteReset(m_pRoomLineSprite);
	GfxLineSpriteReset(m_pMapLineSprite);
	GfxLineSpriteReset(m_pHudMap);
	m_pHome->DrawMap(m_pMapLineSprite);
	m_pHome->DrawMapOnMiniMap(m_pHudMap);

	GfxSpriteSetPosition(m_pHudMap, m_tMapPos.x, m_tMapPos.y);
	GfxSpriteSetPosition(m_pDynamicHudMap, m_tMapPos.x, m_tMapPos.y);

#endif

#ifdef TFA_FRESH
	LuaCallGlobal(GfxFreshLuaState(), "CleanScene");
	LuaCallGlobal(GfxFreshLuaState(), "StopTuto");
	m_pHome->ExportMapToFresh();
#endif

	m_ePlayerLocation = LOCATION_HOME;
	m_tMainMission->SetMission(TMission::EMissionType_None);

	m_pPlayer->Teleport(m_pHome->m_pRooms[0]->tPlayerSpawn, m_pHome->m_pRooms[0]->fAnglePlayerSpawn);
	GetCurrentMap()->PrepareMap();
}

void TWorld::WarpMap(int id)
{
	if (m_pPlayer->IsDead()) m_pPlayer->Respawn();
	ClearWorld();
	if (id <= m_iMapCount)
	{
		if (m_pMaps[id] == 0){
			GfxDbgPrintf("The map doesn't exist");
		}

		m_pPlayer->m_pTarget = 0;

		m_pPlayer->m_ePlayerMode = PLAYER_MODE_WALK;
		if (id == m_iIdTuto)
		{
			m_ePlayerLocation = LOCATION_TUTO;
#ifdef TFA_FRESH
			LuaCallGlobal(GfxFreshLuaState(), "StartTuto");
#endif
		}
		else
		{
			m_ePlayerLocation = LOCATION_MAP;
			m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState = FOCUS_STATE_AWAITING;
		}
		m_iCurrentMap = id;


		
#ifdef TFA_FRESH
	/*	if(m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState == FOCUS_STATE_NOTTAKEN)
		lua_pushboolean(GfxFreshLuaState(), false);
		else lua_pushboolean(GfxFreshLuaState(), true);
		LuaCallGlobal(GfxFreshLuaState(), "ShowFocus");*/
#endif


#ifndef TFA_FRESH
		GfxLineSpriteReset(m_pRoomLineSprite);
		GfxLineSpriteReset(m_pMapLineSprite);
		GfxLineSpriteReset(m_pHudMap);
		m_pMaps[id]->DrawMap(m_pMapLineSprite);
		m_pMaps[id]->DrawMapOnMiniMap(m_pHudMap);

		GfxSpriteSetPosition(m_pHudMap, m_tMapPos.x, m_tMapPos.y);
		GfxSpriteSetPosition(m_pDynamicHudMap, m_tMapPos.x, m_tMapPos.y);

#else
		LuaCallGlobal(GfxFreshLuaState(), "CleanScene");
		GetCurrentMap()->ExportMapToFresh();

#endif
		GetCurrentMap()->ReinitMap();
		SpawnEnemies(1);
	}
	if (GfxMathGetRandomInteger(0, 1) == 0)
	{
		m_tMainMission->SetMission(TMission::EMissionType_Assassination);
	}
	else
	{
		m_tMainMission->SetMission(TMission::EMissionType_Extermination);
	}

	m_pPlayer->Teleport(m_pMaps[id]->m_pRooms[0]->tPlayerSpawn, m_pMaps[id]->m_pRooms[0]->fAnglePlayerSpawn);
	GetCurrentMap()->PrepareMap();
}

void TWorld::WarpTuto()
{
	if (m_pPlayer->IsDead()) m_pPlayer->Respawn();
	ClearWorld();
	m_pPlayer->m_pTarget = 0;
	m_pPlayer->m_ePlayerMode = PLAYER_MODE_WALK;

	m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState = FOCUS_STATE_NOTTAKEN;

# ifndef TFA_FRESH
	GfxLineSpriteReset(m_pMapLineSprite);
	GfxLineSpriteReset(m_pHudMap);
	m_pMaps[m_iIdTuto]->DrawMap(m_pMapLineSprite);
	m_pMaps[m_iIdTuto]->DrawMapOnMiniMap(m_pHudMap);

	GfxSpriteSetPosition(m_pHudMap, m_tMapPos.x, m_tMapPos.y);
	GfxSpriteSetPosition(m_pDynamicHudMap, m_tMapPos.x, m_tMapPos.y);
#endif

#ifdef TFA_FRESH
	LuaCallGlobal(GfxFreshLuaState(), "CleanScene");
	m_pMaps[m_iIdTuto]->ExportMapToFresh();

#endif

#ifdef TFA_FRESH
	
		//lua_pushboolean(GfxFreshLuaState(), true);	
		//LuaCallGlobal(GfxFreshLuaState(), "ShowFocus");
#endif

	SpawnEnemiesTutorial(m_pMaps[m_iIdTuto]);
	m_ePlayerLocation = LOCATION_TUTO;
	m_tMainMission->SetMission(TMission::EMissionType_None);
	m_pPlayer->Teleport(m_pMaps[m_iIdTuto]->m_pRooms[0]->tPlayerSpawn, m_pMaps[m_iIdTuto]->m_pRooms[0]->fAnglePlayerSpawn);
	GetCurrentMap()->PrepareMap();
}

void TWorld::ProcessDeath()
{
	m_fPostDeathCountDown = m_fPostDeathDelay;
}

TMap * TWorld::GetCurrentMap()
{
	switch (m_ePlayerLocation)
	{
		case LOCATION_HOME:
		{
			return m_pHome;
		}

		case LOCATION_TUTO:
		{
			return m_pMaps[m_iIdTuto];
		}

		default:
		{
			return m_pMaps[m_iCurrentMap];
		}
	}
}

int TWorld::GetNumberEnemyInRoom(int iIDRoom)
{
	int
		iIndexPlayer,
		iCountEnemy = 0;

	if (iIDRoom > 0)
	{
		iIndexPlayer = iIDRoom;
	}
	else
	{
		iIndexPlayer = g_pGame->m_pWorld->GetCurrentMap()->GetRoom(g_pGame->m_pWorld->m_pPlayer->Position());
	}

	if (iIndexPlayer > 0)
	{
		for (int i = 0; i < m_iEnemyCount; i++)
		{
			if (m_pEnemyList[i]->m_iIdRoom == iIndexPlayer)
			{
				iCountEnemy++;
			}
		}
	}
	return iCountEnemy;
}

bool TWorld::RoomIsCleaned(int iIDRoom)
{
	int
		iIndexPlayer;
	bool
		bAllKilled = true;
	if (iIDRoom > 0)
	{
		iIndexPlayer = iIDRoom;
	}
	else
	{
		iIndexPlayer = g_pGame->m_pWorld->GetCurrentMap()->GetRoom(g_pGame->m_pWorld->m_pPlayer->Position());
	}

	if (iIndexPlayer > 0)
	{
		for (int i = 0; i < m_iEnemyCount; i++)
		{
			if (m_pEnemyList[i]->m_iIdRoom == iIndexPlayer)
			{
				bAllKilled = false;
			}
		}
	}
	return bAllKilled;
}

TEnemy * TWorld::GetClosestEnemy(TGfxVec2 tOriginPos, float fMaxRange)
{
	TEnemy * pNewTarget = 0;
	vector<TEnemy *> pAllTarget;

	if (m_iEnemyCount != 0)
	{
		for (int iEnemy = 0; iEnemy < m_iEnemyCount; iEnemy++)
		{
			if ((tOriginPos - m_pEnemyList[iEnemy]->Position()).SquaredLength() < fMaxRange * fMaxRange)
			{
				if (pNewTarget != 0)
				{
					if ((tOriginPos - m_pEnemyList[iEnemy]->Position()).SquaredLength() < (tOriginPos - pNewTarget->Position()).SquaredLength())
					{
						bool bInSight = true;
						for (int iRoom = 0; iRoom < (int)GetCurrentMap()->m_pRooms.size(); ++iRoom)
						{
							for (int iWall = 0; iWall < GetCurrentMap()->m_pRooms[iRoom]->m_iWallCount; iWall++)
							{
								if (!IsInLineOfSight(m_pEnemyList[iEnemy]->Position(), m_pPlayer->Position(), GetCurrentMap()->m_pRooms[iRoom]->m_pWalls[iWall]))
								{
									bInSight = false;
									break;
								}
							}
							if (!bInSight) break;
						}
						if (bInSight)
							pNewTarget = m_pEnemyList[iEnemy];
					}
				}
				else
				{
					bool bInSight = true;
					for (int iRoom = 0; iRoom < (int)GetCurrentMap()->m_pRooms.size(); ++iRoom)
					{
						for (int iWall = 0; iWall < GetCurrentMap()->m_pRooms[iRoom]->m_iWallCount; iWall++)
						{
							if (!IsInLineOfSight(m_pEnemyList[iEnemy]->Position(), m_pPlayer->Position(), GetCurrentMap()->m_pRooms[iRoom]->m_pWalls[iWall]))
							{
								bInSight = false;
								break;
							}
						}
						if (!bInSight)	break;
					}
					if (bInSight)
						pNewTarget = m_pEnemyList[iEnemy];
				}
			}
		}
	}
	return pNewTarget;
}

vector<TEnemy *> TWorld::GetAllClosestEnemy(float fMaxRange)
{
	vector<TEnemy *>
		pAllTarget;
	TGfxVec2
		tOriginPos = m_pPlayer->Position();
	if (m_iEnemyCount != 0)
	{
		for (int iEnemy = 0; iEnemy < m_iEnemyCount; iEnemy++)
		{
			if ((tOriginPos - m_pEnemyList[iEnemy]->Position()).SquaredLength() < fMaxRange * fMaxRange)
			{
				bool
					bInSight = true;
				for (int iRoom = 0; iRoom < (int)GetCurrentMap()->m_pRooms.size(); ++iRoom)
				{
					for (int iWall = 0; iWall < GetCurrentMap()->m_pRooms[iRoom]->m_iWallCount; iWall++)
					{
						if (!IsInLineOfSight(m_pEnemyList[iEnemy]->Position(), m_pPlayer->Position(), GetCurrentMap()->m_pRooms[iRoom]->m_pWalls[iWall]))
						{
							bInSight = false;
							break;
						}
					}
					if (!bInSight)	break;
				}
				if (bInSight)
					pAllTarget.push_back(m_pEnemyList[iEnemy]);
			}
		}
	}
	return pAllTarget;
}

void TWorld::DetectCollision()
{
	TCollideResult tResult;

	GetCurrentMap()->CollideOnMap2(m_pPlayer);

	TGfxVec2 camToLook = m_pPlayer->m_tCam.m_tLookat - m_pPlayer->m_tCam.Position();

	float fAngleCame = atan2f(camToLook.y, camToLook.x);
	TGfxVec2 tAim = m_pPlayer->m_tCam.Position() + TGfxVec2(cosf(fAngleCame), sinf(fAngleCame)) * 6.0f;

	for (int i = 0; i < m_iEnemyCount; i++)
	{
		if (m_pEnemyList[i]->m_eEnemyType != EEnemyType_Boss)
			GetCurrentMap()->CollideOnMap2(m_pEnemyList[i]);
	}

	if (m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState == FOCUS_STATE_THROWN ||
		m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState == FOCUS_STATE_STRIKE_LEFT ||
		m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState == FOCUS_STATE_STRIKE_RIGHT ||
		m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState == FOCUS_STATE_STRIKE_ORBITAL)
	{
		int iEnemy = 0;
		TCollideResult tResult;
		while (iEnemy < m_iEnemyCount)
		{
			TEnemy * pEnemy = m_pEnemyList[iEnemy];
			if (pEnemy != 0 && !pEnemy->m_bIsDead)
			{
				if (Collide(m_pPlayer->m_pSpellManager->m_pFocus, m_pEnemyList[iEnemy], tResult))
				{
#ifdef TFA_FRESH
					LuaCallGlobal(GfxFreshLuaState(), "StartImpact");
#endif

					if (m_pEnemyList[iEnemy]->m_eEnemyType == EEnemyType_StrongAndSlow)
					{
						if ((m_pPlayer->m_pSpellManager->m_pFocus->Position() - m_pEnemyList[iEnemy]->Position()).DotProduct(TGfxVec2(cos(m_pEnemyList[iEnemy]->Rotation()), sinf(m_pEnemyList[iEnemy]->Rotation()))) <= 0)
						{
							switch (m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState)
							{
							case FOCUS_STATE_THROWN:		m_pEnemyList[iEnemy]->Damage(m_pPlayer->m_pSpellManager->m_pFocus->m_iIdCharge + 1);
								m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState = FOCUS_STATE_RETURNING;
								break;
							case FOCUS_STATE_STRIKE_LEFT:
							case FOCUS_STATE_STRIKE_RIGHT:
							case FOCUS_STATE_STRIKE_ORBITAL: m_pEnemyList[iEnemy]->Damage(1);
								break;
							default:				break;
							}
						}
					}
					else
					{
						switch (m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState)
						{
						case FOCUS_STATE_THROWN:		m_pEnemyList[iEnemy]->Damage(m_pPlayer->m_pSpellManager->m_pFocus->m_iIdCharge + 1);

							m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState = FOCUS_STATE_RETURNING;
							break;
						case FOCUS_STATE_STRIKE_LEFT:
						case FOCUS_STATE_STRIKE_RIGHT:
						case FOCUS_STATE_STRIKE_ORBITAL: m_pEnemyList[iEnemy]->Damage(1);
							break;
						default:				break;
						}
					}
				}

#ifndef TFA_FRESH
				m_pParticle->Generate(0, PI, 15, m_pEnemyList[iEnemy]->Position(), 0.15f, EGfxColor_Red);
#endif
			}
			iEnemy++;
		}
	}

	// :: Collide Damage  :: //
	for (int i = m_iCountDamage; i > 0; i--)
	{
		TCollideResult  tResult;
		if (m_pDamages[i - 1]->m_tData.m_fDelay < 0)
		{
			// :: DAMAGE FROM PLAYER
			if (m_pDamages[i - 1]->GetFaction() == FACTION_PLAYER)
			{
				// :: COLLIDE Damage - Enemy :: //
				for (int jEnemy = 0; jEnemy < m_iEnemyCount; jEnemy++)
				{
					if (m_pEnemyList[jEnemy]->m_eEnemyType != EEnemyType_Boss && !m_pEnemyList[jEnemy]->m_bIsDead)
					{
						if (m_pEnemyList[jEnemy] != 0 && m_pDamages[i - 1] != 0)
						{
							if (m_pDamages[i - 1]->CollideEnemy(*m_pEnemyList[jEnemy], tResult, jEnemy))
							{
								if (m_pEnemyList[jEnemy]->m_eBrainMode == EBrainMode_Slacking)
								m_pEnemyList[jEnemy]->m_tTempTarget = m_pDamages[i - 1]->m_tOrigin;
							}
						}
					}
				}
			}

			// :: DAMAGE FROM ENEMY :: //
			else if (m_pDamages[i - 1]->GetFaction() == FACTION_ENEMIES)
			{
				if (m_pDamages[i - 1]->CollidePlayer(tResult))
				{
				}

				//----Damage - Focus ----//
				else m_pDamages[i - 1]->CollideFocus(tResult);
			}
		}
	}

	// :: Collide Enemy :: //

	for (int i = 0; i < m_iEnemyCount; i++)
	{
		if (m_pEnemyList[i]->m_eEnemyType != EEnemyType_Boss)
		{
			// :: Enemy - Enemy :: //
			for (int j = 0; j < m_iEnemyCount; j++)
			{
				if (i != j)
				{
					m_pEnemyList[i]->CollideOther(*m_pEnemyList[j]);
				}
			}

			// :: Enemy - Player :: //
			if (Collide(m_pPlayer, m_pEnemyList[i], tResult))
			{
				m_pPlayer->CollideWall(tResult);
				m_pEnemyList[i]->CollidePlayer(tResult);
			}
		}
	}

	// :: Collide Damage - Wall :: //
	for (int i = m_iCountDamage; i > 0; i--)
	{
		if (m_pDamages[i - 1] != 0)
		{
			GetCurrentMap()->CollideOnMap2(m_pDamages[i - 1]);
		}
	}
}

#pragma region

//---- Collectible ----//
void TWorld::AddCollectible(TGfxVec2 tPos, float fHeight)
{
	if (m_iCollectibleCount < MAX_COLLECTIBLE)
	{
#ifdef TFA_FRESH
		LuaCreateCollectible(m_iCollectibleCount, tPos, fHeight);

#endif
		m_pCollectibleList[m_iCollectibleCount] = new TCollectible(tPos);
		m_pCollectibleList[m_iCollectibleCount]->m_fHeight = fHeight;
		m_iCollectibleCount++;
	}
}

void TWorld::DestroyCollectible(int iIndex)
{
	if (iIndex < m_iCollectibleCount)
	{
#ifdef TFA_FRESH
		LuaDestroyCollectible(iIndex, m_iCollectibleCount - 1);
#endif

		delete m_pCollectibleList[iIndex];
		m_pCollectibleList[iIndex] = m_pCollectibleList[m_iCollectibleCount - 1];
	}

	m_pCollectibleList[m_iCollectibleCount - 1] = 0;
	m_iCollectibleCount--;
}

void TWorld::AddEnemy(TGfxVec2 tPos, float fAngle, EEnemyType eType, TRoom * pRoom)
{
	if (m_iEnemyCount < MAX_ENEMY - 1)
	{
		int iIDRoom = GetCurrentMap()->GetRoom(tPos);
		float fHeightRoom = 0;

		if (iIDRoom != -1)
		{
			fHeightRoom = GetCurrentMap()->m_pRooms[iIDRoom]->m_tPos.y;
		}

		switch (eType)
		{
		case EEnemyType_RangedFast:
			m_pEnemyList[m_iEnemyCount] = new TEnemyFastRanged(tPos, iIDRoom, fHeightRoom);
			m_pEnemyList[m_iEnemyCount]->Rotation(fAngle);
			m_pEnemyList[m_iEnemyCount]->m_fDesiredAngle = fAngle;
			pRoom->m_vpEnemies.push_back(m_pEnemyList[m_iEnemyCount]);

#ifdef TFA_FRESH
			LuaCreateEnemy(m_iEnemyCount, tPos, 0.15, fAngle, eType, fHeightRoom);
#endif
			break;
		case EEnemyType_StrongAndSlow:
			m_pEnemyList[m_iEnemyCount] = new TEnemyStrongAndSlow(tPos, iIDRoom, fHeightRoom);
			m_pEnemyList[m_iEnemyCount]->Rotation(fAngle);
			m_pEnemyList[m_iEnemyCount]->m_fDesiredAngle = fAngle;
			pRoom->m_vpEnemies.push_back(m_pEnemyList[m_iEnemyCount]);
#ifdef TFA_FRESH
			LuaCreateEnemy(m_iEnemyCount, tPos, 0.3, fAngle, eType, fHeightRoom);
#endif
			break;

		case EEnemyType_Vegetable:
			m_pEnemyList[m_iEnemyCount] = new TEnemyVegetable(tPos, iIDRoom, fHeightRoom);
			m_pEnemyList[m_iEnemyCount]->Rotation(fAngle);
			m_pEnemyList[m_iEnemyCount]->m_fDesiredAngle = fAngle;
			pRoom->m_vpEnemies.push_back(m_pEnemyList[m_iEnemyCount]);
#ifdef TFA_FRESH
			LuaCreateEnemy(m_iEnemyCount, tPos, 0.2, fAngle, eType, fHeightRoom);
#endif
			break;

		case EEnemyType_Boss:
			m_pEnemyList[m_iEnemyCount] = new TBoss(tPos, iIDRoom);
			m_pEnemyList[m_iEnemyCount]->Rotation(fAngle);
			m_pEnemyList[m_iEnemyCount]->m_fDesiredAngle = fAngle;
			pRoom->m_vpEnemies.push_back(m_pEnemyList[m_iEnemyCount]);
			break;
		default:
			break;
		}

		m_iEnemyCount++;
	}
}

void TWorld::DestroyEnemy(int iIndex)
{
	bool bGetNewTarget = false;
	if (iIndex < m_iEnemyCount && iIndex >= 0)
	{
		if (m_pEnemyList[iIndex] != 0)
		{
			//----Delete----//
			if (m_pEnemyList[iIndex] == m_pPlayer->m_pTarget)
				bGetNewTarget = true;

			if (m_tMainMission->m_pTarget == m_pEnemyList[iIndex])
			{
				m_tMainMission->m_pTarget = 0;
			}
#ifdef TFA_FRESH
			if (m_pEnemyList[iIndex]->m_eEnemyType == EEnemyType_Boss)
				LuaDestroyBoss();
			else
				LuaDestroyEnemy(iIndex, m_iEnemyCount - 1);
#endif
			delete m_pEnemyList[iIndex];

			if (iIndex != m_iEnemyCount - 1)
			{
				m_pEnemyList[iIndex] = m_pEnemyList[m_iEnemyCount - 1];
			}
			m_iEnemyCount--;
			m_pEnemyList[m_iEnemyCount] = 0;

			if (bGetNewTarget)
				m_pPlayer->m_pTarget = 0;

			if (m_pPlayer->m_pTarget == 0)
				m_pPlayer->m_ePlayerMode = PLAYER_MODE_WALK;
			else m_pPlayer->m_ePlayerMode = PLAYER_MODE_COMBAT;
		}
	}
}

int TWorld::GetEnemyID(const TEnemy * pEnemy)
{
	for (int iEnemy = 0; iEnemy < m_iEnemyCount; ++iEnemy)
	{
		if (m_pEnemyList[iEnemy] != 0)
		{
			if (pEnemy == m_pEnemyList[iEnemy]) return iEnemy;
		}
	}
	return -1;
}

TDamage * TWorld::AddDamage(struct TDamageData &m_tDataDmg, enum ETypeDamage eType, const TGfxVec2 & tPos, const float fAngle, enum  EDamageFaction eFaction, unsigned int iColor, float fDiffAngle, TEnemy * tTarget)
{
	if (m_iCountDamage < MAX_DAMAGE)
	{
#ifdef TFA_FRESH
		if (m_tDataDmg.m_fDelay >= 0)
			LuaCreateDamage(m_iCountDamage, tPos, m_tDataDmg.m_tRadius.x, m_tDataDmg.m_tRadius.y, fAngle, false, m_tDataDmg.eForm, m_tDataDmg.m_fHeight, m_tDataDmg.m_iStyle);
		else
			LuaCreateDamage(m_iCountDamage, tPos, m_tDataDmg.m_tRadius.x, m_tDataDmg.m_tRadius.y, fAngle, true, m_tDataDmg.eForm, m_tDataDmg.m_fHeight, m_tDataDmg.m_iStyle);
#endif
		m_pDamages[m_iCountDamage++] = new TDamage(m_tDataDmg, eType, tPos, fAngle, eFaction, iColor, fDiffAngle, 0, tTarget);

		return  m_pDamages[m_iCountDamage - 1];
	}
	return 0;
}

void TWorld::DestroyDamage(int iIndex)
{
	if (iIndex < m_iCountDamage)
	{
		m_iCountDamage--;
#ifdef TFA_FRESH
		LuaDestroyDamage(iIndex, m_iCountDamage);
#endif

		delete m_pDamages[iIndex];
		m_pDamages[iIndex] = m_pDamages[m_iCountDamage];
	}

	m_pDamages[m_iCountDamage] = 0;
}

void TWorld::DestroyDamage(TDamage * pDamage)
{
	if (m_iCountDamage > 0)
	{
		for (int i = 0; i < m_iCountDamage; ++i)
		{
			if (m_pDamages[i] == pDamage)
			{
				DestroyDamage(i);
				break;
			}
		}
	}
}

#pragma endregion Level_Buider

void TWorld::Render()
{
#ifndef TFA_FRESH
	GfxTextSpriteRender(g_Text, 20, 20, EGfxColor_White);

	GfxSpriteRender(m_pGridLineSprite);

	GfxSpriteRender(m_pRoomLineSprite);
	GfxSpriteRender(m_pMapLineSprite);

	GfxSpriteRender(m_pLineSprite);
	GfxSpriteRender(m_pParticleSprite);
	GfxSpriteRender(m_pHudMap);
	GfxSpriteRender(m_pDynamicHudMap);
	extern TGfxSprite * gTempStandbyTextSprite;
	extern TGfxSprite * gTempStateTextSprite;

	GfxSpriteRender(m_pPlayer->m_pSpellManager->m_pSprite);
	GfxTextSpriteRender(m_pTextMission, 50, 50, EGfxColor_White);
	GfxTextSpriteRender(g_Text, 20, 20, EGfxColor_White);
	m_pHud->Render();
	m_tSpellBook.Render();

#endif
}

// :: PRIVATE
void TWorld::SpawnEnemiesTutorial(TMap * pTuto)
{
	for (int iRoom = 0; iRoom < (int)pTuto->m_pRooms.size(); iRoom++)
	{
		for (int iSpawner = 0; iSpawner < (int)pTuto->m_pRooms[iRoom]->m_tSpawners.size(); iSpawner++)
		{
			const TSpawner & tSpawner = pTuto->m_pRooms[iRoom]->m_tSpawners[iSpawner];
			TGfxVec2 tEnemyPos = TGfxVec2(tSpawner.m_tPos.x, tSpawner.m_tPos.z);

			AddEnemy(tEnemyPos, tSpawner.m_fAngle, EEnemyType_Vegetable, pTuto->m_pRooms[iRoom]);
		}
	}

	if (GetCurrentMap() != pTuto)
	{
		GfxDbgAssert("Dafuq are you doing ? I only spawn enemies tutorial in Tuto");
	}
}

void TWorld::SpawnEnemies(int /*iDifficulty*/)
{
	for (int iRoom = 0; iRoom < (int)GetCurrentMap()->m_pRooms.size(); iRoom++)
	{
		for (int iSpawner = 0; iSpawner < (int)GetCurrentMap()->m_pRooms[iRoom]->m_tSpawners.size(); iSpawner++)
		{
			TSpawner & tSpawner = GetCurrentMap()->m_pRooms[iRoom]->m_tSpawners[iSpawner];

			int iEnemyToGenereate = 1;
			tSpawner.m_bForceSpawn = true;
			if (tSpawner.m_bForceSpawn)
			{
				iEnemyToGenereate = 1;
			}
			else
			{
				iEnemyToGenereate = GfxMathGetRandomInteger(0, 1);
			}

			for (int j = 0; j < iEnemyToGenereate; j++)
			{
				vector<EEnemyType> vEnemyList;

				if (tSpawner.m_tEnemiesFlags.GetFlag(EEnemyType_RangedFast)) vEnemyList.push_back(EEnemyType_RangedFast);
				if (tSpawner.m_tEnemiesFlags.GetFlag(EEnemyType_Grunt)) vEnemyList.push_back(EEnemyType_Grunt);
				if (tSpawner.m_tEnemiesFlags.GetFlag(EEnemyType_StrongAndSlow)) vEnemyList.push_back(EEnemyType_StrongAndSlow);
				if (tSpawner.m_tEnemiesFlags.GetFlag(EEnemyType_Vegetable)) vEnemyList.push_back(EEnemyType_Vegetable);
				if (tSpawner.m_tEnemiesFlags.GetFlag(EEnemyType_Boss)){ vEnemyList.push_back(EEnemyType_Boss); GfxDbgPrintf("SpawnBoss\n"); }

				TGfxVec2 tEnemyPos = TGfxVec2(tSpawner.m_tPos.x, tSpawner.m_tPos.z);

				if (vEnemyList.size() == 0)AddEnemy(tEnemyPos, tSpawner.m_fAngle, EEnemyType_StrongAndSlow, GetCurrentMap()->m_pRooms[iRoom]);
				else if (vEnemyList.size() == 0)AddEnemy(tEnemyPos, tSpawner.m_fAngle, EEnemyType_StrongAndSlow, GetCurrentMap()->m_pRooms[iRoom]);
				else{
					EEnemyType eEnemyToSpawn = vEnemyList[GfxMathGetRandomInteger(0, vEnemyList.size() - 1)];
					AddEnemy(tEnemyPos, tSpawner.m_fAngle, eEnemyToSpawn, GetCurrentMap()->m_pRooms[iRoom]);
				}
			}
		}
	}
}

void TWorld::GarbageCleaner()
{
	int iIndex = 0;
	while (iIndex < m_iCountDamage)
	{
		if (m_pDamages[iIndex]->m_bNeedToBeDestroyed)
		{
			DestroyDamage(iIndex);
		}
		iIndex++;
	}

	iIndex = 0;
	while (iIndex < m_iEnemyCount)
	{
		if (m_pEnemyList[iIndex]->m_bNeedToBeDestroyed)
		{
			if (m_pEnemyList[iIndex]->m_eEnemyType == EEnemyType_Boss)
			{
				g_pGame->Victory();
			}
			DestroyEnemy( iIndex );
		}
		iIndex++;
	}
}