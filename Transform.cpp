
#include "Const.h"

extern TGame * g_pGame;

TTransform::TTransform(TGfxVec2 tPos, float fAngle) :
m_tPos(tPos),
m_fAngle(fAngle),
m_pHitbox(0),
m_bNeedToBeDestroyed(false),
m_bCollisionActive(true)
{
	m_eCollisionLayer = LAYER_WALL;
	m_iCollisionFlags.SetEverything();	
}


TTransform::TTransform() :
m_tPos(TGfxVec2(0,0)),
m_fAngle(0),
m_pHitbox(0),
m_bNeedToBeDestroyed(false),
m_bCollisionActive(true)
{
	m_eCollisionLayer = LAYER_WALL;
	m_iCollisionFlags.SetEverything();
}

TTransform::~TTransform()
{
	if(m_pHitbox!=0)delete m_pHitbox;
	m_pHitbox=0;

}

void TTransform::Rotation(float fAngle)
{
	this->m_fAngle = fAngle;

}
void TTransform::Rotate(float fAngle)
{
	this->m_fAngle += fAngle;

}


void TTransform::Translate(TGfxVec2 tVector)
{
	m_tPos+= tVector;
}
bool TTransform::Move(TGfxVec2 tVector,  int iNbStep)
{
	TMap *	pActiveMap = g_pGame->m_pWorld->GetCurrentMap();

	for (int i = 0; i < iNbStep; i++)
	{
		m_tPos += tVector / static_cast < float > ( iNbStep ) ;
		pActiveMap->CollideOnMap2(this);
	}
	return false;
}
void TTransform::OrientedTranslate(float fSpeed)
{
	m_tPos.x += cosf(m_fAngle ) * fSpeed;
	m_tPos.y += sinf(m_fAngle  ) * fSpeed;


}


bool TTransform::AddBoxCollision(TGfxVec2 tRadius, TGfxVec2 tPos)
{
	if(m_pHitbox == 0)
	{
		m_pHitbox = new THitboxBox(tRadius, tPos, this);
		return true;
	}
	else return false;

}

bool TTransform::AddCircleCollision(float fRadius, TGfxVec2 tPos)
{
	if(m_pHitbox == 0)
	{
		m_pHitbox = new THitboxCircle(fRadius, tPos, this);
		return true;
	}
	else return false;

}



bool TTransform::AddCapsculeCollision(float fRadius, float fSize, TGfxVec2 tPos)
{
	if(m_pHitbox == 0)
	{
		m_pHitbox = new THitboxCapsule(  fRadius,  fSize, tPos, this);
		return true;
	}
	else return false;

}

bool TTransform::AddPolygonCollision()
{
	if(m_pHitbox ==0)
	{
		m_pHitbox = new THitboxPolygon(this);
		return true;
	}
	else return false;


}

