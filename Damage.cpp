#include "Const.h"

extern TGame * g_pGame;

TDamage::TDamage(struct TDamageData &m_tDataDmg, const ETypeDamage &eType, const TGfxVec2 &tPos, const  float fAngle, EDamageFaction eFaction, unsigned int iColor, float fDiffAngle, float, TEnemy * tTarget)
{
	m_fAngle = fAngle + fDiffAngle;
	m_tPos = tPos;
	m_iColor = iColor;
	m_tDirection = TGfxVec2(cosf(m_fAngle), sinf(m_fAngle)); // idéalement angle du player ;

	m_tData.m_fDamageValue = m_tDataDmg.m_fDamageValue; // Dégats infligé par l'objet
	m_tData.m_fLifeSpan = m_tDataDmg.m_fLifeSpan;//Durée de vie de l'objet
	m_tData.m_fSpeed = m_tDataDmg.m_fSpeed;
	m_tData.m_tRadius = m_tDataDmg.m_tRadius;
	m_tData.m_fDelay = m_tDataDmg.m_fDelay;
	m_tData.eForm = m_tDataDmg.eForm;
	m_tData.m_fGrowingSpeed = m_tDataDmg.m_fGrowingSpeed;
	m_tData.m_iStyle = m_tDataDmg.m_iStyle;
	m_eType = eType;
	m_eFaction = eFaction;
	if (tTarget != 0)
		m_Target = tTarget;
	else
		m_Target = 0;
	m_tOrigin = tPos;

	m_fLaserLenght = LASER_LENGTH;

	m_eCollisionLayer = LAYER_PROJECTILE;

	m_iCollisionFlags.SetFlag(LAYER_WALL, true);

	switch (m_tDataDmg.eForm)
	{
	case FORM_CIRCLE:
		AddCircleCollision(m_tData.m_tRadius.x); // a refaire si on a une atténuation
		break;
	case FORM_BOX:
		AddBoxCollision(m_tData.m_tRadius); // a refaire si on a une atténuation
		break;
	case FORM_ARC:
		break;
	default:
		break;
	}
}

TDamage::~TDamage()
{
}

void TDamage::Update()
{
	if (m_tData.m_fDelay < 0.0f)
	{
		switch (m_eType)
		{
		case TYPE_CURVE:
		{
						   TGfxVec2 tDirectionEnemy = m_Target->Position() - Position();
						   float fAngleDeplacement = CorrectRange(atan2f(tDirectionEnemy.y, tDirectionEnemy.x));
						   float fRotationSpeed = 0.2f / (2.0f*PI);
						   m_fAngle = GetCorrectAngle(m_fAngle, fAngleDeplacement, fRotationSpeed);

						   m_tDirection = TGfxVec2(cosf(m_fAngle), sinf(m_fAngle));
						   m_tPos += m_tDirection * m_tData.m_fSpeed * DeltaTime();
		}
			break;

		case TYPE_LASER:

			break;

		default:
			m_tPos += m_tDirection * m_tData.m_fSpeed *DeltaTime();
			if (m_tData.eForm == FORM_CIRCLE){
				m_tData.m_tRadius += TGfxVec2(1.f, 1.f) * m_tData.m_fGrowingSpeed * DeltaTime();
				static_cast<THitboxCircle *>(m_pHitbox)->SetRadius(m_tData.m_tRadius.x);
			}
			break;
		}
		m_tData.m_fLifeSpan -= DeltaTime();
	}

	m_tData.m_fDelay -= DeltaTime();
}

void TDamage::Render()
{
#ifndef TFA_FRESH
	if (m_tData.eForm == EDamageForm::FORM_CIRCLE )
		DrawCircle( g_pGame->m_pWorld->m_pLineSprite, m_tPos, m_tData.m_tRadius.x , m_iColor );
	else
		DrawBox(g_pGame->m_pWorld->m_pLineSprite, m_tPos, m_fAngle , m_tData.m_tRadius, m_iColor);
#endif
}

