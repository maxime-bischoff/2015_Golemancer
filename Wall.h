#pragma once
#include "Solid.h"
#include "Transform.h"

enum EWallType
{
	WALL_BOX,
	WALL_CIRCLE,
	WALL_POLYGON


};


class TWall:public TTransform
{
public:
	TWall(TGfxVec2 tPos, TGfxVec2 tRadius, float fAngle, bool bCollideCamera = true, bool bCollidePlayer = true, bool bCollideProjectile = true, unsigned int iColor = EGfxColor_White);
	TWall(TGfxVec2 tPos, float fRadius, float fAngle, bool bCollideCamera = true, bool bCollidePlayer = true, bool bCollideProjectile = true, unsigned int iColor = EGfxColor_White);
	TWall(const TWall & tOther);
	~TWall();

	void Draw(struct TGfxSprite * pLineSprite);

	bool m_bCollideCamera;
	bool m_bCollidePlayer;
	bool m_bCollideProjectiles;
	EWallType m_eType;

	TGfxVec2 tCornerPos[4];
	unsigned int m_iColor;
	TGfxVec2 m_tRadius;
	float m_fRadius;



};

