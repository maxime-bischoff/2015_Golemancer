
#pragma once 
class TTransform;
class THitboxBox;
class THitboxCircle;
struct TBoundingBox;
#include "Collision.h"

enum ECollisionType{
Circle,
Box,
Triangle,
Capsule,
Polygon
};


class THitbox
{
public:
	ECollisionType eCollisionType;
	TGfxVec2 GetWorldPos();
	 TBoundingBox GetBoundingBox();
#ifndef TFA_FRESH
	 virtual void Draw(TGfxSprite * pLineSprite);
#endif
	THitbox(void);
	~THitbox(void);
protected :

	unsigned int m_iHitboxColor;
	TTransform * m_tTransform;
	TGfxVec2 m_tPos;
	TBoundingBox m_BoundingBox;
	
};








// ****** HitBox d'un cercle ******* //

class THitboxCircle: public THitbox
{
public:
	THitboxCircle(float fRadius, TGfxVec2 tPos, TTransform * tTransform);
	~THitboxCircle(void);
	float GetRadius(){return m_fRadius;};
	void  SetRadius(float fRadius){ m_fRadius = fRadius; };
#ifndef TFA_FRESH
	void Draw(TGfxSprite * pLineSprite);
#endif
private :
	float m_fRadius;
	
};


// ****** HitBox d'une box ******* //

class THitboxBox: public THitbox
{
public:	
	THitboxBox(TGfxVec2 tRadius, TGfxVec2 tPos, TTransform * tTransform);
	~THitboxBox(void);
	TGfxVec2 GetRadius(){return m_tRadius;};
#ifndef TFA_FRESH
	void Draw(TGfxSprite * pLineSprite);
#endif
private :

	TGfxVec2 m_tRadius;

};

// ****** HitBox d'une capsule ******* //

class THitboxCapsule: public THitbox
{
public:


	THitboxCapsule(float fRadius, float fSize, TGfxVec2 tPos, TTransform * tTransform);
	~THitboxCapsule(void);

	float GetRadius(){return m_fRadius;};
	float GetSize(){return m_fSize;};

private : 
	float m_fRadius;
	float m_fSize;
};




class THitboxPolygon: public THitbox
{

public:

	void AddPoint(float fX, float fY);
	void RefreshBoundingBox();
#ifndef TFA_FRESH
	void DrawHitbox(TGfxSprite * pLineSprite);
#endif
	TGfxVec2 GetPointCoord(int iPoint) const;
	TGfxVec2 GetWorldPointCoord(int iPoint) const;
	THitboxPolygon( TTransform * tTransform);
	int GetPointCount(){return m_iPointCount;};
	~THitboxPolygon(void);

private :

	static const int MAX_POINTS = 50;
	int m_iPointCount;
	TGfxVec2 m_tPointArray[MAX_POINTS];



};




// ******* HitBox d'un triangle rectangle isocele ****** //
class THitboxTriangle: public THitbox
{
public:
	TGfxVec2 m_tRadius;

	TGfxVec2 GetRadiusBox(){return m_tRadius;};
	TGfxVec2 GetNormal(){return m_tNormal;};
	TGfxVec2 GetTangent(){return m_tTangent;};

	int GetType(){return m_iType;};
	float GetHypo(){return m_fHypo;};

	THitboxTriangle(TGfxVec2 tRadius, TGfxVec2 tPos, TTransform * tTransform, int iType);

	// void SetRadiusBox(TGfxVec2 tRadius){m_tRadius = tRadius;};

	TGfxVec2 m_tNormal;
	TGfxVec2 m_tTangent;
	float  m_fHypo;
	int m_iType;

	~THitboxTriangle(void);
};













