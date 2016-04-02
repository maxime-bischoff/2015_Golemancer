#pragma once

#include "Transform.h"

enum EDamageForm
{
	FORM_CIRCLE, // Damage is a circle
	FORM_BOX, // Damage is a Rectangle
	FORM_ARC // Damage is an cone
};

struct TDamageData  // Durée de vie / Speed / GrowingSpeed / Radius / DamageValue / delay / Form / Visual Aspect : 0 = Blue Ball, 1 = Primitive Box
{
	float m_fLifeSpan ;
	float m_fSpeed ;
	float m_fGrowingSpeed;
	TGfxVec2 m_tRadius ;
	float m_fDamageValue ;
	float m_fDelay;
	EDamageForm eForm;
	int m_iStyle;
	float m_fHeight;
};

enum ETypeDamage
{
	TYPE_LINEAR, // Devant soi
	TYPE_CURVE, // Sur le coté
	TYPE_BOUNCE, // Rebond
	TYPE_ROOT, // Root enemy in zone
	TYPE_LASER,
	TYPE_IMPACT,
	TYPE_STUN,
};

enum EDamageFaction
{
	FACTION_PLAYER,
	FACTION_ENEMIES,
	FACTION_NEUTRAL
};

class TDamage :
	public TTransform
{
	 // :: PRIVATE :: //
private :

	TGfxVec2
		m_tDirection;
	TEnemy *
		m_Target;
	ETypeDamage
		m_eType;
	EDamageFaction
		m_eFaction;
	float
		m_fLaserLenght;
	unsigned int
		m_iColor;

	// :: PUBLIC :: //
public:

	// -- VARIABLES
	struct TDamageData
		m_tData;

	TGfxVec2
		m_tOrigin,
		m_tDirectionLaser;

	// -- FUNCTIONS

		// CONSTRUCTOR
	TDamage(struct TDamageData &m_tDataDmg, const ETypeDamage &eType, const TGfxVec2 &tPos, const float fAngle, EDamageFaction eFaction, unsigned int iColor = EGfxColor_Red, float fDiffAngle = 0, float fDelay = 0, TEnemy * tTarget = 0);
	virtual ~TDamage();

		// ACCESSORS
	float
		GetLaserLenght() { return m_fLaserLenght; };
	void
		SetLaserLenght(float fNewLenght) { m_fLaserLenght = fNewLenght; }
	EDamageFaction
		GetFaction() { return m_eFaction; }
	ETypeDamage
		GetType() { return m_eType; }
	TGfxVec2
		GetSpeed() { return m_tDirection * m_tData.m_fSpeed; }

		//
	bool
		CollideEnemy(class TEnemy &tEnemy, struct TCollideResult  &tResult, int iIndexEnemy ),
		CollidePlayer( struct TCollideResult  &tResult),
		CollideFocus(struct TCollideResult  &tResult);

	void
		CollideWall(TCollideResult & tResult),
		Update(),
		Render();
};
