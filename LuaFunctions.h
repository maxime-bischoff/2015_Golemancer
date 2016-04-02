#pragma once

#ifdef TFA_FRESH


void InitLuaFunctions();
//---- World Management Functions ----//
void LuaCreateWall(TGfxVec2 tPos, float fRadiusX, float fRadiusZ, float fAngle);

void LuaDestroyWall(int iIndex);

void LuaCreateDestructible ( int iIndex, TGfxVec2 tPos );

void LuaDestroyDestructible ( int iIndex ) ; 

void LuaCreateCollectible ( int iIndex , TGfxVec2 tPos, float fHeight = 0);

void LuaUpdateCollectible ( int iIndex, TGfxVec2 tPos, float fHeight );

void LuaDestroyCollectible ( int iIndex, int iCount ) ; 


void LuaCreateTrigger(int iIndex, TGfxVec2 tPos);

void LuaDestroyTrigger(int iIndex);

//---- Dynamics Objects Functions----//
void LuaCreateEnemy(int iIndex, TGfxVec2 tPos, float fRadiusX, float fAngle, enum EEnemyType eType, float fHeight );

void LuaDestroyEnemy(int iIndex, int iCount );

void LuaUpdateEnemy(int iIndex, const  TGfxVec2 &  tPos, float fAngle, int iState, const TGfxVec3 & tColor, float fHeight = 0);

void LuaEnlargeYourTarget(int iIndex, float fRadius);

void LuaUpdateCamera();

void LuaUpdatePlayer(TGfxVec2 tPos, float fAngle, float fHeight, int AnimState);

void LuaUpdateFocus(TGfxVec2 tPos,float fAngle, int iState);

void LuaUpdateTarget(TGfxVec2 tPos, bool bVisible);

void LuaUpdateShield(float fValue);

void LuaCreateDamage(int iIndex, TGfxVec2 tPos, float fRadiusX, float fRadiusZ, float fAngle, bool bActive, enum EDamageForm eform, float m_fHeight = 0 ,int iAspect = 0);

void LuaUpdateDamage(int iIndex, TGfxVec2 tPos, TGfxVec2 tScale, float fAngle, bool bActive, int iAspect = 0);

void LuaCreateImpactParticles( float fPosX, float fPosY, float fPosZ);

void LuaDestroyDamage(int iIndex, int iCount );

void LuaCreateBoss(class TBoss * pBoss);

void LuaUpdateBoss(class TBoss * pBoss);

void LuaDestroyBoss();

// :: Draw Hud :: // 

void LuaDrawRuneWheel ( TGfxVec2 tPosCenter ,TGfxVec2 tPosCursor,  bool bVisible ) ; 

void LuaDrawPointWheel(TGfxVec2 tPos , int iIndex );

void LuaInitLine ( struct TLineFresh tDataLineFresh, int iIndex  ) ; 

void LuaUpdateLine(struct TLineFresh tDataLineFresh, int iIndex);

void LuaClearRune ( int iCount ) ; 

void LuaDrawHudLife(float fLifeScale, float fEnergyScale);

void LuaDrawMana ( int iCountTotal, bool bScale  ) ; 

void LuaDrawCharge ( float fScale ) ; 

void LuaSetMissionText(char *);

// :: Environement :: // 
void LuaCreateEnvi(enum Type_Environment bType,  TGfxVec2 tPos, TGfxVec2 tRadius);

void LuaCreateTriggerWall( int iIndex, TGfxVec2 tPos, TGfxVec2 tRadius);

void LuaUpdateEnvi( ); 

void LuaScreenShake(float fStrenght = 0.09, float fStabilisationSpeed = 0.2);

void LuaScreenShake( TGfxVec2 fEpiCenter, TGfxVec2 fObserver, float fStrenght = 0.09, float fStabilisationSpeed = 0.2 );

void LuaShowHud(bool bShow);
// ************* Controles de camera sur clavier sous fresh ********* //
/*
void C_FovUp(lua_State * L);
void C_FovDown(lua_State * L);

void C_DistanceUp(lua_State * L);
void C_DistanceDown(lua_State * L);

void C_CameraUp(lua_State * L);
void C_CameraDown(lua_State * L);

void C_CamTargetUp(lua_State * L);
void C_CamTargetDown(lua_State * L);
*/

#endif