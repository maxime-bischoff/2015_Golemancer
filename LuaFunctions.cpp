#include "Const.h"
extern TGame * g_pGame;
#ifdef TFA_FRESH


void InitLuaFunctions()
{

}

void LuaCreateWall( TGfxVec2 tPos, float fRadiusX, float fRadiusZ, float fAngle)
{
	lua_State * L = GfxFreshLuaState();

	lua_pushnumber(L, tPos.x);
	lua_pushnumber(L, tPos.y);
	lua_pushnumber(L, fRadiusX);
	lua_pushnumber(L, fRadiusZ);
	lua_pushnumber(L, fAngle);
	LuaCallGlobal(L, "CreateWall");

}
void LuaDestroyWall(int iIndex)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushinteger(L, iIndex);
	LuaCallGlobal(L, "DestroyWall"); 
}

void LuaCreateDestructible ( int iIndex, TGfxVec2 tPos )
{
	lua_State * L = GfxFreshLuaState() ; 

	lua_pushinteger(L, iIndex); 
	lua_pushnumber ( L, tPos.x );
	lua_pushnumber ( L, tPos.y );
	LuaCallGlobal ( L, "CreateDestructible"); 
}


void LuaDestroyDestructible ( int iIndex )
{
	lua_State * L = GfxFreshLuaState() ; 

	lua_pushinteger( L, iIndex ); 
	LuaCallGlobal( L, "DestroyDestructible") ; 
}

void LuaCreateCollectible ( int iIndex , TGfxVec2 tPos, float fHeight )
{
	lua_State * L = GfxFreshLuaState() ; 
	lua_pushinteger(L, iIndex);
	lua_pushnumber ( L, tPos.x );
	lua_pushnumber ( L, tPos.y );
	lua_pushnumber ( L, fHeight );
	LuaCallGlobal(L, "CreateCollectible"); 
}

void LuaUpdateCollectible ( int iIndex, TGfxVec2 tPos, float fHeight )
{
	lua_State * L = GfxFreshLuaState() ; 
	lua_pushinteger(L, iIndex);
	lua_pushnumber ( L, tPos.x );
	lua_pushnumber ( L, tPos.y );
	lua_pushnumber(L, fHeight);
	LuaCallGlobal ( L, "UpdateCollectible");	
}

void LuaDestroyCollectible ( int iIndex, int iCount )
{
	lua_State * L = GfxFreshLuaState() ; 
	lua_pushinteger(L, iIndex);
	lua_pushinteger(L, iCount);
	LuaCallGlobal( L, "DestroyCollectible") ; 
}


void LuaCreateTrigger(int iIndex, TGfxVec2 tPos)
{
	lua_State * L = GfxFreshLuaState();

	lua_pushnumber(L, tPos.x);
	lua_pushnumber(L, tPos.y);
	LuaCallGlobal(L, "CreateTrigger");
}

void LuaDestroyTrigger(int iIndex)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushinteger(L, iIndex);
	LuaCallGlobal(L, "DestroyTrigger");
}


void LuaCreateEnemy(int iIndex, TGfxVec2 tPos, float fRadius, float fAngle, EEnemyType eType, float fHeight)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushinteger(L, iIndex);
	lua_pushnumber(L, tPos.x);
	lua_pushnumber(L, tPos.y);
	lua_pushnumber(L, fRadius);
	lua_pushnumber(L, fAngle);
	lua_pushinteger(L, (int)eType);
	lua_pushnumber(L, fHeight );
	LuaCallGlobal(L, "CreateEnemy");

}

void LuaDestroyEnemy(int iIndex, int iCount)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushinteger(L, iIndex);
	lua_pushinteger(L, iCount);
	LuaCallGlobal(L, "DestroyEnemy");
}

void LuaUpdateEnemy(int iIndex, const TGfxVec2 & tPos, float fAngle, int iState, const TGfxVec3 & tColor, float fHeight)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushinteger(L, iIndex);
	lua_pushnumber(L, tPos.x);
	lua_pushnumber(L, tPos.y);
	lua_pushnumber(L, fAngle);

	lua_pushinteger(L, iState);

	lua_pushnumber(L, tColor.x);
	lua_pushnumber(L, tColor.y);
	lua_pushnumber(L, tColor.z);

	lua_pushnumber( L, fHeight);
	LuaCallGlobal(L, "UpdateEnemy");

}

