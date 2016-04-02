#include "Const.h"
const float FOV = 120 * DegToRad;

extern TGame * g_pGame;

TEnemy::TEnemy(TGfxVec2 tPos, int iIDRoom, EEnemyType eType) :
m_fRadius(0.3f)
, m_fRunSpeed(3.5f)
, m_eEnemyType(eType)
, m_eBrainMode(EBrainMode_Slacking)
, m_fAttackRange(2)
, m_fLockRange(5)
, m_fActionCoolDown(0)
, m_fAttackSpeed(1.f)
, m_fTimeStatic(3.0f)
, m_tVectorRepulse(TGfxVec2(0.0f, 0.0f)),
m_fDisintegrationPower(20.0f)
{
	m_eCollisionLayer = LAYER_CHARACTER;
	m_iCollisionFlags.SetEverything();

	m_tColor = TGfxVec3(1, 1, 1);
	m_tDamagedColor = TGfxVec3(1, 0, 0);
	m_tPos = tPos;
	m_pTarget = g_pGame->m_pWorld->m_pPlayer;
	m_iIdRoom = iIDRoom;
	m_bAware = false;
	m_fInvulCountdown = 0;
	m_fDisintegrationDelay = 1.6f;
	m_fPostDeathDuration = 7.0f;
	m_bIsDead = false;


		


	 m_fDamagedFresnelStrength = 1.0f;
	 m_fDamagedFresnelWidth = 3.0f;
	 m_fStandardFresnelStrength = 1.0f;
	 m_fStandardFresnelWidth = 1.0f;
	 m_fFresnelStrength = m_fStandardFresnelStrength;
	 m_fFresnelWidth = m_fStandardFresnelWidth;

}

TEnemy::~TEnemy()
{
}

void TEnemy::Initialize()
{
}

void TEnemy::Update()
{
}


void TEnemy::UpdateColor()
{
	 float fChangeSpeed = 2	 * DeltaTime();
	 m_tColor = MLerp(m_tColor, m_tBaseColor, fChangeSpeed);
	 m_fFresnelWidth =    MLerp(m_fFresnelWidth, m_fStandardFresnelWidth, fChangeSpeed);
	 m_fFresnelStrength = MLerp(m_fFresnelStrength, m_fStandardFresnelStrength, fChangeSpeed);
}

void TEnemy::CollideWall(TCollideResult &tResult)
{
	if (tResult.fLength != 0)
	{
		Translate(tResult.tNormal * tResult.fLength);
	}
}

void TEnemy::CollideOther(TEnemy &tOther)
{
	TCollideResult tResult;

	if (Collide(this, &tOther, tResult))
	{
		Translate(tResult.tNormal * tResult.fLength / 2);
		tOther.Translate(-tResult.tNormal * tResult.fLength / 2);
	}
}

void TEnemy::CollidePlayer(TCollideResult &tResult)
{
	tResult.other = 0;
	return;
}

bool TEnemy::IsPlayerVisible()
{
	TPlayer * pPlayer = g_pGame->m_pWorld->m_pPlayer;
	bool isInFov = IsInFov(m_tPos, FOV, m_fAngle, m_fLockRange, pPlayer->Position());
	if (isInFov)
	{
		if (!IsInLineOfSight(m_tPos, pPlayer->Position(), true)) return false;
	}
	return false;
}

void TEnemy::NotifyPlayerDeath()
{
}

int TEnemy::GetState(){ return 1; }

void TEnemy::OnDead()
{
	m_bCollisionActive = false;
	g_pGame->m_pWorld->GetCurrentMap()->NotifyEnemyKill(this);
	m_bIsDead = true;
#ifdef TFA_FRESH
	int id = g_pGame->m_pWorld->GetEnemyID(this);

	if (id != -1)
	{
		lua_pushinteger(GfxFreshLuaState(), id);
		LuaCallGlobal(GfxFreshLuaState(), "KillEnemy");
	}

#endif
}

void TEnemy::UpdateDead()
{
	UpdateColor();
	m_fPostDeathDuration -= DeltaTime();
	if (m_fPostDeathDuration <= 0)
	{
		m_bNeedToBeDestroyed = true;
		return;
	}

#ifdef TFA_FRESH

	if (m_fDisintegrationDelay <= 0)
	{
		m_fDisintegrationPower = MLerp(m_fDisintegrationPower, 0, 1 * DeltaTime());
	}
	else
	{
		m_fDisintegrationDelay -= DeltaTime();
	}

#endif
}

void TEnemy::Damage(int iDamage, bool bForce)
{
	if (!bForce && m_fInvulCountdown > 0) return;


	m_iHealthPoint -= iDamage;

	m_fFresnelStrength = m_fDamagedFresnelStrength;
	m_fDamagedFresnelWidth = m_fDamagedFresnelWidth;
	m_tColor = m_tDamagedColor;


	if (m_iHealthPoint <= 0)
	{
		m_iHealthPoint = 0;

		OnDead();
	}
	for (int i = 0; i < iDamage * 2; i++)
		g_pGame->m_pWorld->AddCollectible(this->m_tPos, this->m_fHeight);
}

void TEnemy::PromoteToChampion()
{
	return;
}