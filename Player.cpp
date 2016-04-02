#include "Const.h"

extern TGame * g_pGame;
TGfxSprite * pTest;
const float WALK_LIMIT = 0.1f;
const float RUN_LIMIT = 0.8f;
const float TELEPORT_DELAY = 2.0f;

TPlayer::TPlayer() :m_fRadius(0.2f),
					m_fRunSpeed(PLAYER_RUN_SPEED),
					m_iEnergy(0),
					m_iHealthPoint((int)PLAYERMAXHEALTH),
					m_iShieldPoint(0),
					m_fRotationSpeed(PI ),
					m_fMoney(0),
					m_ePlayerMode(PLAYER_MODE_WALK),
					//m_ePlayerState(PLAYER_STATE_ACTIVE),
					m_fHeight(0),
					m_fLockFov (40.0f),
					m_eLockState(LOCK_UNLOCKED),
					m_fCamAngleAroundPlayer(0),
					m_tLastDirection( 0,1),
					m_tMoveDirection( 0, 0 )
{
#ifdef TFA_FRESH
	m_tCam.m_eMode = CAMERA_MODE_TPS ;
#else

	m_tCam.m_eMode = CAMERA_MODE_TOPDOWN ;
#endif

	m_eMovementAnimState = MOVEMENT_ANIM_STATE_IDLE;
	m_bCameraAngleLocked = false;
	m_fMovingAngle = 0;
	m_bCameraLocked = false;
	m_pTarget = 0;
	m_pSpellManager = new TSpellManager();
	m_fTargetAngle = PI/2.0f;
	m_fRotationSpeed = 5.0f* PI ;
	m_fCamRotationDisabled  = 0.f;
	m_fCamTimeRotationDisabled = 0.f;
	m_fCamRotationSpeed = 3.3f; //Rad per second
	m_fCamMaxRotationSpeed = 5.0f;  //Rad per second

	m_fCamRotationAccel = 20.0f;  //Rad per second
	m_fCamRotationDecel = 30.0f;  //Rad per second

	m_tTeleportPosition = TGfxVec2(0, 0);
	m_fTeleportCountDown = 0.0f;

	m_fStunDelay = 1.0f;
	m_fCountDown = 0;
	m_fDodgeTimer = 0;

	m_fTimeRotationLocked = 0;

	m_eCollisionLayer = LAYER_CHARACTER;

	m_iCollisionFlags.SetEverything();

	AddCircleCollision(m_fRadius);

	InitCamera();
}

TPlayer::~TPlayer()
{
}

void TPlayer::Teleport(const TGfxVec2 & tPos, float m_fAngleStick){
	Rotation( CorrectRange( m_fAngleStick ));
	Position(tPos);
	m_fTargetAngle = m_fAngleStick;

	m_fHeight = g_pGame->m_pWorld->GetCurrentMap()->GetHeight(m_tPos, m_fHeight);
	

	m_tCam.tCurrentParam.fDistance = (m_tCam.tWalkParam.fDistance + fabs(cos(m_tCam.tWalkParam.fHeight - m_tCam.tCurrentParam.fHeight))) - 1;

	m_fDodgeTimer = 0;

	m_tCam.tCurrentParam.fLookatHeight = m_tCam.tWalkParam.fLookatHeight;
	m_tCam.tCurrentParam = m_tCam.tWalkParam;
	m_tCam.Position(m_tPos - TGfxVec2(cosf(m_fAngle ), sin(m_fAngle ) * m_tCam.GetDistance()));
	m_tCam.m_tLookat = m_tPos - (m_tCam.Position() - m_tPos).Normalize() * m_tCam.tCurrentParam.fDistance;

	if (m_pSpellManager->m_pFocus->m_eFocusState != FOCUS_STATE_NOTTAKEN)
	{
		float	fAngleFocus = CorrectRange(Rotation() - 0.8f + PI);
		TGfxVec2 m_tTargetPos = Position() + TGfxVec2(-cosf(fAngleFocus), -sinf(fAngleFocus)) * 0.9f;
		m_pSpellManager->m_pFocus->Position(m_tTargetPos);
	}
	
	m_eMovementAnimState = MOVEMENT_ANIM_STATE_IDLE;
	m_ePlayerState = PLAYER_STATE_ACTIVE;

	GfxDbgPrintf("Teleport heigth = %f \n", m_fHeight);
	
#ifdef TFA_FRESH
	LuaUpdateCamera();
	LuaCallGlobal(GfxFreshLuaState(), "SetPlayerIdle");

	LuaUpdatePlayer(Position(), Rotation(), m_fHeight, (int)m_eMovementAnimState);
	LuaUpdateFocus(m_pSpellManager->m_pFocus->Position(), m_pSpellManager->m_pFocus->Rotation(), FOCUS_ANIM_IDLE);

#endif
}

void TPlayer::Respawn()
{
	m_iHealthPoint = PLAYERMAXHEALTH;
}

void TPlayer::UpdateTeleport()
{
		m_tPos = MLerp(m_tPos, m_tTeleportPosition, 2.0f * DeltaTime());
	
		m_fTeleportCountDown += DeltaTime();
		if (m_fTeleportCountDown >= TELEPORT_DELAY)
		{
			g_pGame->SetPostFadeAction(m_ePostFadeTeleport, m_iPostFadeParameter);
		}
		
	


}