void LuaEnlargeYourTarget(int iIndex, float fRadius)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushinteger(L, iIndex);
	//lua_pushnumber(L, fRadius);

	LuaCallGlobal(L, "Promote");
}

void LuaUpdateCamera()
{
	
	TCamera & tCam =  g_pGame->m_pWorld->m_pPlayer->m_tCam;
	

	lua_State * L = GfxFreshLuaState();

	lua_pushnumber(L, tCam.Position().x);
	lua_pushnumber(L, tCam.GetHeight() + g_pGame->m_pWorld->m_pPlayer->m_fHeight - tCam.tCurrentParam.fOffsetY);
	lua_pushnumber(L, tCam.Position().y);
	lua_pushnumber(L, tCam.GetFOV());
	lua_pushnumber(L, tCam.m_tLookat.x);
	lua_pushnumber(L, tCam.GetLookAtHeight() + g_pGame->m_pWorld->m_pPlayer->m_fHeight - tCam.tCurrentParam.fOffsetY);
	lua_pushnumber(L, tCam.m_tLookat.y );
	lua_pushnumber(L, tCam.GetDistance());
	lua_pushnumber(L, tCam.tCurrentParam.fLateralOffset);


	LuaCallGlobal(L, "UpdateCamera");

}

void LuaUpdatePlayer(TGfxVec2 tPos, float fAngle,float fHeight, int AnimState)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushnumber(L, tPos.x);
	lua_pushnumber(L, tPos.y);
	lua_pushnumber(L, fHeight);
	lua_pushnumber(L, fAngle);
	lua_pushinteger(L, AnimState);
	LuaCallGlobal(L, "UpdatePlayer");

}
void LuaUpdateFocus(TGfxVec2 tPos, float fAngle, int iState)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushnumber(L, tPos.x);
	lua_pushnumber(L, tPos.y);
	lua_pushnumber(L, fAngle);
	lua_pushinteger(L, iState);
	lua_pushnumber(L, g_pGame->m_pWorld->m_pPlayer->m_fHeight);
	LuaCallGlobal(L, "UpdateFocus");
	
}

void LuaUpdateTarget(TGfxVec2 tPos, bool bVisible)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushnumber(L, tPos.x);
	lua_pushnumber(L, tPos.y);
	lua_pushboolean( L, bVisible);
	LuaCallGlobal(L, "UpdateTarget");

}
void LuaUpdateShield( float fValue )
{
	lua_State * L = GfxFreshLuaState();
	lua_pushnumber(L, fValue);

	LuaCallGlobal(L, "UpdateShield");

}

void LuaCreateDamage(int iIndex, TGfxVec2 tPos, float fRadiusX, float fRadiusZ, float fAngle, bool bActive, EDamageForm eform, float fHeight ,int iAspect)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushinteger(L, iIndex );
	lua_pushnumber(L, tPos.x);
	lua_pushnumber(L, tPos.y);
	lua_pushnumber(L, fHeight);
	lua_pushnumber(L, fRadiusX);
	lua_pushnumber(L, fRadiusZ);
	lua_pushnumber(L, fAngle);
	lua_pushboolean(L, bActive);
	lua_pushinteger(L, (int)eform);
	lua_pushinteger(L, iAspect);
	//GfxDbgPrintf ( "valeur index CREATE %d \n", iIndex ) ; 

	LuaCallGlobal(L, "CreateDamage");

}

void LuaDestroyDamage(int iIndex, int iCount )
{
	lua_State * L = GfxFreshLuaState();
	lua_pushinteger(L, iIndex  );
	lua_pushinteger(L, iCount);

	LuaCallGlobal(L, "DestroyDamage");
}

void LuaUpdateDamage(int iIndex, TGfxVec2 tPos, TGfxVec2 tScale, float fAngle, bool bActive, int iAspect)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushinteger(L, iIndex);
	lua_pushnumber(L, tPos.x);
	lua_pushnumber(L, tPos.y);
	lua_pushnumber(L, tScale.x);
	lua_pushnumber(L, tScale.y);
	lua_pushnumber(L, fAngle);
	lua_pushboolean(L, bActive);
	lua_pushinteger(L, iAspect);
	LuaCallGlobal(L, "UpdateDamage");
}

