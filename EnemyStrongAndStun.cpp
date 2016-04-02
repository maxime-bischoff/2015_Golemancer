#include "Const.h"
const float ATTACK_FOV = 90 * DegToRad;
extern TGame * g_pGame;

TEnemyStrongAndStun::TEnemyStrongAndStun(TGfxVec2 tPos, int iIDRoom)
{
	m_eEnemyType = EEnemyTpe_StrongAndStun;
	m_tPos = tPos;
	m_iIdRoom = iIDRoom;
	m_eState = State_Slacking;
	m_eBrainMode = EBrainMode_Slacking;
	m_fActionCoolDown = 0;
	m_fRotationSpeed = 60 * DegToRad;
	m_fRadius = 0.6f;
	m_iHealthPoint = 3;
	m_fHitReach = 1.5f;
	m_fHitDistance = 1.5f;
	m_fAttackSpeed = 1.f;
	m_fLockRange = 15.f;
	m_fRunSpeed = 4.5f;
	m_fWalkSpeed = 1.0f;
	m_fTimeStatic = 3.0f;
	m_pTarget = g_pGame->m_pWorld->m_pPlayer;
	m_tTempTarget = tPos;
	AddCircleCollision(m_fRadius);
	m_fAngle = 0;

	m_fDesiredAngle = 0.f;
	m_fChargeDelay = 0.5f;
	m_fChargeSpeed = 16.0f;
	m_fDistanceToCharge = 9.0f;
	m_fMaxDistanceToCharge = 5.0f;
	m_fChargeLength = 12.0f;
	m_fChargeDeceleration = 24.0f;

	m_fChargingRotationSpeed = 20 * DegToRad;
	m_fDistanceCrossed = 0;
	m_fStunnedDelay = 1.0f;

	m_fChargeCooldown = 0;
	m_fChargeCooldownTime = 6.0f;

	m_fHitDelay = 0.5f;
	m_fCountDown = 0;
	m_fHitRecoveringDelay = 1.0f;
	m_fDamageRecoveringDelay = 0.5f;

	m_tChargeColor = TGfxVec3(1, 0, 0);
	m_tAttackColor = TGfxVec3(1, 0.2f, 0);
	m_tBaseColor = TGfxVec3(0, 1, 1);
	m_tAngryColor = TGfxVec3(0, 1, 1);
	m_tStunColor = TGfxVec3(0.5f, 0.5f, 0.5f);

	m_fSpeed = 0;
}

TEnemyStrongAndStun::~TEnemyStrongAndStun(void)
{
}