bool TDamage::CollideEnemy(TEnemy &tEnemy, TCollideResult  &tResult, int iIndexEnemy)
{
	// :: COLLIDE

	if (Collide(this, &tEnemy, tResult))
	{
		// :: Damage Root :: //
		if (m_eType == TYPE_ROOT)
		{
#ifndef TFA_FRESH
			g_pGame->m_pWorld->m_pParticle->Generate(0, PI, 30, tEnemy.Position(), 0.6f, GfxColor(120, 255, 120, 255));
#endif
			tEnemy.m_eBrainMode = EBrainMode_Static;
			tEnemy.m_fTimeStatic = tEnemy.m_fActionCoolDown - 3.0f;
			//-- Freeze Enemy Around
			for (int k = 0; k < g_pGame->m_pWorld->m_iEnemyCount; k++)
			{
				if (k != iIndexEnemy)
				{
					if ((tEnemy.Position() - g_pGame->m_pWorld->m_pEnemyList[k]->Position()).SquaredLength() < 3.0f)
					{
						g_pGame->m_pWorld->m_pEnemyList[k]->m_eBrainMode = EBrainMode_Static;
						g_pGame->m_pWorld->m_pEnemyList[k]->m_fTimeStatic = g_pGame->m_pWorld->m_pEnemyList[k]->m_fActionCoolDown - 3.0f;
					}
				}
			}
		}
		else
		{
			tEnemy.Damage((int)m_tData.m_fDamageValue);

#ifdef TFA_FRESH
			LuaCreateImpactParticles(m_tPos.x, m_tData.m_fHeight, m_tPos.y);
#endif
			if (m_eType != TYPE_LASER)
				m_bNeedToBeDestroyed = true;

#ifndef TFA_FRESH
			g_pGame->m_pWorld->m_pParticle->Generate(0, PI, 30, tEnemy.Position(), 0.3f, EGfxColor_Red);
#endif
		}

		return true;
	}

	return false;
}

bool TDamage::CollidePlayer(TCollideResult  &tResult)
{
	TPlayer * tPlayer = g_pGame->m_pWorld->m_pPlayer;
#ifndef TFA_FRESH
	TParticle_Manager * tParticule = g_pGame->m_pWorld->m_pParticle;
#endif
	if (Collide(this, tPlayer, tResult))
	{
		int iDamage = static_cast <int>(m_tData.m_fDamageValue);

		if (tPlayer->m_iShieldPoint > 0)
		{
#ifndef TFA_FRESH
			if (m_eType == TYPE_LASER)
				tParticule->Generate(0, PI, 30, tPlayer->Position(), 0.15f, GfxColor(200, 255, 200, 255));
			else
				tParticule->Generate(0, PI, 30, Position(), 0.15f, GfxColor(200, 255, 200, 255));
#endif
		}
		if (m_eType == TYPE_STUN)
		{
			tPlayer->Stun(1.0f);
		}
#ifndef TFA_FRESH
		DrawBox(g_pGame->m_pWorld->m_pLineSprite, m_tPos, m_fAngle, m_tData.m_tRadius, GfxColor(255, 255, 0, 255));

		if (m_eType == TYPE_LASER)
			tParticule->Generate(0, PI, 30, tPlayer->Position(), 0.15f, GfxColor(200, 255, 200, 255));

		else
			tParticule->Generate(0, PI, 30, Position(), 0.15f, EGfxColor_Red);
#endif
		tPlayer->Damage(iDamage);

		return true;
	}
	return false;
}

bool TDamage::CollideFocus(TCollideResult  &tResult)
{
	if (g_pGame->m_pWorld->m_pPlayer->m_pSpellManager->m_pFocus->m_eFocusState != FOCUS_STATE_AWAITING)
	if (Collide(this, g_pGame->m_pWorld->m_pPlayer->m_pSpellManager->m_pFocus, tResult))
	{
		//Position( Position()+ (tResult.tNormal*tResult.fLength));
		m_tDirection = tResult.tNormal;
		m_eFaction = FACTION_PLAYER;
		return true;
	}

	return false;
}

void TDamage::CollideWall(TCollideResult &)
{
	m_bNeedToBeDestroyed = true;
	m_bInterruptCollision = true;
	m_bCollisionActive = false;



#ifdef TFA_FRESH
	LuaCreateImpactParticles(m_tPos.x, m_tData.m_fHeight, m_tPos.y);
	//lua_State * L = GfxFreshLuaState();
	//
	//lua_pushnumber(L, m_tPos.x);
	//lua_pushnumber(L, m_tData.m_fHeight);
	//lua_pushnumber(L, m_tPos.y);
	//LuaCallGlobal(L, "CreateHeavyImpactParticles");
	

#endif
	

}