void LuaCreateImpactParticles( float fPosX, float fPosY, float fPosZ)
{
	lua_State * L = GfxFreshLuaState();

	lua_pushnumber(L, fPosX);
	lua_pushnumber(L, fPosY);
	lua_pushnumber(L, fPosZ);	
	LuaCallGlobal(L, "CreateImpactParticles");




}

void LuaCreateBoss(TBoss * pBoss)
{
	lua_State * L = GfxFreshLuaState();

	lua_pushnumber(L, pBoss->Position().x);
	lua_pushnumber(L, pBoss->Position().y);
	lua_pushnumber(L, pBoss->m_fHeight);
	lua_pushnumber(L, pBoss->GetLeftHandPosition().x);
	lua_pushnumber(L, pBoss->GetLeftHandPosition().y);
	lua_pushnumber(L, pBoss->GetRightHandPosition().x);
	lua_pushnumber(L, pBoss->GetRightHandPosition().y);	
	LuaCallGlobal(L, "CreateBoss");
}

void LuaUpdateBoss(TBoss * pBoss)
{
	lua_State * L = GfxFreshLuaState();

	lua_pushnumber(L, pBoss->Position().x);
	lua_pushnumber(L, pBoss->Position().y);
	lua_pushnumber(L, pBoss->m_fHeight);
	lua_pushnumber(L, pBoss->Rotation() * RadToDeg);
	lua_pushnumber(L, pBoss->GetLeftHandPosition().x);
	lua_pushnumber(L, pBoss->GetLeftHandPosition().y);
	lua_pushnumber(L, pBoss->GetRightHandPosition().x);
	lua_pushnumber(L, pBoss->GetRightHandPosition().y);
	lua_pushinteger(L, pBoss->GetPhase()); 
	lua_pushinteger(L, pBoss->GetRightHandState());
	lua_pushinteger(L, pBoss->GetLeftHandState());
	LuaCallGlobal(L, "UpdateBoss");
}

void LuaDestroyBoss()
{
	lua_State * L = GfxFreshLuaState();
	LuaCallGlobal(L, "DestroyBoss");
}
// :: Draw Hud :: // 

void LuaDrawRuneWheel(TGfxVec2 tPosCenter, TGfxVec2 tPosCursor, bool bVisible)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushnumber(L, tPosCenter.x);
	lua_pushnumber(L, tPosCenter.y);
	lua_pushnumber(L, tPosCursor.x);
	lua_pushnumber(L, tPosCursor.y);
	lua_pushboolean(L, bVisible); 
	LuaCallGlobal(L, "DrawRuneWheel");
}

void LuaDrawPointWheel(TGfxVec2 tPos, int iIndex )
{
	lua_State * L = GfxFreshLuaState();
	lua_pushnumber(L, tPos.x);
	lua_pushnumber(L, tPos.y);
	lua_pushinteger(L, iIndex);
	LuaCallGlobal(L, "DrawPointWheel");
}

void LuaInitLine(struct TLineFresh tDataLineFresh, int iIndex)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushnumber(L, tDataLineFresh.m_fScale ) ; 
	lua_pushnumber(L, tDataLineFresh.m_fAngle ) ;
	lua_pushinteger(L, iIndex);
	LuaCallGlobal(L, "InitLineWheel");
}

void LuaUpdateLine(struct TLineFresh tDataLineFresh, int iIndex)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushnumber(L, tDataLineFresh.m_tPos.x );
	lua_pushnumber(L, tDataLineFresh.m_tPos.y);
	lua_pushinteger(L, iIndex);
	LuaCallGlobal(L, "DrawLineWheel");

}
void LuaClearRune ( int iCount ) 
{
	lua_State * L = GfxFreshLuaState();
	lua_pushinteger(L, iCount);
	LuaCallGlobal(L, "ClearRune");
}
void LuaDrawHudLife ( float fLifeScale, float fEnergyScale ) 
{
	lua_State * L = GfxFreshLuaState();
	lua_pushnumber(L, fLifeScale); 
	lua_pushnumber(L, fEnergyScale);
	LuaCallGlobal(L, "DrawLife");
}

void LuaDrawMana (  int iCountTotal, bool bScale  ) 
{
	lua_State * L = GfxFreshLuaState();
	lua_pushinteger(L, iCountTotal);
	lua_pushboolean (L, bScale ); 
	LuaCallGlobal(L, "DrawMana");
}