void TPlayer::Update()
{

	if (IsDead()) return;

	if (m_ePlayerState == PLAYER_STATE_TELEPORTING)
	{
		UpdateTeleport();
		return;
	}

	m_pSpellManager->Update();

	m_fLeftStickX = MClamp(GfxInputGetPadStickLeftX() / 0.6f, -1, 1);
	m_fLeftStickY = MClamp(GfxInputGetPadStickLeftY() / 0.6f, -1, 1);
	m_fAngleStick = atan2f(m_fLeftStickY, m_fLeftStickX);

	m_fInvulCountDown -= DeltaTime();

	// :: CHEAT MODE INSIDE :: //
	if (GfxInputIsJustPressed(EGfxInputID_AnyPadDown))
	{
		g_pGame->SetPostFadeAction(POSTFADE_WARPHOME, 0);
#ifdef TFA_FRESH
		LuaCallGlobal(GfxFreshLuaState(), "HideTutoText");
#endif
	}

	// :: //

	if (m_ePlayerState == PLAYER_STATE_STUNNED)
	{
		m_fCountDown -= DeltaTime();
		if (m_fCountDown <= 0)
		{
			m_ePlayerState = PLAYER_STATE_ACTIVE;
		}
		else
		{
			return;
		}
	}

	if (m_fTimeRotationLocked > 0)
	{
		m_fTimeRotationLocked -= DeltaTime();
	}

	if (!GfxInputIsPadConnected(0) )
	{
		ChangeControlToKeyboard();
	}

	if (m_tCam.m_eMode == CAMERA_MODE_TOPDOWN)
	{
		TopDownCameraManagement();
	}
	else if (m_tCam.m_eMode == CAMERA_MODE_TPS)
	{
#ifdef OLDCAMERA
		OldCameraManagement();
#else
		TPSCameraManagement();
#endif
	}

	if (GfxInputIsJustPressed(EGfxInputID_AnyPadCross)) // Dodge
	{
		if ( m_tMoveDirection.SquaredLength() > WALK_LIMIT*WALK_LIMIT )
			m_fDodgeTimer = 0.64f;
	}

	if (m_pTarget != 0)
	{
		m_fTargetAngle = atan2f(m_pTarget->Position().y - Position().y, m_pTarget->Position().x - Position().x);
	}


	//Timer and stuffs

	if (m_fDodgeTimer >= 0)
		m_fDodgeTimer -= DeltaTime();

#ifndef TFA_FRESH
	if (m_tCam.m_eMode == CAMERA_MODE_TPS)
	DrawCamera(g_pGame->m_pWorld->m_pLineSprite, m_tCam.Position(), m_tCam.GetFOV(), m_tCam.m_tLookat, 2.0f, GfxColor(255, 0, 255, 255));

	if (GfxInputIsJustPressed(EGfxInputID_AnyPadUp) || GfxInputIsJustPressed(EGfxInputID_KeyBackspace))
	{
		if (m_tCam.m_eMode == CAMERA_MODE_TOPDOWN)m_tCam.m_eMode = CAMERA_MODE_TPS;
		else if (m_tCam.m_eMode == CAMERA_MODE_TPS) m_tCam.m_eMode = CAMERA_MODE_TOPDOWN;
	}
#endif

	//---UPDATE ANIM STATE---//
	if (m_iHealthPoint <= 0)
	{
		m_eMovementAnimState = MOVEMENT_ANIM_STATE_DEAD;
	}
	else if (m_fDodgeTimer > 0)
	{
		m_eMovementAnimState = MOVEMENT_ANIM_STATE_DODGE;
	}
	else if (TGfxVec2(m_fLeftStickX, m_fLeftStickY).SquaredLength() > RUN_LIMIT * RUN_LIMIT)
	{
		m_eMovementAnimState = MOVEMENT_ANIM_STATE_RUN;
	}
	else if (TGfxVec2(m_fLeftStickX, m_fLeftStickY).SquaredLength() > WALK_LIMIT * WALK_LIMIT)
	{
		m_eMovementAnimState = MOVEMENT_ANIM_STATE_WALK;
	}
	else
	{
		m_eMovementAnimState = MOVEMENT_ANIM_STATE_IDLE;
	}

	//--- Find Vertical Position ---//
	m_fHeight = g_pGame->m_pWorld->GetCurrentMap()->GetHeight(m_tPos, m_fHeight);
}

void TPlayer::CollideWall(TCollideResult &tResult)
{
	if (tResult.fLength != 0)
	{
		Translate(tResult.tNormal * tResult.fLength);
		m_tCam.Translate(tResult.tNormal * tResult.fLength);
	}
}

