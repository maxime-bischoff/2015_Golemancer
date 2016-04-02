#include "Const.h"
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
using std::vector;
using std::string;
extern TGame * g_pGame;

int ObjDump(lua_State * L);

TMapGenerator::TMapGenerator() : m_iRoomPatternsCount(0)
{
}

bool StringToBool(const char * value)
{
	if (!strcmp(value, "true"))
		return true;
	else return false;
}

void TMapGenerator::ImportRoomPatterns()
{
	vector<string> sRoomsList;
	// :: Tuto
	sRoomsList.push_back("Room_Tuto_00");
	sRoomsList.push_back("Room_Tuto_02");
	sRoomsList.push_back("Room_Tuto_01");
	sRoomsList.push_back("Room_Tuto_02_bis");
	sRoomsList.push_back("Room_Tuto_02_ter");
	sRoomsList.push_back("Room_Tuto_03");

	// :: Game
	sRoomsList.push_back("Room_Home_02");
	//sRoomsList.push_back("Room_Entrance_01");
	sRoomsList.push_back("Room_Entrance_02");
	sRoomsList.push_back("Room_Standard_01");
	sRoomsList.push_back("Room_Standard_02");
//	sRoomsList.push_back("Room_Standard_03");
	sRoomsList.push_back("Room_Standard_04_Big");
	sRoomsList.push_back("Room_Exit_01");
	sRoomsList.push_back("Room_Boss_01");

	TXML_TParser * pParser = new  TXML_TParser();

	string sPath = "Map/Room";

	bool bError = false;

	int idRoom = -1;

	while (!bError)
	{
		idRoom++;

		if (idRoom >= (int)sRoomsList.size()) break;

		sPath = "Data/Maps/";
		sPath += sRoomsList[idRoom];
		sPath += ".xml";

		char *cPath = new char[sPath.length() + 1];
		std::strcpy(cPath, sPath.c_str());
		TXML_Root * tRoot = pParser->ReadXML(cPath);
		GfxDbgPrintf(cPath);
		GfxDbgPrintf("\n");

		delete[] cPath;

		if (tRoot == 0)
		{
			bError = true;
		}
		else
		{
			//	pParser->ExtractXML();

			TXML_Element * pCurrentElement = pParser->m_tRoot->m_pRootElementsList->m_pFirstItem;

			TXML_Element * pElementTree[50];
			int iDepth = 0;
			TXML_Attribute * pAttribute;// = pCurrentElement->m_pAttributesList->m_pFirstItem;

			pElementTree[iDepth] = pCurrentElement;

			bool bFinished = false;
			//EElementName eCurrentElementName;
			pCurrentElement = pCurrentElement->m_pElementChildsList->m_pFirstItem;
			pElementTree[1] = pCurrentElement;
			iDepth++;

			//pMap->m_iRoomsCount++;
			m_iRoomPatternsCount++;
			m_pRoomPatterns.push_back(new TRoom());
			m_pRoomPatterns[m_pRoomPatterns.size() - 1]->m_tGround.m_iTriangleCount = 0;
			m_pRoomPatterns[m_pRoomPatterns.size() - 1]->m_tGround.m_pIndices = 0;
			m_pRoomPatterns[m_pRoomPatterns.size() - 1]->m_tGround.m_pVertices = 0;
			//GfxDbgPrintf(" *_*_* Going To Parse Ground Collider \n");
			ImportGroundCollider( sRoomsList[m_pRoomPatterns.size() - 1] );
			//GfxDbgPrintf(" *_*_* After Parsing Ground Collider \n");
			TRoom * pRoom = m_pRoomPatterns[m_pRoomPatterns.size() - 1];
			pRoom->m_iDoorCount = 0;
			if (idRoom == 0) pRoom->m_eType = EROOMTYPE_HOME; else pRoom->m_eType = EROOMTYPE_STANDARD;

			int iCount = 0;

			while (!bFinished)
			{
				char * pStrElementName = pCurrentElement->m_cName;
				pRoom->id = m_pRoomPatterns.size() - 1;

				// =============================== //
				// ======= TYPE OF THE ROOM ====== //

				if (!strcmp(pStrElementName, "room"))
				{
					pAttribute = pCurrentElement->FindAttribute("type");
					if (pAttribute == 0)
						break;

					ERoomType eType = EROOMTYPE_STANDARD;
					if (!strcmp(pAttribute->GetValueString(), "Entrance")){
						eType = EROOMTYPE_ENTRANCE;
						pRoom->m_eActionToUnlock = UNLOCK_ONSTART;
					}
					else if (!strcmp(pAttribute->GetValueString(), "Standard")){
						eType = EROOMTYPE_STANDARD;
						pRoom->m_eActionToUnlock = UNLOCK_ONCLEAN;
					}
					else if (!strcmp(pAttribute->GetValueString(), "Empty")){
						eType = EROOMTYPE_EMPTY;
						pRoom->m_eActionToUnlock = UNLOCK_ONCLEAN;
					}
					else if (!strcmp(pAttribute->GetValueString(), "Corridor")){
						eType = EROOMTYPE_CORRIDOR;
					}
					else if (!strcmp(pAttribute->GetValueString(), "Home")){
						eType = EROOMTYPE_HOME;
						pRoom->m_eActionToUnlock = UNLOCK_NEVER;
					}
					else if (!strcmp(pAttribute->GetValueString(), "Exit")){
						eType = EROOMTYPE_EXIT;
						pRoom->m_eActionToUnlock = UNLOCK_NEVER;
					}
					else if (!strcmp(pAttribute->GetValueString(), "Tuto")){
						eType = EROOMTYPE_TUTO;
						pRoom->m_eActionToUnlock = UNLOCK_ONCLEAN;
					}
					else if (!strcmp(pAttribute->GetValueString(), "Boss")){
						eType = EROOMTYPE_BOSS;
						pRoom->m_eActionToUnlock = UNLOCK_NEVER;
					}

					pRoom->m_eType = eType;
				}

				// ============================== //
				// ===IMPORT DES COLLISIONS ===== //

				else if (!strcmp(pStrElementName, "collision"))
				{
					TGfxVec2 tPos(9999, 9999);
					TGfxVec2 tSize(9999, 9999);
					float fAngle = 9999;

					pAttribute = pCurrentElement->FindAttribute("posX");
					if (pAttribute == 0)
						break;	else tPos.x = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posZ");
					if (pAttribute == 0)
						break;	else tPos.y = pAttribute->GetValueNumeric() *-1;

					bool bCollideCamera = false;
					bool bCollidePlayer = false;
					bool bCollideProjectiles = false;

					pAttribute = pCurrentElement->FindAttribute("collidecamera");
					if (pAttribute == 0)
						break;	else bCollideCamera = StringToBool(pAttribute->GetValueString());

					pAttribute = pCurrentElement->FindAttribute("collideplayer");
					if (pAttribute == 0)
						break;	else bCollidePlayer = StringToBool(pAttribute->GetValueString());

					pAttribute = pCurrentElement->FindAttribute("collideprojectiles");
					if (pAttribute == 0)
						break;	else bCollideProjectiles = StringToBool(pAttribute->GetValueString());
					unsigned int iColor = EGfxColor_White;

					if (bCollidePlayer && bCollideCamera && bCollideProjectiles)
						iColor = EGfxColor_Red;
					else if (bCollidePlayer && !bCollideCamera && !bCollideProjectiles)
						iColor = GfxColor(150, 150, 255, 255);
					else if (bCollidePlayer && !bCollideCamera && bCollideProjectiles)
						iColor = GfxColor(150, 150, 0, 255);
					else if (bCollidePlayer && bCollideCamera && bCollideProjectiles)
						iColor = GfxColor(255, 0, 255, 255);

					pAttribute = pCurrentElement->FindAttribute("shape");
					if (pAttribute == 0)
						break; else {
						if (!strcmp(pAttribute->GetValueString(), "box"))
						{
							pAttribute = pCurrentElement->FindAttribute("rotation");
							if (pAttribute == 0)
								break;	else fAngle = -pAttribute->GetValueNumeric() * PI / 180.0f;

							pAttribute = pCurrentElement->FindAttribute("radiusX");
							if (pAttribute == 0)
								break;	else tSize.x = pAttribute->GetValueNumeric();

							pAttribute = pCurrentElement->FindAttribute("radiusZ");
							if (pAttribute == 0)
								break;	else tSize.y = pAttribute->GetValueNumeric();

							pRoom->m_pWalls[pRoom->m_iWallCount++] = new TWall(tPos, tSize, fAngle, bCollideCamera, bCollidePlayer, bCollideProjectiles, iColor);
						}

						else if (!strcmp(pAttribute->GetValueString(), "circle"))
						{
							pAttribute = pCurrentElement->FindAttribute("radius");
							if (pAttribute == 0)
								break;	else tSize.x = pAttribute->GetValueNumeric();

							pRoom->m_pWalls[pRoom->m_iWallCount++] = new TWall(tPos, tSize.x, fAngle, bCollideCamera, bCollidePlayer, bCollideProjectiles, iColor);
						}
					}
				}
				// =============================== //
				// === IMPORT PLAYERSPAWN ===== //



				else if (!strcmp(pStrElementName, "playerSpawn"))
				{
					TGfxVec2 tPos(9999, 9999);
					TGfxVec2 tSize(9999, 9999);
					float fAngle = 9999;
					

					pAttribute = pCurrentElement->FindAttribute("rotation");
					if (pAttribute == 0)
						break;	else fAngle = pAttribute->GetValueNumeric() * DegToRad - PI_2;
					
					pAttribute = pCurrentElement->FindAttribute("posX");
					if (pAttribute == 0)
						break;	else tPos.x = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posZ");
					if (pAttribute == 0)
						break;	else tPos.y = pAttribute->GetValueNumeric() *-1.0f;
									
					pRoom->tPlayerSpawn = tPos;
					pRoom->fAnglePlayerSpawn = fAngle;
				}

				// =============================== //
				// === IMPORT BOSS SPAWN ===== //



				else if (!strcmp(pStrElementName, "bossSpawn"))
				{
					TGfxVec3 tPos(9999, 9999,999);					
					float fAngle = 9999;
					TSpawner tSpawner;

					pAttribute = pCurrentElement->FindAttribute("rotation");
					if (pAttribute == 0)
						break;	else fAngle = pAttribute->GetValueNumeric() * DegToRad - PI_2;

					pAttribute = pCurrentElement->FindAttribute("posX");
					if (pAttribute == 0)
						break;	else tPos.x = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posY");
					if (pAttribute == 0)
						break;	else tPos.y = pAttribute->GetValueNumeric() *-1.0f;

					pAttribute = pCurrentElement->FindAttribute("posZ");
					if (pAttribute == 0)
						break;	else tPos.z = pAttribute->GetValueNumeric() *-1.0f;

					pRoom->m_tBossSpawn = tPos;
					pRoom->m_fAngleBossSpawn = fAngle;


					tSpawner.m_fAngle = fAngle;
					tSpawner.m_tPos = tPos;
					tSpawner.m_tEnemiesFlags.SetFlag(EEnemyType_Boss, true);

					pRoom->m_tSpawners.push_back(tSpawner);

				}


				// =============================== //
				// ==== IMPORT DES TRIGGERS ====== //

				else if (!strcmp(pStrElementName, "trigger"))
				{
					TGfxVec2 tPos(9999, 9999);
					TGfxVec2 tSize(9999, 9999);
					float fHeight = 0;

					pAttribute = pCurrentElement->FindAttribute("posX");
					if (pAttribute == 0)
						break;	else tPos.x = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posY");
					if (pAttribute == 0)
						fHeight = 0;	else fHeight = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posZ");
					if (pAttribute == 0)
						break;	else tPos.y = pAttribute->GetValueNumeric() * -1.0f;



					pAttribute = pCurrentElement->FindAttribute("type");
					if (pAttribute == 0)
						break;

					TTrigger::ETriggerType eType = TTrigger::ETRIGGER_LOADER;
					if (!strcmp(pStrElementName, "Loader"))
						eType = TTrigger::ETRIGGER_LOADER;
					else if (!strcmp(pStrElementName, "Exit"))
						eType = TTrigger::ETRIGGER_EXIT;

					if (pRoom->m_iTriggerCount < pRoom->MAX_TRIGGER){
						pRoom->m_pTriggers[pRoom->m_iTriggerCount++] = new TTrigger(tPos, eType);
						pRoom->m_pTriggers[pRoom->m_iTriggerCount-1]->m_fHeight = fHeight;
					}
				}

				// =============================== //
				// ==== IMPORT DES SORTIE ====== //

				else if (!strcmp(pStrElementName, "teleport"))
				{
					TGfxVec2 tPos(9999, 9999);
					TGfxVec2 tSize(9999, 9999);
					float fHeight = 0;

					pAttribute = pCurrentElement->FindAttribute("posX");
					if (pAttribute == 0)
						break;	else tPos.x = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posY");
					if (pAttribute == 0)
						fHeight;	else fHeight = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posZ");
					if (pAttribute == 0)
						break;	else tPos.y = pAttribute->GetValueNumeric() * -1.0f;

					if (pRoom->m_iTriggerCount < pRoom->MAX_TRIGGER){
						pRoom->m_pTriggers[pRoom->m_iTriggerCount++] = new TTrigger(tPos, TTrigger::ETRIGGER_LOADER);
						pRoom->m_pTriggers[pRoom->m_iTriggerCount-1]->m_fHeight = fHeight;
					}
				}

				// =============================== //
				// ==== IMPORT DES LOADER ====== //

				else if (!strcmp(pStrElementName, "exit"))
				{
					TGfxVec2 tPos(9999, 9999);
					TGfxVec2 tSize(9999, 9999);

					pAttribute = pCurrentElement->FindAttribute("posX");
					if (pAttribute == 0)
						break;	else tPos.x = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posZ");
					if (pAttribute == 0)
						break;	else tPos.y = pAttribute->GetValueNumeric() * -1.0f;

					if (pRoom->m_iTriggerCount < pRoom->MAX_TRIGGER){
						pRoom->m_pTriggers[pRoom->m_iTriggerCount++] = new TTrigger(tPos, TTrigger::ETRIGGER_EXIT);
						pRoom->m_pTriggers[pRoom->m_iTriggerCount - 1]->m_iDifficulty = pRoom->m_iTriggerCount - 1;
					}
				}

				// ================================ //
				// === IMPORT DES ENEMYSPAWN ===== //

				else if (!strcmp(pStrElementName, "enemySpawn"))
				{
					TGfxVec3 tPos(9999, 9999,9999);					
					TSpawner tSpawner;

					pAttribute = pCurrentElement->FindAttribute("posX");
					if (pAttribute == 0)
						break;	else tPos.x = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posY");
					if (pAttribute == 0)
						tPos.y = 0 ;	else tPos.y = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posZ");
					if (pAttribute == 0)
						break;	else tPos.z = pAttribute->GetValueNumeric() *-1.0f;

					pAttribute = pCurrentElement->FindAttribute("rotation");
					if (pAttribute == 0)
						tSpawner.m_fAngle = 0;	else tSpawner.m_fAngle = pAttribute->GetValueNumeric() * DegToRad - PI_2;
									
					pAttribute = pCurrentElement->FindAttribute("golem_laser");
					if (pAttribute == 0)
						tSpawner.m_tEnemiesFlags.SetFlag(EEnemyType_RangedFast, false);	else 	tSpawner.m_tEnemiesFlags.SetFlag(EEnemyType_RangedFast, StringToBool(pAttribute->GetValueString() ) );

					pAttribute = pCurrentElement->FindAttribute("golem_stun");
					if (pAttribute == 0)
						tSpawner.m_tEnemiesFlags.SetFlag(EEnemyType_Grunt, false);	else 	tSpawner.m_tEnemiesFlags.SetFlag(EEnemyType_Grunt, StringToBool(pAttribute->GetValueString()));

					pAttribute = pCurrentElement->FindAttribute("golem_charge");
					if (pAttribute == 0)
						tSpawner.m_tEnemiesFlags.SetFlag(EEnemyType_StrongAndSlow, false);	else 	tSpawner.m_tEnemiesFlags.SetFlag(EEnemyType_StrongAndSlow, StringToBool(pAttribute->GetValueString()));

					pAttribute = pCurrentElement->FindAttribute("golem_vegetable");
					if (pAttribute == 0)
						tSpawner.m_tEnemiesFlags.SetFlag(EEnemyType_Vegetable, false);	else 	tSpawner.m_tEnemiesFlags.SetFlag(EEnemyType_Vegetable, StringToBool(pAttribute->GetValueString()));
					
					tSpawner.m_tPos = tPos;
					pRoom->m_tSpawners.push_back(tSpawner);
				}
				else if (!strcmp(pStrElementName, "door"))
				{
					TGfxVec3 tPos(0, 0, 0);				
					TGfxVec2 tSize(9999, 9999);

					pAttribute = pCurrentElement->FindAttribute("posX");
					if (pAttribute == 0)
						break;	else tPos.x = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posY");
					if (pAttribute == 0)
						tPos.y = 0; else  tPos.y = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posZ");
					if (pAttribute == 0)
						break;	else tPos.z = pAttribute->GetValueNumeric() *-1.0f;

					pAttribute = pCurrentElement->FindAttribute("type");
					TGfxVec2 tDoorSize = TGfxVec2(0.5f, 10.0f);
				
					EDoor eDoorOrientation = EDOOR_TOP;

					if (!strcmp(pAttribute->GetValueString(), "North"))	{
						eDoorOrientation = EDOOR_TOP;
					}
					else if (!strcmp(pAttribute->GetValueString(), "South")){			
						eDoorOrientation = EDOOR_BOTTOM;
					}
					else if (!strcmp(pAttribute->GetValueString(), "West")) {	
						eDoorOrientation = EDOOR_LEFT;
					}
					else if (!strcmp(pAttribute->GetValueString(), "East")) {					
						eDoorOrientation = EDOOR_RIGHT;
					}

					pRoom->m_tEntrances[eDoorOrientation].m_tPos = tPos;
					pRoom->m_tEntrances[eDoorOrientation].bExist = true;
					pRoom->m_tEntrances[eDoorOrientation].m_eOrientation = eDoorOrientation;

					pAttribute = pCurrentElement->FindAttribute("socket");
					ESocketType eSocketType = SOCKET_NORMAL;

					if (pAttribute != 0)
					{			
						if (!strcmp(pAttribute->GetValueString(), "Normal")) {
							eSocketType = SOCKET_NORMAL;
						}
						else if (!strcmp(pAttribute->GetValueString(), "Empty"))
						{
							eSocketType = SOCKET_EMPTY;
						}					
					}

					pRoom->m_tEntrances[eDoorOrientation].m_eSocketType = eSocketType;




				}

				// ===============================//
				// ===== IMPORT POSITION ORBE ===== //

				else if (!strcmp(pStrElementName, "orbe"))
				{
					TGfxVec2 tPos(9999, 9999);
					float fHeight = 0;


					pAttribute = pCurrentElement->FindAttribute("posX");
					if (pAttribute == 0)
						break;
					else tPos.x = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posZ");
					if (pAttribute == 0)
						break;
					else tPos.y = pAttribute->GetValueNumeric() * -1.0f;

					pAttribute = pCurrentElement->FindAttribute("posY");
					if (pAttribute == 0)
						break;
					else fHeight = pAttribute->GetValueNumeric();

					g_pGame->m_pWorld->m_pPlayer->m_pSpellManager->m_pFocus->Position(TGfxVec2(tPos.x, tPos.y));

					//GfxDbgPrintf("XML Orbe position %f - %f - %f", tPos.x, fHeight + 2.0f, tPos.y);
					g_pGame->m_pWorld->m_pPlayer->m_pSpellManager->m_pFocus->m_fHeight = fHeight +0.7f;
				}

				// ======================================//
				// ===== IMPORT DES BOUNDING BOXES ===== //

				else if (!strcmp(pStrElementName, "boundingbox"))
				{
					TGfxVec2 tPos(9999, 9999);
					TGfxVec2 tSize(9999, 9999);

					pAttribute = pCurrentElement->FindAttribute("posX");
					if (pAttribute == 0)
						break;
					else tPos.x = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("posZ");
					if (pAttribute == 0)
						break;
					else tPos.y = pAttribute->GetValueNumeric() * -1.0f;

					pAttribute = pCurrentElement->FindAttribute("radiusX");
					if (pAttribute == 0)
						break;
					else tSize.x = pAttribute->GetValueNumeric();

					pAttribute = pCurrentElement->FindAttribute("radiusZ");
					if (pAttribute == 0)
						break;
					else tSize.y = pAttribute->GetValueNumeric();

					pRoom->m_tPositionBoundingBox = TGfxVec2(tPos.x, tPos.y);
					pRoom->m_tSizeBoundingBox = TGfxVec2(tSize.x, tSize.y);
					pRoom->m_tBoundingBox.ur = TGfxVec2(tSize.x + tPos.x, tSize.y + tPos.y);
					pRoom->m_tBoundingBox.bl = TGfxVec2(-tSize.x + tPos.x, -tSize.y + tPos.y);
				}
				// ========================================== //
				// === IMPORT DES NODES DE PATHFINDING ===== //

				else if (!strcmp(pStrElementName, "node"))
				{
					TGfxVec2 tPos(9999, 9999);

					pAttribute = pCurrentElement->FindAttribute("posZ");
					if (pAttribute == 0)
						break;	else tPos.y = pAttribute->GetValueNumeric();					
				}

				//Check si l'element dispose d'un enfant

				if (pCurrentElement->m_pElementChildsList->m_pFirstItem != 0)
				{
					pElementTree[iDepth - 1] = pCurrentElement;
					pCurrentElement = pCurrentElement->m_pElementChildsList->m_pFirstItem;
					pElementTree[iDepth++] = pCurrentElement;
				}
				else if (pCurrentElement->GetNextItem() != 0)
				{
					pCurrentElement = pCurrentElement->GetNextItem();
				}
				else{
					//iDepth--;
					//	pCurrentElement = pElementTree[iDepth-1];
					bool bNextFound = false;

					while (iDepth > 1 && bNextFound == false)
					{
						iDepth--;
						pCurrentElement = pElementTree[iDepth - 1];

						if (pCurrentElement->GetNextItem() != 0)
						{
							pCurrentElement = pCurrentElement->GetNextItem();
							bNextFound = true;
						}
					}
				}
				if (iDepth == 1){ bFinished = true; }
				iCount++;
			}
		}
	}

#ifdef TFA_FRESH

	for (int i = 0; i< (int)sRoomsList.size(); ++i)
	{
		char *cRoomName = new char[sRoomsList[i].length() + 1];
		std::strcpy(cRoomName, sRoomsList[i].c_str());

		GfxDbgPrintf(cRoomName);
		lua_pushstring(GfxFreshLuaState(),cRoomName);
		LuaCallGlobal(GfxFreshLuaState(), "LoadRoomPak");
		delete [] cRoomName ;
	}

#endif
}

