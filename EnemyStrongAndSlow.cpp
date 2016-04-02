#include "Const.h"

const float ATTACK_FOV = 90 * DegToRad;
const float CHARGE_DELAY = 0.7f;
const float CHARGE_START_SPEED = 8.0f;
const float CHARGE_ACCELERATION = 10.0f;
const float CHARGE_MAX_SPEED = 16.0f;
const float CHARGING_ROTATION_SPEED = 40.0f * DegToRad;
const float HIT_REACH = 12.25f;
const float HIT_DISTANCE = 1.5f;
const float STUNNED_DELAY = 1.0f;
const float HIT_RECOVERING_DELAY = 1.0f;
const float HIT_DELAY = 0.6f;

extern TGame * g_pGame;

TEnemyStrongAndSlow::TEnemyStrongAndSlow(TGfxVec2 tPos, int iIDRoom, float fHeight)
{
	m_eEnemyType = EEnemyType_StrongAndSlow;
	m_tPos = tPos;
	m_iIdRoom = iIDRoom;
	m_eState = State_Slacking;
	m_fActionCoolDown = 0;
	m_fRotationSpeed = 60 * DegToRad;
	m_fRadius = 0.8f;
	m_iHealthPoint = 3;
	m_fAttackSpeed = 1.f;
	m_fLockRange = 20.f;
	m_fRunSpeed = 6.0f;
	m_fWalkSpeed = 2.0f;
	m_fTimeStatic = 3.0f;
	m_pTarget = g_pGame->m_pWorld->m_pPlayer;
	m_tTempTarget = tPos;
	AddCircleCollision(m_fRadius);
	m_fAngle = 0;

	m_fDistanceCrossed = 0.0f,
	m_fChargeCooldown = 0.0f;

	m_fDesiredAngle = 0.f;

	m_fCountDown = 0;

	m_tChargeColor = TGfxVec3(1, 0, 0);
	m_tAttackColor = TGfxVec3(1, 0.2f, 0);
	m_tBaseColor = TGfxVec3(0, 1, 1);
	m_tAngryColor = TGfxVec3(0, 1, 1);
	m_tStunColor = TGfxVec3(0.5f, 0.5f, 0.5f);

	m_tColor = m_tBaseColor;

	m_fStandardFresnelWidth = 1.916f;
	m_fStandardFresnelStrength = 0.394f;

	m_fDamagedFresnelStrength = 1.0f;
	m_fDamagedFresnelWidth = 0.0f;
	
	m_fFresnelStrength = m_fStandardFresnelStrength;
	m_fFresnelWidth = m_fStandardFresnelWidth;

	

	m_fSpeed = 0;
	m_fHeight = fHeight;


}

TEnemyStrongAndSlow::~TEnemyStrongAndSlow(void)
{
}

