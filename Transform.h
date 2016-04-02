#pragma once 

#include "flib_vec2.h"
#include "Collision.h"
class THitbox;

	enum TType
	{
		TType_Player,
		TType_Enemy,
		TType_Other,
	};



	enum ECollisionLayer
	{
		LAYER_WALL = 0,
		LAYER_HOLE = 1,
		LAYER_SEMI_WALL = 2,
		LAYER_CHARACTER = 3,
		LAYER_PROJECTILE = 4,	
		LAYER_CAMERA = 5,
		LAYER_TRIGGER = 6,
		LAYER_COUNT = 7
	};


	template <typename E, int iSize> class TFlag 
	{	
	private :
		int m_iFlags;
		//int m_iSize;

	public :
		TFlag() : m_iFlags( 0 ) {};
		bool GetFlag(E flag){ return (m_iFlags & (1 << (int)flag)) != 0 ; }
		void SetFlag(E flag, bool active = true){ m_iFlags |= active << (int)flag; }
		void Clear(){ m_iFlags = 0; }
		void SetEverything(){ m_iFlags = 0 | ((1 << iSize) - 1); }
		int GetFlags(){ return m_iFlags; }
		int GetSize(){ return iSize; }

	};
	


class TTransform
{
	


public:

	ECollisionLayer m_eCollisionLayer;
	TFlag<ECollisionLayer, LAYER_COUNT> m_iCollisionFlags;

	TTransform(TGfxVec2 tPos, float fAngle = 0);
	TTransform();

	virtual ~TTransform();





	// ***** Accessor
	float Rotation(){return m_fAngle;};
	TGfxVec2 Position(){return m_tPos;};

	// ***** Mutator
	void Rotation (float fAngle);
	void Rotate (float fAngle);
	void Position(TGfxVec2 tPos){m_tPos = tPos;};
	void Translate(TGfxVec2 tVector);
	bool Move(TGfxVec2 tVector, int iNbStep = 3);
	void OrientedTranslate(float fSpeed);


	bool AddBoxCollision(TGfxVec2 tRadius, TGfxVec2 tPos =  TGfxVec2(0,0));
	bool AddCircleCollision(float fRadius, TGfxVec2 tPos = TGfxVec2(0,0));

	bool AddCapsculeCollision(float fRadius, float fSize, TGfxVec2 tPos);
	bool AddPolygonCollision();

	virtual void CollideWall(TCollideResult & tResult){ tResult = tResult; };
	
	
	THitbox * m_pHitbox;

	bool m_bNeedToBeDestroyed ;
	bool m_bCollisionActive;
	bool m_bInterruptCollision;

protected :


	TGfxVec2 m_tPos;
	
	//TGfxVec2 m_tPos;
	float m_fAngle;


};