TMapGenerator::~TMapGenerator()
{
}

void TMapGenerator::GenerateHome(THome * pHome)
{
	TRoom * pRoom = GetRandomRoomOfType(EROOMTYPE_HOME);

	pHome->m_pRooms.push_back(new TRoom(*pRoom));
	pHome->m_tPlayerSpawn = pRoom->tPlayerSpawn;

	int iTriggerCount = 0;
	for (unsigned int iRoom = 0; iRoom < pHome->m_pRooms.size(); ++iRoom)
	{
		for (int iTrigger = 0; iTrigger < pHome->m_pRooms[iRoom]->m_iTriggerCount; ++iTrigger)
		{
			TTrigger * pTrigger = pHome->m_pRooms[iRoom]->m_pTriggers[iTrigger];
			if (pTrigger != 0 && pTrigger->m_eTriggerType == TTrigger::ETRIGGER_LOADER)
			{
				pTrigger->m_iMapId = iTriggerCount;
				iTriggerCount++;
			}
		}
	}
	pHome->PlaceDoors(EROOMTYPE_TUTO);


#ifdef TFA_FRESH

#endif
}

TMap * TMapGenerator::GenerateTuto()
{
	TMap 
		* pMap = new TMap();
	TRoom
		* pStarter;
	int
		iCountRoom = 0;
	vector<TRoom *>
		pRoomsTuto = GetAllRoomOfType(EROOMTYPE_TUTO);

	pMap->m_tPlayerSpawn = pRoomsTuto[0]->tPlayerSpawn;
	pStarter = pRoomsTuto[0];
	pStarter->idInArray = 0;
	pMap->m_pRooms.push_back(new TRoom(*pStarter));

	while (iCountRoom < (int)pRoomsTuto.size() - 1)
	{
		TRoom
			* pLastRoom,
			* pRoomNew,
			* pRoomPattern;
		EDoor
			eDoorExit,
			eDoorEntrance;
		TGfxVec3
			tLastDoorPos;

		pLastRoom = pMap->m_pRooms[iCountRoom];
		eDoorExit = EDOOR_TOP;
		tLastDoorPos = pLastRoom->m_tEntrances[EDOOR_TOP].m_tPos + pLastRoom->m_tPos;

		eDoorEntrance = GetOppositeDoor(eDoorExit);
		pRoomPattern = 0;
		pRoomPattern = pRoomsTuto[iCountRoom + 1];

		pMap->m_pRooms.push_back(new TRoom(*pRoomPattern));

		pRoomNew = pMap->m_pRooms[iCountRoom + 1];
		

		pLastRoom->m_tEntrances[eDoorExit].tDestRoom = pRoomNew;
		pRoomNew->m_tEntrances[eDoorEntrance].tDestRoom = pLastRoom;

		pRoomNew->Translate(tLastDoorPos - pRoomNew->m_tEntrances[eDoorEntrance].m_tPos);

		if (iCountRoom == pRoomsTuto.size() - 1)
		{
			pRoomNew->m_eActionToUnlock = UNLOCK_NEVER;
		}

		iCountRoom++;
	}


	// :: Close the doors
	pMap->PlaceDoors(EROOMTYPE_TUTO);

	return pMap;
}