void TEnemyStrongAndStun::Update()
{
	TPlayer * pPlayer = g_pGame->m_pWorld->m_pPlayer;
	float fSquaredPlayerDistance = (Position() - g_pGame->m_pWorld->m_pPlayer->Position()).SquaredLength();

	m_fActionCoolDown -= DeltaTime();
	m_fChargeCooldown -= DeltaTime();

	switch (m_eState)
	{
	case State_Slacking:

		if (fSquaredPlayerDistance < m_fLockRange * m_fLockRange)
		{
			m_eState = EState_Pursuit;
			m_bAware = true;
			break;
		}

		if (m_fActionCoolDown <= 0)
		{
			m_fDesiredAngle = (GfxMathGetRandomFloat(0.f, static_cast <float>(2.f * M_PI)));
			m_fActionCoolDown = 3.f;
		}

		if ((Position() - m_tTempTarget).SquaredLength() < 1){
			Translate(TGfxVec2(cos(m_fAngle), sinf(m_fAngle)) * (m_fWalkSpeed)* DeltaTime());
			m_tTempTarget = Position();
		}
		else if ((Position() - m_tTempTarget).SquaredLength() > 1){
			m_fAngle = atan2f(TGfxVec2(m_tTempTarget - Position()).y, TGfxVec2(m_tTempTarget - Position()).x);
			Translate(TGfxVec2(cos(m_fAngle), sinf(m_fAngle)) * (m_fWalkSpeed)* DeltaTime());
		}
		break;

	case EState_Pursuit:

		m_fDesiredAngle = atan2f(m_pTarget->Position().y - Position().y, m_pTarget->Position().x - Position().x);

		Translate(TGfxVec2(cos(m_fAngle), sinf(m_fAngle)) * m_fRunSpeed * DeltaTime());

		if (m_fChargeCooldown <= 0 &&
			fSquaredPlayerDistance < m_fDistanceToCharge * m_fDistanceToCharge &&
			fSquaredPlayerDistance > m_fMaxDistanceToCharge * m_fMaxDistanceToCharge &&
			IsInLineOfSight(pPlayer->Position(), m_tPos, false))
		{
			m_eState = State_Preparing_Charge;
			m_fCountDown = m_fChargeDelay;
		}
		else if (fSquaredPlayerDistance > m_fLockRange * m_fLockRange *5.0f)
		{
			m_eState = State_Slacking;
			m_bAware = false;
			m_tTempTarget = Position();
		}
		else if (fSquaredPlayerDistance < m_fHitReach * m_fHitReach && (fabs(m_fAngle - atan2f(m_pTarget->Position().y - Position().y, m_pTarget->Position().x - Position().x)) < ATTACK_FOV)){
			m_eState = State_Preparing_Hit;
			m_fCountDown = m_fHitDelay;
		}

		break;

	case State_Preparing_Hit:

		m_fCountDown -= DeltaTime();
		if (m_fCountDown <= 0)
		{
			TGfxVec2 tImpactPos = m_tPos + TGfxVec2(cos(m_fAngle), sinf(m_fAngle))* m_fHitDistance;
			TDamageData tDmg = { 0.25f, 0.f, 0.f, TGfxVec2(0.75f, 0.75f), 0, 0.5f, FORM_CIRCLE, 1, m_fHeight };
			g_pGame->m_pWorld->AddDamage(tDmg, TYPE_STUN, tImpactPos, m_fAngle, FACTION_ENEMIES, EGfxColor_Red);

#ifdef TFA_FRESH
			LuaScreenShake(tImpactPos, pPlayer->Position(), 0.09);

			lua_pushnumber(GfxFreshLuaState(), tImpactPos.x);
			lua_pushnumber(GfxFreshLuaState(), tImpactPos.y);
			LuaCallGlobal(GfxFreshLuaState(), "CreateHeavyImpact");
#endif

			m_eState = State_Hit_Recovering;
			m_fCountDown = m_fHitRecoveringDelay;
		}
		break;

	case State_Hit_Recovering:

		m_fCountDown -= DeltaTime();
		if (m_fCountDown <= 0) m_eState = EState_Pursuit;

		break;

	case State_Static:
		if (m_fActionCoolDown < m_fTimeStatic){
			m_eState = State_Slacking;
			m_tTempTarget = Position();
		}

		break;
	case State_Wrecking: break;

	case State_Preparing_Charge:

		m_fCountDown -= DeltaTime();
		if (m_fCountDown <= 0)
		{
			m_fDistanceCrossed = 0;
			m_fSpeed = m_fChargeSpeed;
			m_eState = State_Charging;
		}
		break;

	case State_Charging:
	{
						   float fSpeed = m_fSpeed * DeltaTime();
						   OrientedTranslate(fSpeed);
						   m_fDistanceCrossed += fSpeed;

						   if (m_fDistanceCrossed >= m_fChargeLength || fSpeed <= 0)
						   {
							   m_eState = State_Charge_Decelerating;
							   m_fChargeCooldown = m_fChargeCooldownTime;
						   }
						   break;
	}
	case State_Charge_Decelerating:
	{
									  m_fSpeed -= m_fChargeDeceleration * DeltaTime();

									  float fSpeed = m_fSpeed * DeltaTime();
									  OrientedTranslate(fSpeed);

									  if (m_fSpeed <= 0.5) {
										  m_fSpeed = 0;
										  m_eState = State_Slacking;
									  }
									  break;
	}

	case State_Stun:
		m_fCountDown -= DeltaTime();

		if (m_fCountDown <= 0)
		{
			m_eState = State_Slacking;
		}
		break;

	default:
		break;
	}

	if (m_eState == State_Charging)
	{
		m_fAngle = GetCorrectAngle(m_fAngle, m_fDesiredAngle, m_fChargingRotationSpeed * DeltaTime());
	}
	else
	{
		m_fAngle = RotationLerp(m_fAngle, m_fDesiredAngle, 5.0f * DeltaTime());
	}

	m_fInvulCountdown -= DeltaTime();

	UpdateColor();
}

void TEnemyStrongAndStun::PromoteToChampion() //Employee of the year
{
	m_tBaseColor = TGfxVec3(1, 0, 1);
};

