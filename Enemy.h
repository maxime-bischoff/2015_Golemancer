#pragma once
#include "Transform.h"
const float ENEMY_INVUL_FRAME = 0.6f;
enum EBrainMode
{
	EBrainMode_Chasing,
	EBrainMode_Attacking,
	EBrainMode_Slacking,
	EBrainMode_RunningAway,
	EBrainMode_Static,
	EBrainMode_Repulsing,
};

enum EEnemyType
{
	EEnemyType_RangedFast,
	EEnemyType_StrongAndSlow,
	EEnemyType_Grunt,
	EEnemyType_Vegetable,
	EEnemyTpe_StrongAndStun,
	EEnemyType_Boss
};

class TEnemy :
	public TTransform
{
	// :: PUBLIC :: //
public:
	TEnemy(TGfxVec2 tPos = TGfxVec2(0, 0), int iIDRoom = 1, EEnemyType eType = EEnemyType_StrongAndSlow);
	virtual ~TEnemy();

	TGfxVec3
		m_tColor,
		m_tDamagedColor,
		m_tBaseColor;

	TGfxVec2
		m_tTempTarget;

	float
		m_fActionCoolDown,
		m_fDisintegrationPower,

		m_fDesiredAngle,
		m_fTimeStatic,

		m_fInvulCountdown,
		m_fHeight;

	float m_fFresnelStrength;
	float m_fFresnelWidth;
	float m_fDamagedFresnelStrength;
	float m_fDamagedFresnelWidth;
	float m_fStandardFresnelStrength;
	float m_fStandardFresnelWidth;

	int
		m_iHealthPoint,
		m_iIdRoom;

	bool
		m_bAware,
		m_bIsDead;

	EEnemyType
		m_eEnemyType;
	EBrainMode
		m_eBrainMode;

	TTransform
		* m_pTarget;

	void
		Initialize(),
		Render();

	virtual void
		CollideOther(TEnemy &tOther),
		CollideWall(TCollideResult &tResult),
		CollidePlayer(TCollideResult &tResult),
		PromoteToChampion(),

		Damage(int iDamage, bool bForce = false),
		Update() = 0,
		Draw() = 0;
	void
		SetRadius(float fNewRadius){ m_fRadius = fNewRadius; }
	float
		GetRadius() { return m_fRadius; }
	virtual void OnDead();
	virtual void UpdateDead();
	virtual void NotifyPlayerDeath();

	virtual bool
		CanBeOpened(){ return false; };
	void
		IsRepulse(TGfxVec2 tStrongVector) { m_tVectorRepulse = tStrongVector*-0.04f; m_fTimeRepulse = 0.0f; m_eBrainMode = EBrainMode_Repulsing; };

	virtual int
		GetState();

	virtual void Stun(){};

	bool IsVulnerable(){ return m_bVulnerable; };
	void IsVulnerable(bool bVulnerable){ m_bVulnerable = bVulnerable; };
	bool IsPlayerVisible();

	void UpdateColor();

	// :: PROTECTED :: //
protected:
	TGfxVec2
		m_tVectorRepulse;
	float
		m_fTimeRepulse,
		m_fRadius,
		m_fRunSpeed,
		m_fWalkSpeed,

		m_fSpeed,
		m_fRotationSpeed,
		m_fAcceleration,
		m_fLockRange,
		m_fAttackRange,
		m_fAttackSpeed,
		m_fDisintegrationDelay,
		m_fPostDeathDuration;

	bool
		m_bVulnerable;
};