TMap * TMapGenerator::GenerateBoss()
{

	TMap * pMap = new TMap;;
	TRoom * pStarter;

	vector<TRoom *>	pRoomsEntrance = GetAllRoomOfType(EROOMTYPE_ENTRANCE);

	pMap->m_tPlayerSpawn = pRoomsEntrance[0]->tPlayerSpawn;
	pStarter = pRoomsEntrance[0];
	pMap->m_pRooms.push_back(new TRoom(*pStarter));
	

	TRoom * pEntrance = pMap->m_pRooms[0];
	vector<TRoom *>	pRoomsBoss = GetAllRoomOfType(EROOMTYPE_BOSS);		
	
	TGfxVec3 tLastDoorPos = pEntrance->m_tEntrances[EDOOR_TOP].m_tPos + pEntrance->m_tPos;

	pMap->m_pRooms.push_back(new TRoom(*pRoomsBoss[0]));
			
	TRoom * pBossRoom = pMap->m_pRooms[1];
	
	pBossRoom->m_tEntrances[EDOOR_BOTTOM].tDestRoom = pEntrance;
	pEntrance->m_tEntrances[EDOOR_TOP].tDestRoom = pBossRoom;
	pBossRoom->Translate(tLastDoorPos - pBossRoom->m_tEntrances[EDOOR_BOTTOM].m_tPos);

	pMap->PlaceDoors(EROOMTYPE_BOSS);

	
	return pMap;

	// :: Close the doors
}