void TEnemyStrongAndSlow::Update()
{
	UpdateColor();

	if (m_bIsDead) { UpdateDead(); return; }

	TPlayer * pPlayer = g_pGame->m_pWorld->m_pPlayer;
	float fSquaredPlayerDistance = (Position() - g_pGame->m_pWorld->m_pPlayer->Position()).SquaredLength();

	m_fActionCoolDown -= DeltaTime();
	m_fChargeCooldown -= DeltaTime();

	if (pPlayer->IsDead()) m_eState = State_Slacking;

	if (m_fTimeRepulse < 0.2f)
	{
		m_tPos += m_tVectorRepulse;
		m_fTimeRepulse += DeltaTime();
	}
	else if (m_eBrainMode == EBrainMode_Repulsing)
	{
		m_eState = State_Preparing_Charge;
		m_eBrainMode = EBrainMode_Slacking;
	}

	switch (m_eState)
	{
	case State_Slacking:

		if (!pPlayer->IsDead() && fSquaredPlayerDistance < m_fLockRange * m_fLockRange)
		{
			if (g_pGame->m_pWorld->GetCurrentMap()->GetRoom(m_tPos) != g_pGame->m_pWorld->GetCurrentMap()->GetRoom(m_pTarget->Position()))
			{
				break;
			}

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

		if (g_pGame->m_pWorld->GetCurrentMap()->GetRoom(m_tPos) != g_pGame->m_pWorld->GetCurrentMap()->GetRoom(m_pTarget->Position()))
		{
			m_eState = State_Slacking;
		}

		m_fDesiredAngle = atan2f(m_pTarget->Position().y - Position().y, m_pTarget->Position().x - Position().x);

		Translate(TGfxVec2(cos(m_fAngle), sinf(m_fAngle)) * m_fRunSpeed * DeltaTime());

		if (m_fChargeCooldown <= 0 &&
			fSquaredPlayerDistance < DISTANCE_TO_CHARGE &&
			fSquaredPlayerDistance > MIN_DISTANCE_TO_CHARGE &&
			IsInLineOfSight(pPlayer->Position(), m_tPos, false))
		{
			m_eState = State_Preparing_Charge;
			m_fCountDown = CHARGE_DELAY;
		}
		else if (fSquaredPlayerDistance > m_fLockRange * m_fLockRange *5.0f)
		{
			m_eState = State_Slacking;
			m_bAware = false;
			m_tTempTarget = Position();
		}
		else if (fSquaredPlayerDistance < HIT_REACH && (fabs(m_fAngle - atan2f(m_pTarget->Position().y - Position().y, m_pTarget->Position().x - Position().x)) < ATTACK_FOV)){
			m_eState = State_Preparing_Hit;
			m_fCountDown = HIT_DELAY;
		}

		break;

	case State_Preparing_Hit:

		m_fCountDown -= DeltaTime();
		if (m_fCountDown <= 0)
		{
			TGfxVec2 tImpactPos = m_tPos + TGfxVec2(cos(m_fAngle), sinf(m_fAngle))* HIT_DISTANCE;
			TDamageData tDmg = { 0.25f, 0.f, 0.f, TGfxVec2(0.75f, 0.75f), 3.f, 0, FORM_CIRCLE, 3, m_fHeight };
			g_pGame->m_pWorld->AddDamage(tDmg, TYPE_LINEAR, tImpactPos, m_fAngle, FACTION_ENEMIES, EGfxColor_Red);

#ifdef TFA_FRESH
			LuaScreenShake(tImpactPos, pPlayer->Position(), 0.09);

			lua_pushnumber(GfxFreshLuaState(), tImpactPos.x);
			lua_pushnumber(GfxFreshLuaState(), tImpactPos.y);
			LuaCallGlobal(GfxFreshLuaState(), "CreateHeavyImpact");
#endif

			m_eState = State_Hit_Recovering;
			m_fCountDown = HIT_RECOVERING_DELAY;
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

		m_fDesiredAngle = atan2f(m_pTarget->Position().y - Position().y, m_pTarget->Position().x - Position().x);

		if (m_fCountDown <= 0)
		{
			m_fDistanceCrossed = 0;
			m_fSpeed = CHARGE_START_SPEED;
			m_eState = State_Charging;
		}
		break;

	case State_Charging:
	{
						   m_fSpeed = MMin(m_fSpeed + CHARGE_ACCELERATION* DeltaTime(), CHARGE_MAX_SPEED);
						   float fSpeed = m_fSpeed * DeltaTime();
						   m_fDesiredAngle = atan2f(m_pTarget->Position().y - Position().y, m_pTarget->Position().x - Position().x);

						   OrientedTranslate(fSpeed);
						   m_fDistanceCrossed += fSpeed;

						   if (m_fDistanceCrossed >= CHARGE_LENGTH || fSpeed <= 0)
						   {
							   m_eState = State_Charge_Decelerating;
							   m_fChargeCooldown = CHARGE_COOLDOWN_TIME;
						   }
						   break;
	}
	case State_Charge_Decelerating:
	{
									  m_fSpeed -= CHARGE_DECELERATION * DeltaTime();

									  float fSpeed = m_fSpeed * DeltaTime();
									  OrientedTranslate(fSpeed);

									  if (m_fSpeed <= 0.5) {
										  m_fSpeed = 0;
										  m_eState = EState_Pursuit;
									  }
									  break;
	}

	case State_Stun:
		m_fCountDown -= DeltaTime();

		if (m_fCountDown <= 0)
		{
			m_eState = EState_Pursuit;
		}
		break;

	default:
		break;
	}

	if (m_eState == State_Charging || m_eState == State_Charge_Decelerating)
	{
		m_fAngle = GetCorrectAngle(m_fAngle, m_fDesiredAngle, CHARGING_ROTATION_SPEED * DeltaTime());
	}
	else
	{
		m_fAngle = RotationLerp(m_fAngle, m_fDesiredAngle, 5.0f * DeltaTime());
	}

	m_fInvulCountdown -= DeltaTime();

	m_fHeight = g_pGame->m_pWorld->GetCurrentMap()->GetHeight(m_tPos, m_fHeight);

	
}

void TEnemyStrongAndSlow::PromoteToChampion() //Employee of the year
{
	m_tBaseColor = TGfxVec3(1, 0, 1);
};

void TEnemyStrongAndSlow::UpdateColor()
{
	TEnemy::UpdateColor();
	/*
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
	*/
}

void TEnemyStrongAndSlow::Draw()
{
#ifndef TFA_FRESH
	if (m_eBrainMode == EBrainMode_Static)
	{
		DrawShatteredCircle(g_pGame->m_pWorld->m_pLineSprite, m_tPos, sinf((float)GfxTimeGetMilliseconds()) * 2.0f, m_fRadius + 0.2f, EGfxColor_Green);
	}
	DrawOrientedCircle(g_pGame->m_pWorld->m_pLineSprite, m_tPos, m_fAngle, m_fRadius, EGfxColor_Red);
#endif
}

void TEnemyStrongAndSlow::CollideWall(TCollideResult &tResult)
{
	if (tResult.fLength != 0)
	{
		Translate(tResult.tNormal * tResult.fLength);

		if (m_eState == State_Charging || (m_eState == State_Charge_Decelerating && m_fSpeed > CHARGE_MAX_SPEED * 0.9))
		{
			if (tResult.tNormal.DotProduct(-TGfxVec2(cosf(m_fAngle), sin(m_fAngle))) > 0.7f)
			{
				m_eState = State_Stun;
				m_fCountDown = STUNNED_DELAY;
				m_fSpeed = 0;

#ifdef TFA_FRESH
				LuaScreenShake(m_tPos, g_pGame->m_pWorld->m_pPlayer->Position(), 0.09);
#endif
			}
			else
			{
				m_fSpeed -= CHARGE_DECELERATION * DeltaTime() * 3.0f;
			}
		}
		else if (m_eState == State_Charge_Decelerating)
		{
			m_fSpeed = 0;
			m_eState = State_Slacking;
		}
	}
}

void TEnemyStrongAndSlow::CollideOther(TEnemy &tOther)
{
	TCollideResult tResult;

	if (Collide(this, &tOther, tResult))
	{
		Translate(tResult.tNormal * tResult.fLength / 2);
		tOther.Translate(-tResult.tNormal * tResult.fLength / 2);
	}
}

void TEnemyStrongAndSlow::CollidePlayer(TCollideResult &)
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

void TEnemyStrongAndSlow::Stun()
{
	m_fCountDown = 3.f;
	m_eState = State_Stun;
}