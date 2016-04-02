
#include "Const.h"



// ************ HITBOX ********************** //
// ****************************************** //


THitbox::THitbox(void)
{
	m_BoundingBox.bl = TGfxVec2(0,0);
	m_BoundingBox.ur = TGfxVec2(0,0);

	m_iHitboxColor = GfxColor(255, 255, 0, 255);
}


THitbox::~THitbox(void)
{
	m_tTransform = 0;
}

TGfxVec2 THitbox::GetWorldPos()
{
	return m_tTransform->Position() + m_tPos;

}

TBoundingBox THitbox::GetBoundingBox()
{
	TBoundingBox tResult;
	tResult.bl = m_BoundingBox.bl +  m_tTransform->Position();
	tResult.ur = m_BoundingBox.ur + m_tTransform->Position();

	return tResult;
}
#ifndef TFA_FRESH
void THitbox::Draw(TGfxSprite * pLineSprite){
	pLineSprite;
	return;
}
#endif


// ************ HITBOX CIRCLE  ************** //
// ****************************************** //


THitboxCircle::THitboxCircle(float fRadius, TGfxVec2 tPos,TTransform * tTransform):
	m_fRadius(fRadius)				
{
	m_BoundingBox.bl = TGfxVec2(-fRadius, -fRadius);
	m_BoundingBox.ur = TGfxVec2(fRadius, fRadius);
	m_tPos = tPos;
	m_tTransform = tTransform;
	eCollisionType = Circle;

}

THitboxCircle::~THitboxCircle(void)
{
}

#ifndef TFA_FRESH
void THitboxCircle::Draw(TGfxSprite * pLineSprite)
{
	DrawCircle(pLineSprite, GetWorldPos(), m_fRadius, m_iHitboxColor, 10);


}
#endif





// ************ HITBOX BOX  ***************** //
// ****************************************** //


THitboxBox::THitboxBox(TGfxVec2 tRadius, TGfxVec2 tPos, TTransform * tTransform)
{
	eCollisionType = Box;
	m_tRadius = tRadius;
	m_tPos = tPos;
	m_tTransform= tTransform;

}

THitboxBox::~THitboxBox(void)
{
}
#ifndef TFA_FRESH
void THitboxBox::Draw(TGfxSprite * pLineSprite)
{
	DrawBox(pLineSprite, GetWorldPos(), m_tTransform->Rotation(), m_tRadius, m_iHitboxColor);

}
#endif

// ************ HITBOX CAPSULE  ************* //
// ****************************************** //


THitboxCapsule::THitboxCapsule(float fRadius,float fSize, TGfxVec2 tPos, TTransform * tTransform)
{
	eCollisionType = Capsule;
	m_fRadius = fRadius;
	m_fSize = fSize;
	m_tPos = tPos;
	m_tTransform= tTransform;

}

THitboxCapsule::~THitboxCapsule(void)
{
}




THitboxTriangle::THitboxTriangle(TGfxVec2 /*tRadius*/, TGfxVec2 /*tPos*/, TTransform * /*tTransform*/, int /*iType*/)
{
	

}

// ************ HITBOX POLYGON  ************* //
// ****************************************** //

THitboxPolygon::THitboxPolygon( TTransform * tTransform):
	m_iPointCount(0)
	
	
{
	m_tPos = TGfxVec2(0,0);
	eCollisionType = Polygon;
	m_tTransform = tTransform;
}

THitboxPolygon::~THitboxPolygon(void)
{
}
TGfxVec2 THitboxPolygon::GetPointCoord(int iPoint) const 
{

	if(iPoint < m_iPointCount)
	{
		return (m_tPointArray[iPoint]);

	}else return (TGfxVec2(404,404));

}

TGfxVec2 THitboxPolygon::GetWorldPointCoord(int iPoint) const 
{

	if(iPoint < m_iPointCount)
	{
		return (m_tPointArray[iPoint]);

	}else return (TGfxVec2(404,404));

}
#ifndef TFA_FRESH
void THitboxPolygon::DrawHitbox(TGfxSprite * pLineSprite)
{
	float fPosX = m_tTransform->Position().x;
	float fPosY = m_tTransform->Position().y;
	GfxLineSpriteJumpTo(pLineSprite, m_tPointArray[0].x + fPosX, m_tPointArray[0].y  + fPosY);
	for(int i = 0; i< m_iPointCount; ++i)
	{
		GfxLineSpriteLineTo(pLineSprite, m_tPointArray[i].x + fPosX, m_tPointArray[i].y + fPosY);	

	}
}
#endif


void THitboxPolygon::RefreshBoundingBox()
{

	for(int i = 0; i< m_iPointCount; ++i)
	{
		m_BoundingBox.ur.x = MMax(m_BoundingBox.ur.x, m_tPointArray[i].x);
		m_BoundingBox.ur.y = MMax(m_BoundingBox.ur.y, m_tPointArray[i].y);
		m_BoundingBox.bl.x = MMin(m_BoundingBox.bl.x, m_tPointArray[i].x);
		m_BoundingBox.bl.y = MMin(m_BoundingBox.bl.y, m_tPointArray[i].y);
	
	
	}


}

void THitboxPolygon::AddPoint(float fX, float fY)
{
	if(m_iPointCount< MAX_POINTS)
	{
		m_tPointArray[m_iPointCount] = TGfxVec2(fX, fY);	
		m_iPointCount++;	
		RefreshBoundingBox();
	}

}