TMap * TMapGenerator::GenerateMap(const int iSeed, int iDifficulty)
{
	iDifficulty = (int)MClamp((float)iDifficulty, 0, 5);

	TMap * pMap = new TMap();

	GfxMathResetRandomSeed(iSeed);

	TGfxVec2 tStartRoom(0, 0);

	TRoom * pStarter = GetRandomRoomOfType(EROOMTYPE_ENTRANCE);

	TGfxVec3 tRoomPos = TGfxVec3(0, 0, 0);

	TGfxVec3 tLastDoorPos = pStarter->m_tEntrances[EDOOR_TOP].m_tPos + tRoomPos;
	//int idRoom = 0;

	if (pStarter == 0){ GfxDbgPrintf("Error, there is no Entrance room"); return 0; }
	pMap->m_pRooms.push_back(new TRoom(*pStarter));
	TRoom * pLastRoom = pMap->m_pRooms[0];
	EDoor eDoorExit = EDOOR_TOP;

	bool bAbleToPlace = true;
	int iRoomCount = 0;
	int iRoomMax = 2 + iDifficulty;
	//iRoomMax = 0.0f;
	while (bAbleToPlace && iRoomCount <= iRoomMax)
	{
		//bool bRoomFounded = false;

		// *********** Cherche un type de salle à générer **** //

		//if (iRoomCount == iRoomMax - 1)
		//{
		//	//idRoom = 2;
		//	//bRoomFounded = true;
		//}

		TRoom * pRoomPattern = 0;
		if (iRoomCount >= iRoomMax)
		{
			pRoomPattern = FindRoomToConnect(eDoorExit, EROOMTYPE_EXIT);
		}
		else
		{
			pRoomPattern = FindRoomToConnect(eDoorExit, EROOMTYPE_STANDARD);
		}

		if (pRoomPattern == 0)
			pRoomPattern = GetRandomRoomOfType(EROOMTYPE_EXIT);

		// ******** Calcul de la position de la salle en fonction de la porte d'entrée     ******* /////

		EDoor eDoorEntrance = GetOppositeDoor(eDoorExit);

		pMap->m_pRooms.push_back(new TRoom(*pRoomPattern));

		TRoom * pRoom = pMap->m_pRooms[pMap->m_pRooms.size() - 1];

		pLastRoom->m_tEntrances[eDoorExit].tDestRoom = pRoom;
		pRoom->m_tEntrances[eDoorEntrance].tDestRoom = pLastRoom;

		pRoom->Translate(tLastDoorPos - pRoom->m_tEntrances[eDoorEntrance].m_tPos);

		// ****** Détermine si l'on se trouve dans un cul de sac **** //

		//int bAvailableDirection[4];

		vector<EDoor> eDoorAvailalble;

		for (int i = 0; i< 4; ++i)
		{
			if (pRoom->m_tEntrances[i].bExist && i != (int)eDoorEntrance)
			{
				eDoorAvailalble.push_back((EDoor)i);
			}
		}

		int iRandomDirection = 0;
		if (eDoorAvailalble.size()>0)
		{
			iRandomDirection = GfxMathGetRandomInteger(0, eDoorAvailalble.size() - 1);
		}
		else
		{
			bAbleToPlace = false;
		}

		eDoorExit = (eDoorAvailalble[iRandomDirection]);
		tLastDoorPos = pRoom->m_tPos + pRoom->m_tEntrances[eDoorExit].m_tPos;

		pLastRoom = pRoom;

		iRoomCount++;
		pMap->m_iRoomsCount++;
	}

	// ************* Fermeture des portes ************ //
	pMap->PlaceDoors(EROOMTYPE_STANDARD);


	//  ************** Creation de la map dans le world  ******** //

	//		int iValue = MAP_MAX_H * MAP_MAX_W;

	TGfxVec2 tWorldSpawn = TGfxVec2(pMap->m_tPlayerSpawn.x + tStartRoom.x, -(pMap->m_tPlayerSpawn.y + tStartRoom.y));


	// ********* Setting des trigger ********************* //
	int iTriggerCount = 0;
	for (unsigned int iRoom = 0; iRoom < pMap->m_pRooms.size(); ++iRoom)
	{
		for (int iTrigger = 0; iTrigger < pMap->m_pRooms[iRoom]->m_iTriggerCount; ++iTrigger)
		{
			TTrigger * pTrigger = pMap->m_pRooms[iRoom]->m_pTriggers[iTrigger];
			if (pTrigger != 0 && pTrigger->m_eTriggerType == TTrigger::ETRIGGER_LOADER)
			{
				pTrigger->m_iMapId = iTriggerCount;
				iTriggerCount++;
			}
		}
	}

	pMap->m_tPlayerSpawn = tWorldSpawn;
	return pMap;
	//g_pGame->m_pWorld->m_pPlayer->m_pTarget = 0;
	//g_pGame->m_pWorld->m_pPlayer->Rotation(PI);
	//	g_pGame->m_pWorld->ClearWorld();


}



