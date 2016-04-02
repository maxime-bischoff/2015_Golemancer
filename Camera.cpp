#include "Camera.h"
#include "Const.h"

TCamera::TCamera() 
{
	m_eMode				= CAMERA_MODE_TPS;
	m_eState			= CAMERA_STATE_WALK;
	m_eCollisionLayer	= LAYER_CAMERA;
	m_iCollisionFlags.SetEverything();
	m_tLookat = TGfxVec2(0, 0);




	m_tLookat = TGfxVec2(0, 0);

}


TCamera::~TCamera()
{
}

void TCamera::Update()
{
	ParamCamera();

		float fLerpValue = 5 * UnscaleDeltaTime();

		TCameraParameter & tCamParam = GetCurrentStateParameter();
		tCurrentParam.fov = MLerp(tCurrentParam.fov, tCamParam.fov, fLerpValue);
		tCurrentParam.fHeight = MLerp(tCurrentParam.fHeight, tCamParam.fHeight, fLerpValue);
		tCurrentParam.fDistance = MLerp(tCurrentParam.fDistance, tCamParam.fDistance, fLerpValue);
		tCurrentParam.fLateralOffset = MLerp(tCurrentParam.fLateralOffset, tCamParam.fLateralOffset, fLerpValue);
		tCurrentParam.fLookatHeight = MLerp(tCurrentParam.fLookatHeight, tCamParam.fLookatHeight, fLerpValue);

}

void TCamera::AddFov(float fValue)
{
	TCameraParameter & tCamParam = GetCurrentStateParameter();
	tCamParam.fov = MClamp(tCamParam.fov + fValue, (float)FOV_MIN, (float)FOV_MAX);
	tCurrentParam.fov = MClamp(tCurrentParam.fov + fValue, (float)FOV_MIN, (float)FOV_MAX);
}

void TCamera::AddHeight(float fValue)
{
	TCameraParameter & tCamParam = GetCurrentStateParameter();
	tCamParam.fHeight = MClamp(tCamParam.fHeight + fValue, 0.5, 20);
	tCurrentParam.fHeight = MClamp(tCurrentParam.fHeight + fValue, 0.5, 20);
}

void TCamera::AddDistance(float fValue)
{
	TCameraParameter & tCamParam = GetCurrentStateParameter();
	tCamParam.fDistance = MClamp(tCamParam.fDistance + fValue, 0.5f, 10.0f);
	tCurrentParam.fDistance = MClamp(tCurrentParam.fDistance + fValue, 0.5f, 10.0f);
}

void TCamera::AddLookAtHeight(float fValue)
{
	TCameraParameter & tCamParam = GetCurrentStateParameter();
	tCamParam.fLookatHeight = MClamp(tCamParam.fLookatHeight + fValue, 0, 15.0f);
	tCurrentParam.fLookatHeight = MClamp(tCurrentParam.fLookatHeight + fValue, 0, 15.0f);
}

void TCamera::AddLateralOffset(float fValue)
{
	TCameraParameter & tCamParam = GetCurrentStateParameter();
	tCamParam.fLateralOffset = MClamp(tCamParam.fLateralOffset + fValue, -5.0f, 5.0f);
	tCurrentParam.fLateralOffset = MClamp(tCurrentParam.fLateralOffset + fValue, -5.0f, 5.0f);

}


TCameraParameter & TCamera::GetCurrentStateParameter()
{
	if (m_eState == CAMERA_STATE_WALK)
		return tWalkParam;
	else if (m_eState == CAMERA_STATE_FIGHT)
		return tFightParam;
	else if (m_eState == CAMERA_STATE_CAST)
		return tCastParam;
	else return tWalkParam;
}


