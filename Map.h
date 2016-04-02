#pragma once
//#include <string>
#include "Trigger.h"
#include "Door.h"
using std::string;
using std::vector;


 


 struct TSpawner
 {
	 TGfxVec3
		m_tPos; 
	 TFlag<EEnemyType, 6> 
		 m_tEnemiesFlags;
	 float
		 m_fAngle;
	 bool 
		 m_bForceSpawn;
 };
  


 struct TGround
 {
	 TGround()
	 {
		 m_pVertices = 0;
		 m_pIndices = 0;
		 m_iTriangleCount = 0;
	 }
	 float GetHeight(const float fPosX, const float fPosY) const
	 {
		 const TGfxVec2 t2DPoint(fPosX, fPosY);

		 for (int i = 0; i < m_iTriangleCount; ++i)
		 {
			 const float * fPointA = m_pVertices + m_pIndices[i * 3 + 0] * 3;
			 const float * fPointB = m_pVertices + m_pIndices[i * 3 + 1] * 3;
			 const float * fPointC = m_pVertices + m_pIndices[i * 3 + 2] * 3;

			 const TGfxVec2 t2DPointA(fPointA[0], fPointA[2]);
			 const TGfxVec2 t2DPointB(fPointB[0], fPointB[2]);
			 const TGfxVec2 t2DPointC(fPointC[0], fPointC[2]);

			 const TGfxVec2 tNormalAB = (t2DPointB - t2DPointA).Rotate90();
			 const TGfxVec2 tNormalBC = (t2DPointC - t2DPointB).Rotate90();
			 const TGfxVec2 tNormalCA = (t2DPointA - t2DPointC).Rotate90();

			 const TGfxVec2 tPA = t2DPoint - t2DPointA;
			 const TGfxVec2 tPB = t2DPoint - t2DPointB;
			 const TGfxVec2 tPC = t2DPoint - t2DPointC;

			 const float fDotA = tNormalAB.DotProduct(tPA);
			 const float fDotB = tNormalBC.DotProduct(tPB);
			 const float fDotC = tNormalCA.DotProduct(tPC);

			 if (fDotA <= 0 && fDotB <= 0 && fDotC <= 0)
			 {
				 const TGfxVec3 t3DPointA(fPointA[0], fPointA[1], fPointA[2]);
				 const TGfxVec3 t3DPointB(fPointB[0], fPointB[1], fPointB[2]);
				 const TGfxVec3 t3DPointC(fPointC[0], fPointC[1], fPointC[2]);
				 const TGfxVec3 t3DPoint(fPosX, 0, fPosY);

				 return GetHeight(t3DPoint, t3DPointA, t3DPointB, t3DPointC);
			 }
		 }

		 return 0;
	 }

	 float GetHeight(const TGfxVec3 & p, const TGfxVec3 & a, const TGfxVec3 & b, const TGfxVec3 & c) const
	 {
		 const TGfxVec3 tNormal = (b - a).CrossProduct(c - a).Normalize();
		 return (a - p).DotProduct(tNormal) / tNormal.y;
	 }

	 const float * m_pVertices;
	 const int * m_pIndices;
	 int m_iTriangleCount;
 };


class TMap
{
public:

	TMap();
	~TMap();

	TGfxVec2 m_tPlayerSpawn;
	
	vector<TRoom *>m_pRooms;
	int m_iRoomsCount;

	float m_fMinX;
	float m_fMaxX;
	float m_fMinY;
	float m_fMaxY;

	void ReinitMap();
	
	void Update();



	void DrawMap(TGfxSprite * pLineSprite);
	void DrawMapOnMiniMap(TGfxSprite * pLineSprite);

	
	void AddWall(int iRoomIndex, TGfxVec2 tPos, TGfxVec2 tRadius, float fAngle, bool bCollideCamera = true, bool bCollidePlayer = true, bool bCollideProjectile = true, unsigned int iColor = EGfxColor_White, bool bIsADoor = false);
	void AddWall(int iRoomIndex, TGfxVec2 tPos, float fRadius, float fAngle, bool bCollideCamera = true, bool bCollidePlayer = true, bool bCollideProjectile = true, unsigned int iColor = EGfxColor_White);
	void AddDoor(int iRoomIndex, TGfxVec2 tPos, TGfxVec2 tRadius);
	void AddEnemySpawn(int iRoomIndex, TGfxVec2 tPos);

	void NotifyEnemyKill(const TEnemy * pEnemy);

	void ExportMapToFresh();

	void Clean();

	void PlaceDoors(enum ERoomType eGameType);

	void PrepareMap();

	bool CollideOnMap(TTransform * pObject, bool & bContinue, TCollideResult & tResult);
	void CollideOnMap(TDamage * pDamage);
	void CollideOnMap2(TTransform * pObject);

	int GetRoom(const TGfxVec2 & tPos);
	float GetHeight(const TGfxVec2 & tPos, float fCurrentHeight = 0);

	vector<TDoor * > m_vpDoors;
};