void LuaDrawCharge ( float fScale ) 
{
	lua_State * L = GfxFreshLuaState();
	lua_pushnumber(L, fScale ); 
	LuaCallGlobal(L, "DrawCharge");
}

void LuaSetMissionText( char* cString)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushstring(L, cString);
	LuaCallGlobal(L, "SetMissionText");
}

// :: Environement :: // 


void LuaCreateTriggerWall(int iIndex, TGfxVec2 tPos, TGfxVec2 tRadius)
{
	lua_State * L = GfxFreshLuaState();
	lua_pushinteger(L, iIndex);
	lua_pushnumber(L, tPos.x);
	lua_pushnumber(L, tPos.y);
	lua_pushnumber(L, tRadius.x);
	lua_pushnumber(L, tRadius.y);
	LuaCallGlobal(L, "CreateTriggerWall");

}

void LuaUpdateEnvi()
{

}


void LuaScreenShake(float fStrenght, float fStabilisationSpeed)
{
	lua_pushnumber(GfxFreshLuaState(), fStrenght);
	lua_pushnumber(GfxFreshLuaState(), fStabilisationSpeed);
	LuaCallGlobal(GfxFreshLuaState(), "ShakeScreen");
}


void LuaScreenShake( TGfxVec2 fEpiCenter, TGfxVec2 fObserver, float fStrenght, float fStabilisationSpeed)
{
	float fFullImpactEffect = 4.0f;
	float fNullImpactEffect = 12.0f;
	float fImpactFallOff = 1.0f;
	float fImpactDistance =  (fEpiCenter - fObserver).Length();

	float fAttenuation = Attenuate(fFullImpactEffect, fNullImpactEffect,fImpactDistance, fImpactFallOff);

	lua_pushnumber(GfxFreshLuaState(), fStrenght * fAttenuation);
	lua_pushnumber(GfxFreshLuaState(), fStabilisationSpeed);
	LuaCallGlobal(GfxFreshLuaState(), "ShakeScreen");

}

void LuaShowHud(bool bShow)
{

	lua_pushboolean(GfxFreshLuaState(), bShow);
	LuaCallGlobal(GfxFreshLuaState(), "ShowHUD");


}



// ************* Controles de camera sur clavier sous fresh ********* //
/*
void C_FovUp(lua_State * L){
	g_pGame->m_pWorld->m_tCamFresh.fFOV += TCamera::FOV_CHANGE_SPEED * UnscaleDeltaTime();

	if (g_pGame->m_pWorld->m_tCamFresh.fFOV > TCamera::FOV_MAX)
		g_pGame->m_pWorld->m_tCamFresh.fFOV = TCamera::FOV_MAX;
	
}
void C_FovDown(lua_State * L){
	g_pGame->m_pWorld->m_tCamFresh.fFOV -= TCamera::FOV_CHANGE_SPEED * UnscaleDeltaTime();

	if (g_pGame->m_pWorld->m_tCamFresh.fFOV < TCamera::FOV_MIN)
		g_pGame->m_pWorld->m_tCamFresh.fFOV = TCamera::FOV_MIN;
}

void C_DistanceUp(lua_State * L){
	g_pGame->m_pWorld->m_tCamFresh.fDistance -= TCamera::CAMERA_CHANGE_SPEED * UnscaleDeltaTime();
}
void C_DistanceDown(lua_State * L){
	g_pGame->m_pWorld->m_tCamFresh.fDistance -= TCamera::CAMERA_CHANGE_SPEED * UnscaleDeltaTime();
}

void C_CameraUp(lua_State * L){
	g_pGame->m_pWorld->m_tCamFresh.fDistance -= TCamera::CAMERA_CHANGE_SPEED * UnscaleDeltaTime();
}
void C_CameraDown(lua_State * L){
	g_pGame->m_pWorld->m_tCamFresh.fDistance -= TCamera::CAMERA_CHANGE_SPEED * UnscaleDeltaTime();
}

void C_CamTargetUp(lua_State * L){
	g_pGame->m_pWorld->m_tCamFresh.fDistance -= TCamera::CAMERA_CHANGE_SPEED * UnscaleDeltaTime();
}
void C_CamTargetDown(lua_State * L){
	g_pGame->m_pWorld->m_tCamFresh.fDistance -= TCamera::CAMERA_CHANGE_SPEED * UnscaleDeltaTime();
}

*/

#endif