void TCamera::ParamCamera()
{
	float fFOVChangeSpeed = 50.0f;
	float fChangeSpeed = 1.5f;


#ifndef TFA_FRESH
	if (GfxInputIsPressed(EGfxInputID_KeyPlus))
	{
		AddFov( fFOVChangeSpeed * UnscaleDeltaTime());		
	}

	if (GfxInputIsPressed(EGfxInputID_KeyMinus))
	{
		AddFov(-fFOVChangeSpeed * UnscaleDeltaTime());
	}

	if (GfxInputIsPressed(EGfxInputID_KeyNumPad9))
	{
		 AddHeight( fChangeSpeed * UnscaleDeltaTime());
	}

	if (GfxInputIsPressed(EGfxInputID_KeyNumPad3))
	{
		AddHeight( -fChangeSpeed * UnscaleDeltaTime());
	}

	if (GfxInputIsPressed(EGfxInputID_KeyNumPad8))
	{
		AddDistance( -fChangeSpeed * UnscaleDeltaTime());
	}

	if (GfxInputIsPressed(EGfxInputID_KeyNumPad2))
	{
		AddDistance( fChangeSpeed * UnscaleDeltaTime());
	}

	if (GfxInputIsPressed(EGfxInputID_KeyNumPad7))
	{
		AddLookAtHeight(fChangeSpeed * UnscaleDeltaTime());
	}

	if (GfxInputIsPressed(EGfxInputID_KeyNumPad1))
	{
		AddLookAtHeight( -fChangeSpeed * UnscaleDeltaTime());
	}

	if (GfxInputIsPressed(EGfxInputID_KeyNumPad4))
	{
		AddLateralOffset(fChangeSpeed * UnscaleDeltaTime());
	}

	if (GfxInputIsPressed(EGfxInputID_KeyNumPad6))
	{
		AddLateralOffset(-fChangeSpeed * UnscaleDeltaTime());
	}


#else
	
	LuaCallGlobal(GfxFreshLuaState(), "GetFovState");
	int iFovState = lua_tointeger(GfxFreshLuaState(), 1);
	lua_pop(GfxFreshLuaState(), 1);

	//if (iFovState)GfxDbgPrintf("True"); else GfxDbgPrintf("false");

	
	if (iFovState == 1)
	{
		AddFov( fFOVChangeSpeed * UnscaleDeltaTime());	
	}
	else if (iFovState == -1)
	{
		AddFov( -fFOVChangeSpeed * UnscaleDeltaTime());	;
	}



	LuaCallGlobal(GfxFreshLuaState(), "GetDistanceState");
	int iDistState = lua_tointeger(GfxFreshLuaState(), 1);
	lua_pop(GfxFreshLuaState(), 1);
	if (iDistState == 1)
	{		
		AddHeight( fChangeSpeed * UnscaleDeltaTime());
	}
	else if (iDistState == -1)
	{		
		AddHeight( -fChangeSpeed * UnscaleDeltaTime());
	}

	LuaCallGlobal(GfxFreshLuaState(), "GetCameraHeightState");
	int iHeightState = lua_tointeger(GfxFreshLuaState(), 1);
	lua_pop(GfxFreshLuaState(), 1);
	if (iHeightState == 1)
	{
		AddDistance( -fChangeSpeed * UnscaleDeltaTime());
	}
	else if (iHeightState == -1)
	{
		AddDistance( fChangeSpeed * UnscaleDeltaTime());
	}

	LuaCallGlobal(GfxFreshLuaState(), "GetTargetHeightState");
	int iTargetHeightState = lua_tointeger(GfxFreshLuaState(), 1);
	lua_pop(GfxFreshLuaState(), 1);
	if (iTargetHeightState == 1)
	{
		AddLookAtHeight( fChangeSpeed * UnscaleDeltaTime());
	}
	else if (iTargetHeightState == -1)
	{
		AddLookAtHeight( -fChangeSpeed * UnscaleDeltaTime());
	}

	LuaCallGlobal(GfxFreshLuaState(), "GetLateralOffsetState");
	int iLateralOffsetState = lua_tointeger(GfxFreshLuaState(), 1);
	lua_pop(GfxFreshLuaState(), 1);
	if (iLateralOffsetState == 1)
	{
		AddLateralOffset(fChangeSpeed * UnscaleDeltaTime());
	}
	else if (iLateralOffsetState == -1)
	{
		AddLateralOffset(-fChangeSpeed * UnscaleDeltaTime());
	}


	


#endif



}




