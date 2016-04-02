#pragma once
#include "Transform.h"

struct TCameraParameter
{
	TCameraParameter() : fov(90), fHeight(2.0f), fDistance(2.0f), fLateralOffset(1.0f), fLookatHeight(1.5f){}
	float fov;
	float fHeight;
	float fDistance;
	float fLateralOffset;
	float fLookatHeight;
	float fMaxHeight;
	float fMinHeight;
	float fOffsetY;
};

enum ECameraMode{
	CAMERA_MODE_TOPDOWN,
	CAMERA_MODE_TPS
};

enum ECameraState{
	CAMERA_STATE_WALK,
	CAMERA_STATE_FIGHT,
	CAMERA_STATE_CAST
};

class TCamera :
	public TTransform
{
public:	
		float fBackRadius;
		float fSideRadius;		

		TGfxVec2 m_tLookat;	

		ECameraState m_eState;
		ECameraMode m_eMode;
	
		TCameraParameter tWalkParam;
		TCameraParameter tFightParam;
		TCameraParameter tCastParam;
		TCameraParameter tCurrentParam;

		float GetFOV(){ return tCurrentParam.fov; }
		float GetHeight(){ return tCurrentParam.fHeight; }
		float GetDistance(){ return tCurrentParam.fDistance  ; }
		float GetLookAtHeight(){ return tCurrentParam.fLookatHeight ; }

		void AddFov( float fValue );
		void AddHeight( float fValue );
		void AddDistance( float fValue );
		void AddLookAtHeight( float fValue );
		void AddLateralOffset( float fValue );

	
		TCameraParameter & GetCurrentStateParameter();

		static const int FOV_MAX = 110;
		static const int FOV_MIN = 50;
		static const int FOV_CHANGE_SPEED = 50;

		bool bIsColliding;

	TCamera();
	virtual ~TCamera();

	void Update();
	void ParamCamera();
};