void TMapGenerator::ImportGroundCollider( string sFileName)
{

	lua_State * LParseObjState = luaL_newstate();
	luaL_openlibs(LParseObjState);
	lua_pushcfunction(LParseObjState, ObjDump);
	lua_setglobal(LParseObjState, "ObjDump");
	LuaDoFile(LParseObjState, "Data/Maps/parseobj.lua");

	//lua_pushinteger(LParseObjState, pRoom->id);
	lua_pushlstring(LParseObjState, sFileName.c_str(), sFileName.size() );
	LuaCallGlobal(LParseObjState, "ParseGround");

	lua_close(LParseObjState);

}


int ObjDump(lua_State * L)
{
#if FLIB_PLATFORM == FLIB_PLATFORM_FRESH
	const int iVertexCount = lua_objlen(L, 1);
	const int iTriangleCount = lua_objlen(L, 2);
#else
	const int iVertexCount = lua_rawlen(L, 1);
	const int iTriangleCount = lua_rawlen(L, 2);
#endif

	GfxDbgPrintf("%d %d\n", iVertexCount, iTriangleCount);

	float * pVertices = new float[iVertexCount * 3];
	float * pVertex = pVertices;

	for (int i = 0; i < iVertexCount; ++i)
	{
		lua_pushnumber(L, i + 1.f);
		lua_gettable(L, 1);

		lua_pushnumber(L, 1);
		lua_gettable(L, 3);
		lua_pushnumber(L, 2);
		lua_gettable(L, 3);
		lua_pushnumber(L, 3);
		lua_gettable(L, 3);

		*pVertex++ = float(lua_tonumber(L, -3));
		*pVertex++ = float(lua_tonumber(L, -2));
		*pVertex++ = float(lua_tonumber(L, -1));

		lua_pop(L, 4);
	}

	int * pIndices = new int[iTriangleCount * 3];
	int * pIndex = pIndices;

	for (int i = 0; i < iTriangleCount; ++i)
	{
		lua_pushnumber(L, i + 1.f);
		lua_gettable(L, 2);

		lua_pushnumber(L, 1);
		lua_gettable(L, 3);
		lua_pushnumber(L, 2);
		lua_gettable(L, 3);
		lua_pushnumber(L, 3);
		lua_gettable(L, 3);

		*pIndex++ = lua_tointeger(L, -3) - 1;
		*pIndex++ = lua_tointeger(L, -2) - 1;
		*pIndex++ = lua_tointeger(L, -1) - 1;

		lua_pop(L, 4);
	}

	int iLastRoomID = g_pGame->m_pWorld->m_pMapGenerator->m_iRoomPatternsCount-1;
	GfxDbgPrintf("Collision for Room ID : %d \n", iLastRoomID);
	g_pGame->m_pWorld->m_pMapGenerator->m_pRoomPatterns[iLastRoomID]->m_tGround.m_iTriangleCount = iTriangleCount;	
	g_pGame->m_pWorld->m_pMapGenerator->m_pRoomPatterns[iLastRoomID]->m_tGround.m_pVertices = pVertices;
	g_pGame->m_pWorld->m_pMapGenerator->m_pRoomPatterns[iLastRoomID]->m_tGround.m_pIndices = pIndices;

	//g_tGround.m_iTriangleCount = iTriangleCount;
	return 0;
}