void TPlayer::Damage(int iDamage, bool bForce)
{
	if (!bForce &&  m_fInvulCountDown > 0) return;

	m_fInvulCountDown = PLAYER_INVUL_FRAME;

	if (m_iShieldPoint > 0)
	{
		int iOriginalShield = m_iShieldPoint;
		m_iShieldPoint -= iDamage;
		iDamage -= iOriginalShield;
		if (iDamage < 0)
		{
			iDamage = 0;
		}

#ifdef TFA_FRESH
		lua_pushboolean(GfxFreshLuaState(), true); //shield on
		LuaCallGlobal(GfxFreshLuaState(), "PlayDamageHud");
#endif
	}
	else{
#ifdef TFA_FRESH
		lua_pushboolean(GfxFreshLuaState(), false);
		LuaCallGlobal(GfxFreshLuaState(), "PlayDamageHud");
#endif
	}
	m_iHealthPoint -= iDamage;

	if (m_iHealthPoint <= 0)
	{
		m_iHealthPoint = 0;
		OnDead();
	}
}

void TPlayer::Stun(float fDelay)
{
	m_ePlayerState = PLAYER_STATE_STUNNED;
	m_fCountDown = fDelay;
}

void TPlayer::OnDead()
{
	g_pGame->m_pWorld->ProcessDeath();
	m_eMovementAnimState = MOVEMENT_ANIM_STATE_DEAD;

#ifdef TFA_FRESH
	LuaCallGlobal(GfxFreshLuaState(), "KillPlayer");
#endif
}

void TPlayer::Render()
{
#ifndef TFA_FRESH
	DrawOrientedCircle( g_pGame->m_pWorld->m_pLineSprite, m_tPos, m_fAngle, m_fRadius, EGfxColor_Green );

	if (m_iShieldPoint > 0)
	{
		DrawCircle(g_pGame->m_pWorld->m_pLineSprite, m_tPos, m_fRadius * 2, GfxColor(0, 255, 255, 25 * m_iShieldPoint));
	}

#else
	LuaUpdateShield((float)m_iShieldPoint / (float) MAX_SHIELD);
#endif
}

// :: PRIVATE :: //

