#pragma once

class TEnemyStrongAndStun :
	public TEnemy
{
public :
	TEnemyStrongAndStun(TGfxVec2 tPos, int iIDRoom);
	virtual ~TEnemyStrongAndStun();

	enum EGolemStrongState
	{
		EState_Pursuit,
		EState_Attacking,
		State_Slacking,
		State_RunningAway,
		State_Static,
		State_Wrecking,
		State_Preparing_Hit,
		State_Hit_Recovering,
		State_Damage_Recovering,
		State_Preparing_Charge,
		State_Charging,
		State_Charge_Decelerating,
		State_Stun
	};

	EGolemStrongState 
		m_eState;

	float 
		m_fHitDelay,
		m_fCountDown,

		m_fHitRecoveringDelay,

		m_fDamageRecoveringDelay,

		m_fHitReach,
		m_fHitDistance,

		m_fChargeDelay,
		m_fChargeSpeed,
		m_fDistanceToCharge,
		m_fMaxDistanceToCharge,
		m_fChargeLength,
		m_fDistanceCrossed,
		m_fChargeDeceleration,
		m_fChargeRangeStart,
		m_fChargingRotationSpeed,
		m_fChargeCooldown,
		m_fChargeCooldownTime,

		m_fStunnedDelay;

	//Temp colors waiting for animation
	TGfxVec3 
		m_tChargeColor,
		m_tAttackColor,
		m_tAngryColor,
		m_tStunColor;

	void
		Update(),
		UpdateColor(),
		Draw();
	

	virtual void
		CollideOther(TEnemy &tOther),
		CollideWall(TCollideResult &tResult),
		CollidePlayer(TCollideResult &tResult),
		PromoteToChampion();
		
	virtual int 
		GetState();

	bool 
		IsPlayerVisible();
};