EDoor TMapGenerator::GetOppositeDoor(EDoor eDoor){
	switch (eDoor)
	{
	case EDOOR_LEFT: return EDOOR_RIGHT;
	case EDOOR_RIGHT: return EDOOR_LEFT;
	case EDOOR_TOP: return EDOOR_BOTTOM;
	case EDOOR_BOTTOM: return EDOOR_TOP;
	default: return EDOOR_LEFT;
	}
}

vector < TRoom * > TMapGenerator::GetAllRoomOfType(ERoomType eType)
{
	vector<TRoom *> tRooms;

	for (int i = 0; i < m_iRoomPatternsCount; ++i)
	{
		if (m_pRoomPatterns[i] != 0)
		{
			if (m_pRoomPatterns[i]->m_eType == eType)
			{
				tRooms.push_back(m_pRoomPatterns[i]);
			}
		}
	}
	if (tRooms.size() == 0)
	{
		GfxDbgAssert(" No Room of this type");
	}

	return tRooms;
}

TRoom * TMapGenerator::GetRandomRoomOfType(ERoomType eType)
{
	vector<TRoom *> tRooms;

	for (int i = 0; i < m_iRoomPatternsCount; ++i)
	{
		if (m_pRoomPatterns[i] != 0)
		{
			if (m_pRoomPatterns[i]->m_eType == eType)
			{
				tRooms.push_back(m_pRoomPatterns[i]);
			}
		}
	}

	if (tRooms.size() == 0) return 0;
	if (tRooms.size() == 1) return tRooms[0];

	int iRand = GfxMathGetRandomInteger(0, tRooms.size() - 1);

	return tRooms[iRand];
}