void TPlayer::InitCamera()
{
	m_tCam.tWalkParam.fov = 65;

	m_tCam.tWalkParam.fHeight = 2.f;
	m_tCam.tWalkParam.fLateralOffset = 1.0f;
	m_tCam.tWalkParam.fDistance = 4.f;
	m_tCam.tWalkParam.fLookatHeight = 1.75f;
	m_tCam.tWalkParam.fMaxHeight = 3.f;
	m_tCam.tWalkParam.fMinHeight = 1.f;

	m_tCam.tFightParam.fov = 60.0f;
	m_tCam.tFightParam.fHeight = 0.8f;
	m_tCam.tFightParam.fLateralOffset = 0.55f;
	m_tCam.tFightParam.fDistance = 0.9f;
	m_tCam.tFightParam.fLookatHeight = 0.8f;

	m_tCam.tCastParam.fHeight = 1.0f;
	m_tCam.tCastParam.fLateralOffset = 1.0f;
	m_tCam.tCastParam.fDistance = 1.0f;

	m_tCam.tCurrentParam = m_tCam.tWalkParam;

	m_tCam.fBackRadius = 0.05f;
	m_tCam.fSideRadius = 0.5f;
}
void TPlayer::LockLeftEnemy()
{
	vector<TEnemy *> vpEnemies = GetEnemiesInFov(100);

	int iIteration = 0;

	while (iIteration < (int) vpEnemies.size())
	{
		for (int iIndex = 0; iIndex < (int) vpEnemies.size() - iIteration - 1; ++iIndex)
		{
			TEnemy * pEnemy1 = vpEnemies[iIndex];
			TEnemy * pEnemy2 = vpEnemies[iIndex + 1];

			TGfxVec2 tCamToEnemy1 = pEnemy1->Position() - m_tCam.Position();
			float fAngleDiff1 = GetAngleDiff(atan2f(tCamToEnemy1.y, tCamToEnemy1.x), m_tCam.Rotation());
			TGfxVec2 tCamToEnemy2 = pEnemy2->Position() - m_tCam.Position();
			float fAngleDiff2 = GetAngleDiff(atan2f(tCamToEnemy2.y, tCamToEnemy2.x), m_tCam.Rotation());

			if ((fAngleDiff1 >= 0 && fAngleDiff2 > 0 && fAngleDiff1 > fAngleDiff2) ||
				(fAngleDiff1 <= 0 && fAngleDiff1 < fAngleDiff2))
			{
				vpEnemies[iIndex + 1] = pEnemy1;
				vpEnemies[iIndex] = pEnemy2;
			}
		}
		iIteration++;
	}

	for (int iEnemy = 0; iEnemy < (int) vpEnemies.size(); ++iEnemy)
	{
		TEnemy * pEnemy = vpEnemies[iEnemy];
		TGfxVec2 tIntersection;
		if (pEnemy != m_pTarget)
		{
			if (!IsObstructed(m_tPos, pEnemy->Position(), tIntersection))
			{
				m_pTarget = pEnemy;
				m_eLockState = LOCK_LOCKING;
				break;
			}
		}
	}
}
void TPlayer::LockRightEnemy()
{
	vector<TEnemy *> vpEnemies = GetEnemiesInFov(100);

	int iIteration = 0;

	//Sorting Enemy
	while (iIteration < (int) vpEnemies.size())
	{
		for (int iIndex = 0; iIndex < (int) vpEnemies.size() - iIteration - 1; ++iIndex)
		{
			TEnemy * pEnemy1 = vpEnemies[iIndex];
			TEnemy * pEnemy2 = vpEnemies[iIndex + 1];

			TGfxVec2 tCamToEnemy1 = pEnemy1->Position() - m_tCam.Position();
			float fAngleDiff1 = GetAngleDiff(atan2f(tCamToEnemy1.y, tCamToEnemy1.x), m_tCam.Rotation());
			TGfxVec2 tCamToEnemy2 = pEnemy2->Position() - m_tCam.Position();
			float fAngleDiff2 = GetAngleDiff(atan2f(tCamToEnemy2.y, tCamToEnemy2.x), m_tCam.Rotation());

			if ( (fAngleDiff1 <= 0 && fAngleDiff2 < 0  && fAngleDiff1 < fAngleDiff2 ) ||
			     (fAngleDiff1 >= 0 && fAngleDiff1 > fAngleDiff2 ) )
			{
				vpEnemies[iIndex + 1] = pEnemy1;
				vpEnemies[iIndex] = pEnemy2;
			}
		}
		iIteration++;
	}

	for (int iEnemy = 0; iEnemy < (int) vpEnemies.size(); ++iEnemy)
	{
		TEnemy * pEnemy = vpEnemies[iEnemy];
		TGfxVec2 tIntersection;
		if (pEnemy != m_pTarget)
		{
			if (!IsObstructed(m_tPos, pEnemy->Position(), tIntersection))
			{
				m_pTarget = pEnemy;
				m_eLockState = LOCK_LOCKING;
				break;
			}
		}
	}
}
void TPlayer::SmartLock()
{
	TEnemy * tNewTarget = 0;

	TGfxVec2 tSrc = m_tPos;
	TGfxVec2 tDir = TGfxVec2(cos(m_fAngle), sin(m_fAngle));

	TGfxVec2 tDest = tSrc + tDir * 10000.f;

	m_pTarget = tNewTarget;
	if (tNewTarget)
		GfxDbgPrintf(" Target is : %d \n", tNewTarget);

	vector<TEnemy*> vpEnemies = GetEnemiesInFov(40);

	for (int iEnemy = 0; iEnemy < (int) vpEnemies.size(); ++iEnemy)
	{
		TEnemy * pEnemy = vpEnemies[iEnemy];

		TGfxVec2 tIntersection;
		if (! IsObstructed(m_tPos, pEnemy->Position(), tIntersection))
		{
			m_pTarget = pEnemy;
			break;
		}
	}
}
void TPlayer::CollideCamera()
{
	m_tCam.bIsColliding = false;

	// *********** Trace du rayon central avec la map ********** //

	TGfxVec2 tRaySrc =  m_tPos;
	TGfxVec2 tRayDest = m_tCam.Position();
	if (tRayDest == tRaySrc) return;
	TGfxVec2 tRayDir = (tRayDest - tRaySrc).Normalize();
	float fOffset = 0.3f;
	tRayDest = m_tPos + tRayDir *  (MMin(m_tCam.GetDistance() + m_tCam.fBackRadius + fOffset, m_tCam.tWalkParam.fDistance + m_tCam.fBackRadius));

		TGfxVec2 tIntersection= TGfxVec2(0,0);

		enum EIntersect{ Left, Mid, Right, None };
		EIntersect eIntersect = None;

		TGfxVec2 tNearestIntersect = TGfxVec2(100000.0f,100000.0f);
		TGfxVec2 tNearestMid = TGfxVec2(100000.0f, 100000.0f);

		bool bCollide = false;

		if (IsObstructed(tRaySrc, tRayDest, tNearestIntersect))
		{
			bCollide = true;
			m_tCam.bIsColliding = true;
			eIntersect = Mid;
		}

		float fNewDistance = m_tCam.tWalkParam.fDistance;

		switch (eIntersect)
		{
		case None :
			break;

		case Mid:
		{
			fNewDistance = (tNearestIntersect - tRaySrc).Length() - m_tCam.fBackRadius;

			#ifndef TFA_FRESH
			GfxLineSpriteSetDrawingColor(g_pGame->m_pWorld->m_pLineSprite, EGfxColor_Green);
			GfxLineSpriteJumpTo(g_pGame->m_pWorld->m_pLineSprite, tRaySrc.x, tRaySrc.y);
			GfxLineSpriteLineTo(g_pGame->m_pWorld->m_pLineSprite, tRayDest.x, tRayDest.y);
			#endif

			break;
		}

		default:
			break;
		}

		{
			float fLerpValue = 2 * DeltaTime();
			m_tCam.tCurrentParam.fDistance = fNewDistance;
		}

#ifndef TFA_FRESH
		GfxLineSpriteSetDrawingColor(g_pGame->m_pWorld->m_pLineSprite, EGfxColor_White);

		DrawCircle(g_pGame->m_pWorld->m_pLineSprite, m_tCam.Position(), m_tCam.fBackRadius, EGfxColor_Red);
#endif

		TCircle tCamera;
		tCamera.fRadius = m_tCam.fBackRadius;
		tCamera.tPos = m_tCam.Position();
		TCollideResult tResult;
		TBox tWall;

		TGfxVec2 tRay = TGfxVec2(3, 5).Normalize();
		TGfxVec2 tNormal = TGfxVec2(0, 1).Normalize();
}
void TPlayer::TopDownCameraManagement(void)
{
	if (m_fDodgeTimer > 0)
	{
		Move(TGfxVec2(cos(m_fAngleStick), sinf(m_fAngleStick)) * m_fRunSpeed * 2 * DeltaTime());
	}
	else if (TGfxVec2(m_fLeftStickX, m_fLeftStickY).Length() > 0.1)
	{
		Translate(TGfxVec2(cos(m_fAngleStick), sinf(m_fAngleStick)) * m_fRunSpeed * DeltaTime());
	}

	if (m_fLeftStickX != 0 || m_fLeftStickY != 0)
	{
		m_fAngle = m_fAngleStick;
	}

	float fLerpValue = 10.0f * DeltaTime();
	m_tCam.Position(MLerp(m_tCam.Position(), m_tPos, fLerpValue));
}
void TPlayer::OldCameraManagement()
{
	if (m_pTarget != 0)
	{
		m_tCam.m_eState = CAMERA_STATE_FIGHT;

		float fLerpValue = 5 * UnscaleDeltaTime();
		m_tCam.m_tLookat = MLerp(m_tCam.m_tLookat, m_tPos - (m_tPos - m_pTarget->Position()) / 2.0f, fLerpValue);

		if (m_fLeftStickY > 0.5f)
		{
			OrientedTranslate(m_fRunSpeed * DeltaTime());
		}

		if (m_fLeftStickY < -0.5f)
		{
			Translate(-TGfxVec2(cos(m_fAngle), sinf(m_fAngle)) * m_fRunSpeed *  DeltaTime());
		}

		float fTargetDistance = (m_pTarget->Position() - m_tPos).Length();
		float minMultiplier = 0.7f;
		float minDistance = 4;
		float fMultiplier = 1;
		if (fTargetDistance < minDistance)
		{
			fMultiplier = minMultiplier + ((fTargetDistance / minDistance) * fMultiplier - minMultiplier);
		}

		if (m_fLeftStickX < -0.3)
		{
			Translate((TGfxVec2(-sinf(m_fAngle), cosf(m_fAngle))*   m_fRunSpeed *  DeltaTime()));
		}
		if (m_fLeftStickX > 0.3)
		{
			Translate((-TGfxVec2(-sinf(m_fAngle), cosf(m_fAngle))*   m_fRunSpeed *  DeltaTime()));
		}

		TGfxVec2 tOffsetCamera = TGfxVec2(cosf(m_tCam.tCurrentParam.fLateralOffset), sinf(m_tCam.tCurrentParam.fLateralOffset));

		float fLerpValueCamPos = 50.0f * UnscaleDeltaTime();

		TGfxVec2 tTargetToPlayer = m_pTarget->Position() - m_tPos;
		float fTargetAngle = atan2f(tTargetToPlayer.y, tTargetToPlayer.x);
		m_tCam.Rotation(fTargetAngle);
		TGfxVec2 offset = -tTargetToPlayer.Rotate90().Normalize() * m_tCam.tCurrentParam.fLateralOffset;
		TGfxVec2 tNewCamPos = m_tPos - TGfxVec2(cosf(m_fAngle), sinf(m_fAngle)) * m_tCam.tCurrentParam.fDistance + offset;
		m_tCam.Position(MLerp(m_tCam.Position(), tNewCamPos, fLerpValueCamPos));
	}
	else
	{
		m_tCam.m_eState = CAMERA_STATE_WALK;
		TGfxVec2 tMoveVector = TGfxVec2(m_fLeftStickX, m_fLeftStickY).Rotate(m_fAngle - PI / 2.0f);

		float fDistanceLookat = 3.0f;

		TGfxVec2 tCamToPlayer = m_tCam.Position() - m_tPos;

		float fCamAngleWorld = atan2f(tCamToPlayer.y, tCamToPlayer.x);
		float fCamAngleLocal = CorrectRange(atan2f(tCamToPlayer.y, tCamToPlayer.x) - m_fAngle);
		fCamAngleLocal;
		float fRightStickX = GfxInputGetPadStickRightX();

		if (!m_pSpellManager->IsRuneTracing())
		{
			if (fRightStickX == 0)
			{
				if (m_fCamRotationSpeed > 0)
				{
					m_fCamRotationSpeed -= m_fCamRotationDecel * DeltaTime();
					if (m_fCamRotationSpeed < 0) m_fCamRotationSpeed = 0;
				}
				else if (m_fCamRotationSpeed < 0)
				{
					m_fCamRotationSpeed += m_fCamRotationDecel * DeltaTime();
					if (m_fCamRotationSpeed > 0) m_fCamRotationSpeed = 0;
				}
			}
			else
			{
				if (fRightStickX > 0){
					m_fCamRotationSpeed += m_fCamRotationAccel * DeltaTime();
					if (m_fCamRotationSpeed > m_fCamMaxRotationSpeed) m_fCamRotationSpeed = m_fCamMaxRotationSpeed;
				}
				else if (fRightStickX < 0){
					m_fCamRotationSpeed -= m_fCamRotationAccel * DeltaTime();
					if (m_fCamRotationSpeed < -m_fCamMaxRotationSpeed) m_fCamRotationSpeed = -m_fCamMaxRotationSpeed;
				}
			}
		}
		float fAngleModifier = m_fCamRotationSpeed * DeltaTime();

		float fCamNewAngle = fCamAngleWorld - fAngleModifier;

		m_tCam.Position(m_tPos + TGfxVec2(cosf(fCamNewAngle), sinf(fCamNewAngle)) * m_tCam.tCurrentParam.fDistance);
		m_tCam.m_tLookat = m_tPos - TGfxVec2(cosf(fCamNewAngle), sinf(fCamNewAngle)) * m_tCam.tCurrentParam.fDistance;

		TGfxVec2 tMoveDirection = TGfxVec2(m_fLeftStickX, m_fLeftStickY);

		tMoveDirection = tMoveDirection.Rotate(CorrectRange(fCamAngleWorld + PI / 2.0f));
		Translate(tMoveDirection * m_fRunSpeed * DeltaTime());
		if (m_fLeftStickX != 0 || m_fLeftStickY != 0)
		{
			m_fTargetAngle = CorrectRange(m_fAngleStick + CorrectRange(fCamAngleWorld + PI / 2.0f));
		}
	}
}
void TPlayer::TPSCameraManagement()
{
	float  RightTrigger = GfxInputGetPadTriggerRight(0);
	m_tMoveDirection = TGfxVec2(m_fLeftStickX, m_fLeftStickY);
	
	//--- Rune Or Close combat fight
	if (RightTrigger > 0.2)
	{
		m_tCam.Update();

		TGfxVec2 tCamToTarget = (m_tCam.Position()) - m_tPos;

		if (m_eLockState == LOCK_UNLOCKED) { m_eLockState = LOCK_LOCKING; }

		if (m_pTarget)
		{
			if (m_eLockState == LOCK_LOCKED)
			{
				tCamToTarget = m_tCam.Position() - m_pTarget->Position();
				float fNewAngle = atan2f(tCamToTarget.y, tCamToTarget.x);
				m_fCamAngleAroundPlayer = fNewAngle;
			}
			else if (m_eLockState == LOCK_LOCKING)
			{
				float fRotationLerpCam = 15.0f;
				tCamToTarget = m_tCam.Position() - m_pTarget->Position();
				float fNewAngle = atan2f(tCamToTarget.y, tCamToTarget.x);
				m_fCamAngleAroundPlayer = RotationLerp(m_fCamAngleAroundPlayer, fNewAngle, fRotationLerpCam * DeltaTime());
				float fAngleDiff = fabs(GetAngleDiff(fNewAngle, m_fCamAngleAroundPlayer));
				if (fAngleDiff< 0.005f)
				{
					m_fCamAngleAroundPlayer = fNewAngle;
					m_eLockState = LOCK_LOCKED;
				}
			}

			m_tMoveDirection = m_tMoveDirection.Rotate(CorrectRange(m_fCamAngleAroundPlayer + PI / 2.0f));

			if (GfxInputIsJustPressed(EGfxInputID_AnyPadShoulderR))
			{
				LockRightEnemy();
			}
			else if (GfxInputIsJustPressed(EGfxInputID_AnyPadShoulderL))
			{
				LockLeftEnemy();
			}
		}
		else
		{
			m_fCamAngleAroundPlayer = atan2f(tCamToTarget.y, tCamToTarget.x);
			m_tMoveDirection = m_tMoveDirection.Rotate(CorrectRange(m_fCamAngleAroundPlayer + PI / 2.0f));
			SmartLock();
		}

		if (m_fDodgeTimer > 0)
		{
			//TCollideResult tResult;
			if (m_tMoveDirection.SquaredLength() > WALK_LIMIT * WALK_LIMIT)
				m_tLastDirection = m_tMoveDirection.Normalize();
			Move(m_tLastDirection * m_fRunSpeed * 2 * DeltaTime());

			m_fAngle = CorrectRange(atan2f(m_tLastDirection.y, m_tLastDirection.x));
		}
		else
		{
			if (m_tMoveDirection.SquaredLength() > WALK_LIMIT * WALK_LIMIT)
				Move(m_tMoveDirection * m_fRunSpeed * DeltaTime());
			m_fAngle = CorrectRange(m_fCamAngleAroundPlayer + PI);
		}

		m_fTargetAngle = m_fAngle;
		m_tCam.Rotation(m_fAngle);
		CollideCamera();
		m_tCam.Position(m_tPos + TGfxVec2(cosf(m_fCamAngleAroundPlayer), sinf(m_fCamAngleAroundPlayer)) * m_tCam.tCurrentParam.fDistance);
		m_tCam.m_tLookat = m_tPos - TGfxVec2(cosf(m_fCamAngleAroundPlayer), sinf(m_fCamAngleAroundPlayer)) * m_tCam.tCurrentParam.fDistance;
	}
	else //--- Trigger not pressed
	{
		if (m_eLockState != LOCK_UNLOCKED) m_eLockState = LOCK_UNLOCKED;

		m_pTarget = 0;
		float fRightStickX = GfxInputGetPadStickRightX();
		TGfxVec2 tCamToPlayer = m_tCam.Position() - m_tPos;
		float fCamAngleWorld = atan2f(tCamToPlayer.y, tCamToPlayer.x);
		float fAngleModifier = fRightStickX * m_fCamRotationSpeed * DeltaTime();
		float fCamNewAngle = fCamAngleWorld - fAngleModifier;

		m_tMoveDirection = m_tMoveDirection.Rotate(CorrectRange(fCamAngleWorld + PI_2));
		if (m_fDodgeTimer > 0)
		{
			if (m_tMoveDirection.SquaredLength() > WALK_LIMIT * WALK_LIMIT)
				m_tLastDirection = m_tMoveDirection.Normalize();
			Move(m_tLastDirection * m_fRunSpeed * 2 * DeltaTime());
		}
		else
			if (m_tMoveDirection.SquaredLength() > WALK_LIMIT * WALK_LIMIT)
				Move(m_tMoveDirection * m_fRunSpeed * DeltaTime());

		CollideCamera();
		m_tCam.Position(m_tPos + TGfxVec2(cosf(fCamNewAngle), sinf(fCamNewAngle)) * m_tCam.tCurrentParam.fDistance);
		m_tCam.m_tLookat = m_tPos - TGfxVec2(cosf(fCamNewAngle), sinf(fCamNewAngle)) * m_tCam.tCurrentParam.fDistance;
		m_tCam.Rotation(CorrectRange(fCamAngleWorld + PI));

		if (m_fLeftStickX != 0 || m_fLeftStickY != 0)
		{
			m_fAngle = atan2(m_tMoveDirection.y, m_tMoveDirection.x);
			m_fTargetAngle = CorrectRange(m_fAngleStick + CorrectRange(fCamAngleWorld + PI_2));
		}
		m_tCam.tCurrentParam.fHeight -= GfxInputGetPadStickRightY() / 8.0f;
		m_tCam.tCurrentParam.fLookatHeight += GfxInputGetPadStickRightY() / 8.0f;

		if (m_tCam.tCurrentParam.fHeight < m_tCam.tWalkParam.fMinHeight)
		{
			m_tCam.tCurrentParam.fHeight = m_tCam.tWalkParam.fMinHeight;
			m_tCam.tCurrentParam.fLookatHeight = m_tCam.tWalkParam.fLookatHeight - (m_tCam.tCurrentParam.fHeight - m_tCam.tWalkParam.fHeight);
		}
		else if (m_tCam.tCurrentParam.fHeight > m_tCam.tWalkParam.fMaxHeight)
		{
			m_tCam.tCurrentParam.fHeight = m_tCam.tWalkParam.fMaxHeight;
			m_tCam.tCurrentParam.fLookatHeight = m_tCam.tWalkParam.fLookatHeight - (m_tCam.tCurrentParam.fHeight - m_tCam.tWalkParam.fHeight);
		}

		m_tCam.tCurrentParam.fOffsetY = (1 - m_tCam.tCurrentParam.fDistance / m_tCam.tWalkParam.fDistance) *1.f;
	}
}
void TPlayer::ChangeControlToKeyboard()
{
	TGfxVec2 tAngleTemp(0, 0);
	if (GfxInputIsPressed(EGfxInputID_KeyArrowLeft))
	{
		tAngleTemp.x -= 1;
		m_tPos.x -= m_fRunSpeed * DeltaTime();
	}
	if (GfxInputIsPressed(EGfxInputID_KeyArrowRight))
	{
		tAngleTemp.x += 1;
		m_tPos.x += m_fRunSpeed * DeltaTime();
	}
	if (GfxInputIsPressed(EGfxInputID_KeyArrowUp))
	{
		tAngleTemp.y += 1;
		m_tPos.y += m_fRunSpeed * DeltaTime();
	}
	if (GfxInputIsPressed(EGfxInputID_KeyArrowDown))
	{
		tAngleTemp.y -= 1;
		m_tPos.y -= m_fRunSpeed * DeltaTime();
	}

	if (tAngleTemp != TGfxVec2(0, 0))
	{
		float
			m_fAngleStick = atan2f(tAngleTemp.y, tAngleTemp.x);
		m_fAngle = m_fAngleStick;
	}
}
bool TPlayer::IsObstructed(const TGfxVec2 & tSrc, const TGfxVec2 & tDest,  TGfxVec2 & tNearest)
{
	TMap *  pMap = g_pGame->m_pWorld->GetCurrentMap();
	TGfxVec2  tIntersection = TGfxVec2(0, 0);
	tNearest = TGfxVec2(100000.0f, 100000.0f);

	bool bCollide = false;

	for (int iRoom = 0; iRoom< (int)pMap->m_pRooms.size(); ++iRoom)
	{
		TRoom * pRoom = pMap->m_pRooms[iRoom];

		for (int i = 0; i < pRoom->m_iWallCount; i++)
		{
			if (pRoom->m_pWalls[i]->m_iCollisionFlags.GetFlag(LAYER_CAMERA))
			{
				TWall * pWall = pRoom->m_pWalls[i];

				if (pWall->m_eType == WALL_BOX)
				{
					for (int iCorner = 0; iCorner < 4; iCorner++)
					{
						TGfxVec2 b1 = pWall->Position() + pWall->tCornerPos[iCorner];
						TGfxVec2 b2 = pWall->Position() + pWall->tCornerPos[(iCorner + 1) % 4];

						if (CollideSegments(tSrc, tDest, b1, b2, tIntersection))
						{
							if ((tSrc - tIntersection).SquaredLength() < (tSrc - tNearest).SquaredLength())
							{
								#ifndef TFA_FRESH
									GfxLineSpriteSetDrawingColor(g_pGame->m_pWorld->m_pLineSprite, GfxColor(0,255,255,255));
									GfxLineSpriteJumpTo(g_pGame->m_pWorld->m_pLineSprite, tSrc.x, tSrc.y);
									GfxLineSpriteLineTo(g_pGame->m_pWorld->m_pLineSprite, pWall->Position().x, pWall->Position().y);
								#endif

								tNearest = tIntersection;
								bCollide = true;
							}
						}
					}
				}
			}
		}
	}

	for (int iDoor = 0; iDoor < (int) pMap->m_vpDoors.size(); ++iDoor)
	{
		TDoor * pDoor = pMap->m_vpDoors[iDoor];

			TWall * pWallLeft = pDoor->m_pWallLeft;

				for (int iCorner = 0; iCorner < 4; iCorner++)
				{
					if (CollideSegments(tSrc, tDest, pWallLeft->Position() + pWallLeft->tCornerPos[iCorner], pWallLeft->Position() + pWallLeft->tCornerPos[(iCorner + 1) % 4], tIntersection))
					{
						if ((tSrc - tIntersection).SquaredLength() < (tSrc - tNearest).SquaredLength())
						{
							tNearest = tIntersection;
							bCollide = true;
						}
					}
				}

				TWall * pWallRight = pDoor->m_pWallRight;

				for (int iCorner = 0; iCorner < 4; iCorner++)
				{
					if (CollideSegments(tSrc, tDest, pWallRight->Position() + pWallRight->tCornerPos[iCorner], pWallRight->Position() + pWallRight->tCornerPos[(iCorner + 1) % 4], tIntersection))
					{
						if ((tSrc - tIntersection).SquaredLength() < (tSrc - tNearest).SquaredLength())
						{
							tNearest = tIntersection;
							bCollide = true;
						}
					}
				}
	}

#ifndef TFA_FRESH
	GfxLineSpriteSetDrawingColor(g_pGame->m_pWorld->m_pLineSprite, EGfxColor_White);
	GfxLineSpriteJumpTo(g_pGame->m_pWorld->m_pLineSprite, tSrc.x, tSrc.y);
	GfxLineSpriteLineTo(g_pGame->m_pWorld->m_pLineSprite, tDest.x, tDest.y);

	if (bCollide)
	{
		DrawCircle(g_pGame->m_pWorld->m_pLineSprite, tNearest, 0.10f, EGfxColor_Red, 10);
	}
#endif

	return bCollide;
}
vector<TEnemy *>  TPlayer::GetEnemiesInFov(float fFov)
{
	vector<TEnemy*>  vpEnemies;
	float fMaxDistance = 30;

	TGfxVec2 fDir = TGfxVec2(cos(m_fAngle), sin(m_fAngle));

	for (int iEnemy = 0; iEnemy < g_pGame->m_pWorld->m_iEnemyCount; ++iEnemy)
	{
		TEnemy * pEnemy = g_pGame->m_pWorld->m_pEnemyList[iEnemy];
		if (!pEnemy->m_bIsDead)
		{
			if (IsInFov(m_tCam.Position(), fFov * DegToRad, m_tCam.Rotation(), fMaxDistance, pEnemy->Position()))
			{
				vpEnemies.push_back(pEnemy);
			}
		}
	}

	// Enemies priority sortings

	int iIteration = 0;

	while (iIteration < (int) vpEnemies.size())
	{
		for (int iIndex = 0; iIndex < (int) vpEnemies.size() - iIteration - 1; ++iIndex)
		{
			TEnemy * pEnemy1 = vpEnemies[iIndex];
			TEnemy * pEnemy2 = vpEnemies[iIndex + 1];

			float fDistance1 = (pEnemy1->Position() - m_tPos).SquaredLength();
			float fDistance2 = (pEnemy2->Position() - m_tPos).SquaredLength();

			if (fDistance1 > fDistance2)
			{
				vpEnemies[iIndex + 1] = pEnemy1;
				vpEnemies[iIndex] = pEnemy2;
			}
		}
		iIteration++;
	}

#ifndef TFA_FRESH

	DrawPartialCircle(g_pGame->m_pWorld->m_pLineSprite, m_tCam.Position(), m_tCam.Rotation(), fMaxDistance, fFov * DegToRad, EGfxColor_Red);

	for (int iEnemy = 0; iEnemy < (int)vpEnemies.size(); iEnemy++)
	{
		TEnemy * pEnemy = vpEnemies[iEnemy];

		if (pEnemy != 0)
		{
			unsigned int iColor;
			switch (iEnemy)
			{
			case 0:   iColor = EGfxColor_Red;
				break;
			case 1:   iColor = GfxColor(0, 255, 0, 255);
				break;
			case 2:   iColor = GfxColor(0, 0, 255, 255);
				break;
			default:  iColor = GfxColor(255, 255, 255, 255);
				break;
			}
			DrawShatteredCircle(g_pGame->m_pWorld->m_pLineSprite, pEnemy->Position(),0, 0.70f, iColor);
		}
	}

#endif

	return vpEnemies;
}