void TEnemyStrongAndStun::UpdateColor()
{
	if (m_fInvulCountdown > 0) return;
	float fChangeSpeed = 13 * DeltaTime();
	switch (m_eState)
	{
	case EState_Pursuit:  m_tColor = MLerp(m_tColor, m_tAngryColor, fChangeSpeed);
		break;
	case EState_Attacking: m_tColor = MLerp(m_tColor, m_tAttackColor, fChangeSpeed);
		break;
	case State_Slacking: m_tColor = MLerp(m_tColor, m_tBaseColor, fChangeSpeed);
		break;
	case State_Preparing_Hit: m_tColor = MLerp(m_tColor, m_tAttackColor, fChangeSpeed);
		break;
	case State_Hit_Recovering: m_tColor = MLerp(m_tColor, m_tAngryColor, fChangeSpeed);
		break;
	case State_Damage_Recovering: m_tColor = MLerp(m_tColor, m_tAngryColor, fChangeSpeed);
		break;
	case State_Preparing_Charge: m_tColor = MLerp(m_tColor, m_tChargeColor, fChangeSpeed);
		break;
	case State_Charging: m_tColor = MLerp(m_tColor, m_tChargeColor, fChangeSpeed);
		break;
	case State_Charge_Decelerating:m_tColor = MLerp(m_tColor, m_tAngryColor, fChangeSpeed);
		break;
	case State_Stun: m_tColor = MLerp(m_tColor, m_tStunColor, fChangeSpeed);
		break;
	default:
		m_tColor = MLerp(m_tColor, m_tBaseColor, fChangeSpeed);
		break;
	}
}

void TEnemyStrongAndStun::Draw()
{
#ifndef TFA_FRESH
	if (m_eBrainMode == EBrainMode_Static)
	{
		DrawShatteredCircle(g_pGame->m_pWorld->m_pLineSprite, m_tPos, sinf((float)GfxTimeGetMilliseconds()) * 2.0f, m_fRadius + 0.2f, EGfxColor_Green);
	}
	DrawOrientedCircle(g_pGame->m_pWorld->m_pLineSprite, m_tPos, m_fAngle, m_fRadius, EGfxColor_Red);
#endif
}

void TEnemyStrongAndStun::CollideWall(TCollideResult &tResult)
{
	if (tResult.fLength != 0)
	{
		Translate(tResult.tNormal * tResult.fLength);

		if (m_eState == State_Charging || (m_eState == State_Charge_Decelerating && m_fSpeed > m_fChargeSpeed * 0.9))
		{
			if (tResult.tNormal.DotProduct(-TGfxVec2(cosf(m_fAngle), sin(m_fAngle))) > 0.7f)
			{
				m_eState = State_Stun;
				m_fCountDown = m_fStunnedDelay;
				m_fSpeed = 0;

#ifdef TFA_FRESH
				LuaScreenShake(m_tPos, g_pGame->m_pWorld->m_pPlayer->Position(), 0.09);
#endif
			}
			else
			{
				m_fSpeed -= m_fChargeDeceleration * DeltaTime() * 3.0f;
			}
		}
		else if (m_eState == State_Charge_Decelerating)
		{
			m_fSpeed = 0;
			m_eState = State_Slacking;
		}
	}
}

void TEnemyStrongAndStun::CollideOther(TEnemy &tOther)
{
	TCollideResult tResult;

	Collide(this, &tOther, tResult);
	if (tResult.fLength != 0)
	{
		Translate(tResult.tNormal * tResult.fLength / 2);
		tOther.Translate(-tResult.tNormal * tResult.fLength / 2);
	}
}

void TEnemyStrongAndStun::CollidePlayer(TCollideResult &)
{
	if (m_eState == State_Charging)
	{
		TPlayer * pPlayer = g_pGame->m_pWorld->m_pPlayer;
		TGfxVec2 enemyDirection = TGfxVec2(cos(m_fAngle), sin(m_fAngle));
		float m_fDot = (pPlayer->Position() - m_tPos).DotProduct(enemyDirection.Rotate90());
		if (fabs(m_fDot) < m_fRadius){
#ifdef TFA_FRESH
			LuaScreenShake(0.09);
#endif
			pPlayer->Stun(1.0f);
			m_eState = State_Slacking;
		}
	}
}

int TEnemyStrongAndStun::GetState()
{
	if (m_eState == State_Preparing_Hit)
	{
		return 2;
	}
	else
	{
		return 1;
	}
}