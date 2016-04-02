#include "Const.h"


TWall::TWall(TGfxVec2 tPos, TGfxVec2 tRadius, float fAngle, bool bCollideCamera, bool bCollidePlayer, bool bCollideProjectile, unsigned int iColor)
{
	m_tPos = tPos,
	m_fAngle = fAngle;
	m_tRadius = tRadius;
	m_iColor = iColor;
	m_bCollideCamera = bCollideCamera;
	m_bCollidePlayer = bCollidePlayer;
	m_bCollideProjectiles = bCollideProjectile;

	m_iCollisionFlags.Clear();

	if (m_bCollideCamera && bCollidePlayer && bCollideProjectile){
		m_eCollisionLayer = LAYER_WALL;
		m_iCollisionFlags.SetEverything();

	}
	else if (!m_bCollideCamera && bCollidePlayer && !bCollideProjectile)
	{
		m_eCollisionLayer = LAYER_HOLE;
		m_iCollisionFlags.SetFlag(LAYER_CHARACTER);	
	}
	else if (!m_bCollideCamera && bCollidePlayer && bCollideProjectile)
	{
		m_eCollisionLayer = LAYER_SEMI_WALL;
		m_iCollisionFlags.SetFlag(LAYER_CHARACTER);
		m_iCollisionFlags.SetFlag(LAYER_PROJECTILE);
	}
	else{
		m_eCollisionLayer = LAYER_WALL;
		m_iCollisionFlags.SetEverything();
	}
	
	AddBoxCollision(tRadius, TGfxVec2(0, 0));

	m_eType = WALL_BOX;
	if (fAngle == 0)
	{
		tCornerPos[0] = TGfxVec2(-m_tRadius.x, m_tRadius.y);
		tCornerPos[1] = TGfxVec2(m_tRadius.x, m_tRadius.y);
		tCornerPos[2] = TGfxVec2(m_tRadius.x, -m_tRadius.y);
		tCornerPos[3] = TGfxVec2(-m_tRadius.x, -m_tRadius.y);
	}
	else
	{
		const TGfxVec2 tAxisX = TGfxVec2(cosf(m_fAngle), -sinf(m_fAngle));
		const TGfxVec2 tAxisY = TGfxVec2(sinf(m_fAngle), cosf(m_fAngle));
		
		tCornerPos[0] =  - tAxisX * tRadius.x + tAxisY * tRadius.y;
		tCornerPos[1] =  + tAxisX * tRadius.x + tAxisY * tRadius.y;
		tCornerPos[2] =  + tAxisX * tRadius.x - tAxisY * tRadius.y;
		tCornerPos[3] =  - tAxisX * tRadius.x - tAxisY * tRadius.y;
	}
}

TWall::TWall(TGfxVec2 tPos, float fRadius, float fAngle, bool bCollideCamera, bool bCollidePlayer, bool bCollideProjectile, unsigned int iColor)
{
	m_tPos = tPos,
	m_fAngle = fAngle;
	m_fRadius = fRadius;
	m_iColor = iColor;

	m_eType = WALL_CIRCLE;
	m_bCollideCamera = bCollideCamera;
	m_bCollidePlayer = bCollidePlayer;
	m_bCollideProjectiles = bCollideProjectile;

	if (m_bCollideCamera && bCollidePlayer && bCollideProjectile){
		m_eCollisionLayer = LAYER_WALL;
		m_iCollisionFlags.SetEverything();

	}
	else if (!m_bCollideCamera && bCollidePlayer && !bCollideProjectile)
	{
		m_eCollisionLayer = LAYER_HOLE;
		m_iCollisionFlags.SetFlag(LAYER_CHARACTER);
	}
	else if (m_bCollideCamera && bCollidePlayer && bCollideProjectile)
	{
		m_eCollisionLayer = LAYER_SEMI_WALL;
		m_iCollisionFlags.SetFlag(LAYER_CHARACTER);
		m_iCollisionFlags.SetFlag(LAYER_PROJECTILE);
	}
	else{
		m_eCollisionLayer = LAYER_WALL;
		m_iCollisionFlags.SetEverything();
	}



	AddCircleCollision(fRadius, TGfxVec2(0, 0));


	if (fAngle == 0)
	{
		tCornerPos[0] = TGfxVec2(-m_tRadius.x, m_tRadius.y);
		tCornerPos[1] = TGfxVec2(m_tRadius.x, m_tRadius.y);
		tCornerPos[2] = TGfxVec2(m_tRadius.x, -m_tRadius.y);
		tCornerPos[3] = TGfxVec2(-m_tRadius.x, -m_tRadius.y);
	}
	

}

TWall::TWall(const TWall & tOther)
{
	m_tPos =    tOther.m_tPos;
	m_fAngle =  tOther.m_fAngle;
	m_fRadius = tOther.m_fRadius;
	m_tRadius = tOther.m_tRadius;
	m_iColor =  tOther.m_iColor;
	m_bCollideCamera = tOther.m_bCollideCamera;
	m_bCollidePlayer = tOther.m_bCollidePlayer;
	m_bCollideProjectiles = tOther.m_bCollideProjectiles;
	m_eType = tOther.m_eType;

	m_eCollisionLayer = tOther.m_eCollisionLayer;
	m_iCollisionFlags = tOther.m_iCollisionFlags;

	if (m_eType == WALL_BOX)
	{
		AddBoxCollision(m_tRadius);
	}
	else if (m_eType == WALL_CIRCLE)
	{
		AddCircleCollision(m_fRadius);
	}

	tCornerPos[0] = tOther.tCornerPos[0];
	tCornerPos[1] = tOther.tCornerPos[1];
	tCornerPos[2] = tOther.tCornerPos[2];
	tCornerPos[3] = tOther.tCornerPos[3];



}

TWall::~TWall()
{
}

void TWall::Draw(struct TGfxSprite * pLineSprite)
{
#ifndef TFA_FRESH
	switch (m_eType)
	{

	case WALL_BOX:	DrawBox(pLineSprite, m_tPos,  m_fAngle,  m_tRadius, 	m_iColor);		


		/*GfxLineSpriteJumpTo(pLineSprite, m_tPos.x + tCornerPos[0].x, m_tPos.y + tCornerPos[0].y);
		GfxLineSpriteLineTo(pLineSprite, m_tPos.x + tCornerPos[1].x, m_tPos.y + tCornerPos[1].y);
		GfxLineSpriteLineTo(pLineSprite, m_tPos.x + tCornerPos[2].x, m_tPos.y + tCornerPos[2].y);
		GfxLineSpriteLineTo(pLineSprite, m_tPos.x + tCornerPos[3].x, m_tPos.y + tCornerPos[3].y);
		GfxLineSpriteLineTo(pLineSprite, m_tPos.x + tCornerPos[0].x, m_tPos.y + tCornerPos[0].y);
		*/


					break;
	case WALL_CIRCLE: 
		DrawCircle(pLineSprite, m_tPos, m_fRadius, m_iColor);		
					  break;
	default:
		break;
	}

#endif

}
