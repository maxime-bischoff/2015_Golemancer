#pragma once
#include "Transform.h"
#include "Camera.h"
const float PLAYERMAXENERGY = 15.f;
const int PLAYERMAXHEALTH = 5;
const int MAX_SHIELD = 6;
const float PLAYER_INVUL_FRAME = 0.3f;
const float PLAYER_RUN_SPEED = 5.0f;

enum EPlayerMode{
	PLAYER_MODE_COMBAT,
	PLAYER_MODE_WALK
};

enum EMovementAnimState
{
	MOVEMENT_ANIM_STATE_IDLE = 0,
	MOVEMENT_ANIM_STATE_WALK,
	MOVEMENT_ANIM_STATE_RUN,
	MOVEMENT_ANIM_STATE_DODGE,
	MOVEMENT_ANIM_STATE_DEAD,
	MOVEMENT_ANIM_STATE_TELEPORT
};

enum EPlayerState{
	PLAYER_STATE_ACTIVE,
	PLAYER_STATE_STUNNED,
	PLAYER_STATE_TELEPORTING
};

enum ELockState{
	LOCK_LOCKING,
	LOCK_LOCKED,
	LOCK_UNLOCKED
};

class TPlayer :
	public TTransform
{
public:
	// :: PUBLIC

	// -- ARRAY

	// -- VARIABLES

	TEnemy
		* m_pTarget;

	TSpellManager
		* m_pSpellManager;

	EPlayerMode
		m_ePlayerMode;

	EPlayerState
		m_ePlayerState;

	EMovementAnimState
		m_eMovementAnimState;

	EPlayerMode
		m_eMoveMod;

	ELockState m_eLockState;

	TCamera
		m_tCam;

	TGfxVec2 m_tTeleportPosition;
	float m_fTeleportCountDown;	
	EPostFadeAction m_ePostFadeTeleport;
	int m_iPostFadeParameter;


	float
		m_fCamRotationDisabled,
		m_fCamTimeRotationDisabled,
		m_fCamRotationSpeed,
		m_fCamMaxRotationSpeed,
		m_fCamRotationAccel,
		m_fCamRotationDecel,
		m_fCamAngleAroundPlayer,
		m_fMoney,
		m_fRadius,
		m_fRunSpeed,
		m_fTimeRotationLocked,
		m_fStickAngleLocked,
		m_fMovingAngle,
		m_fRotationSpeed,
		m_fTargetAngle, // The angle the player while try to reach
		m_fCountDown,
		m_fStunDelay,
		m_fInvulCountDown,
		m_fHeight,
		m_fLockFov,
		m_fDodgeTimer;

	int
		m_iEnergy,
		m_iHealthPoint,
		m_iShieldPoint;
	bool
		m_bCameraAngleLocked,
		m_bCameraLocked;

	TGfxVec2 m_tMoveDirection, m_tLastDirection;
	// -- FUNCTIONS
	TPlayer();
	virtual ~TPlayer();

	void
		Update(),
		Render(),
		Respawn(),
		Damage(int iDamage, bool bForce = false),
		Stun(float fDelay),
		Teleport(const TGfxVec2 & tPos, float fAngle),
		CollideWall(TCollideResult &tResult),
		OnDead();

	void UpdateTeleport();

	bool
		IsDead()
		{ if( m_iHealthPoint <= 0) return true; else return false;}

private :
	// :: PRIVATE :: //

	// -- VARIABLES
	float
		m_fLeftStickX,
		m_fLeftStickY,
		m_fAngleStick;

	// -- FUNCTIONS
	void
		InitCamera(),
		SmartLock( void ),
		CollideCamera( void ),
		LockLeftEnemy( void ),
		LockRightEnemy( void ),
		TopDownCameraManagement( void ),
		OldCameraManagement( void ),
		TPSCameraManagement( void ),
		ChangeControlToKeyboard( void );
	bool
		IsObstructed(const TGfxVec2 & tSrc, const TGfxVec2 & tDest, TGfxVec2 & tNearest);
	vector<TEnemy *>
		GetEnemiesInFov(float fFov);
};