TRoom * TMapGenerator::FindRoomToConnect(EDoor eDoor, ERoomType eType)
{
	vector<TRoom *> tRooms;
	TRoom * pPattern = 0;
	for (int i = 0; i < m_iRoomPatternsCount; ++i)
	{
		pPattern = m_pRoomPatterns[i];
		if (pPattern != 0)
		{
			if (pPattern->m_eType == eType)
			{
				bool bDoorAvailable = false;

				switch (eDoor)
				{
				case EDOOR_LEFT: if (pPattern->m_tEntrances[EDOOR_RIGHT].bExist)  bDoorAvailable = true;
					break;
				case EDOOR_RIGHT: if (pPattern->m_tEntrances[EDOOR_LEFT].bExist)   bDoorAvailable = true;
					break;
				case EDOOR_TOP: if (pPattern->m_tEntrances[EDOOR_BOTTOM].bExist) bDoorAvailable = true;
					break;
				case EDOOR_BOTTOM: if (pPattern->m_tEntrances[EDOOR_TOP].bExist)    bDoorAvailable = true;
					break;
				default:
					break;
				}

				if (bDoorAvailable){
					tRooms.push_back(m_pRoomPatterns[i]);
				}
			}
		}
	}

	if (tRooms.size() == 0) return 0;
	if (tRooms.size() == 1) return tRooms[0];

	int iRand = GfxMathGetRandomInteger(0, tRooms.size() - 1);

	return tRooms[iRand];
}
