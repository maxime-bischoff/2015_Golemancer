#pragma once

class TEnemyStrongAndSlow :
	public TEnemy
{
	// :: PUBLIC :: //
public:
// -- TYPES
	enum EGolemStrongState
	{
		EState_Pursuit,
		State_Slacking,
		EState_Attacking,
		State_RunningAway,
		State_Static,
		State_Wrecking,
		State_Preparing_Hit,
		State_Hit_Recovering,
		State_Damage_Recovering,
		State_Preparing_Charge,
		State_Charging,
		State_Charge_Decelerating,
		State_Stun,
		EState_Repulsing
	};

// -- FUNCTIONS
	void
		Update(),
		Draw();

	TEnemyStrongAndSlow(TGfxVec2 tPos, int iIDRoom, float fHeight = 0);
	virtual ~TEnemyStrongAndSlow(void);

	virtual void
		CollideOther(TEnemy &tOther),
		CollideWall(TCollideResult &tResult),
		CollidePlayer(TCollideResult &tResult),
		PromoteToChampion();
	void
		SetState(EGolemStrongState eNewState){ m_eState = eNewState; }
	virtual int
		GetState() { return m_eState; }
	virtual void
		Stun();
	// :: PRIVATE :: //
private:
	static const int
		DISTANCE_TO_CHARGE = 81,
		MIN_DISTANCE_TO_CHARGE = 25,
		CHARGE_LENGTH = 12,
		CHARGE_DECELERATION = 24,
		CHARGE_COOLDOWN_TIME = 6;

	TGfxVec3
		m_tChargeColor,
		m_tAttackColor,
		m_tAngryColor,
		m_tStunColor;

	EGolemStrongState
		m_eState;

	float
		m_fCountDown;

	float m_fDistanceCrossed ;
	float m_fChargeCooldown ;
	// -- FUNCTIONS
	void
		UpdateColor();
};
