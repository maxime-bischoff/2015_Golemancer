#include "Const.h"
extern TGame * g_pGame;

bool IsCollideAABB(TBox tFirst, TBox tOther)
{
	if( (tFirst.tPos.x + tFirst.tRadius.x < tOther.tPos.x - tOther.tRadius.x) &&
		(tFirst.tPos.x - tFirst.tRadius.x > tOther.tPos.x + tOther.tRadius.x) &&
		(tFirst.tPos.y + tFirst.tRadius.y < tOther.tPos.y - tOther.tRadius.y) &&
		(tFirst.tPos.y - tFirst.tRadius.y < tOther.tPos.y + tOther.tRadius.y) ){return false;} else {return true;}

}

bool CollideBoundingBox(const TBoundingBox & b1,const TBoundingBox & b2)
{
	
	if( (b1.ur.x <b2.bl.x) ||
		(b1.bl.x >b2.ur.x) ||
		(b1.ur.y <b2.bl.y) ||
		(b1.bl.y >b2.ur.y)) {
	
			return false;
	} else {
		return true;}

}

bool IsInBoundingBox(const TGfxVec2 & tPos, const TBoundingBox & bb)
{
	if ((tPos.x <bb.bl.x) ||
		(tPos.x >bb.ur.x) ||
		(tPos.y <bb.bl.y) ||
		(tPos.y >bb.ur.y)) 	
		 return false;	
	else return true;
}

TBoundingBox GetSegmentBoundingBox(TGfxVec2 a1, TGfxVec2 a2)
{
	TBoundingBox tResult;
	tResult.bl.x = MMin(a1.x , a2.x);
	tResult.bl.y = MMin(a1.y , a2.y);

	tResult.ur.x = MMax(a1.x , a2.x);
	tResult.ur.y = MMax(a1.y , a2.y);
	return tResult;
}



TCollideResult CollideSegment_AABB(const TGfxVec2 & /*p1*/, const TGfxVec2 & /*p2*/, TBox & /*tOBB*/)
{

	TCollideResult tresult;
	return tresult;
}

TCollideResult CollideSegment_OBB(TBox & /*tAABB*/, TBox & /*tOBB*/)
{
	
	TCollideResult tresult;
	return tresult;
}


TCollideResult CollideAABB_OBB(TBox & tAABB, TBox & tOBB)
{
	TGfxVec2 tSegmentsAABB[4];
	TGfxVec2 tSegmentsOBB[4];
	tSegmentsAABB[0] =  TGfxVec2(tAABB.tPos.x -  tAABB.tRadius.x, tAABB.tPos.y -  tAABB.tRadius.y);
	tSegmentsAABB[1] =  TGfxVec2(tAABB.tPos.x -  tAABB.tRadius.x, tAABB.tPos.y +  tAABB.tRadius.y);
	tSegmentsAABB[2] =  TGfxVec2(tAABB.tPos.x +  tAABB.tRadius.x, tAABB.tPos.y +  tAABB.tRadius.y);
	tSegmentsAABB[3] =  TGfxVec2(tAABB.tPos.x +  tAABB.tRadius.x, tAABB.tPos.y -  tAABB.tRadius.y);



	const TGfxVec2 tAxisX = TGfxVec2(cosf(tOBB.fAngle),sinf(tOBB.fAngle));
	const TGfxVec2 tAxisY = TGfxVec2(-sinf(tOBB.fAngle),cosf(tOBB.fAngle));
	
	
	tSegmentsOBB[0] = tAABB.tPos -  tAABB.tRadius.x  * tAxisX  -  tAABB.tRadius.y * tAxisY;
	tSegmentsOBB[1] = tAABB.tPos -  tAABB.tRadius.x  * tAxisX  +  tAABB.tRadius.y * tAxisY;
	tSegmentsOBB[2] = tAABB.tPos +  tAABB.tRadius.x  * tAxisX  +  tAABB.tRadius.y * tAxisY;
	tSegmentsOBB[3] = tAABB.tPos +  tAABB.tRadius.x  * tAxisX  -  tAABB.tRadius.y * tAxisY;



//	const TGfxVec2 tAxeX = tBoxCenter + tAxisX ;
	//const TGfxVec2 tAxeY = tBoxCenter + tAxisY ;
	TGfxVec2 tIntersection(0,0);
	for (int i = 0; i<4 ; ++i)
	{
		for (int j = 0; j< 4; ++j)
		{
		
			CollideSegments(tSegmentsAABB[i], tSegmentsOBB[i%4], tSegmentsOBB[j], tSegmentsOBB[j%4],tIntersection);
		
		}
	
	
	}
	TCollideResult tresult;
	return tresult;


}

TCollideResult CollideCircleSegment(const TCircle & tCircle,const  TGfxVec2 & p1, const TGfxVec2 & p2)
{
	TCollideResult tResult;
	tResult.fLength = 0;

	const TGfxVec2 & tAtoB = ( p2 - p1 );
	const TGfxVec2 & tAtoC = ( tCircle.tPos - p1 );
	const float fLen = tAtoB.DotProduct( tAtoC ) / tAtoB.SquaredLength();
	const TGfxVec2 & tNearest = p1 + MClamp( fLen, 0,1 ) * tAtoB;
	const TGfxVec2 & tNearestToCenter = ( tCircle.tPos - tNearest );
	const float fSqDist = tNearestToCenter.SquaredLength();
	//GfxDbgPrintf("circle Pos %f \n", tCircle.tPos.y);
	if( fSqDist < tCircle.fRadius * tCircle.fRadius )
	{
		tResult.tNormal = tNearestToCenter.Normalize();
		tResult.fLength = tCircle.fRadius - (tCircle.tPos-tNearest).Length()  ;
		return tResult;
	}

	return tResult;	

}

bool CollideCircleSegmentSimple(const TCircle & tCircle, const TGfxVec2 & p1, const TGfxVec2 & p2)
{
	const TGfxVec2 & tAtoB = (p2 - p1);
	const TGfxVec2 & tAtoC = (tCircle.tPos - p1) ;

	const float fdot = tAtoB.DotProduct(tAtoC);
	
	
	if (fdot < tAtoB.Length() && fdot > 0)
	{
		const float fLen = tAtoC.SquaredLength() - (fdot * fdot);

		if (fLen < tCircle.fRadius * tCircle.fRadius)
		{
			return false;
		}
	}
	return true;
}


TTraceResult Trace(TGfxVec2 tPos, TGfxVec2 tDir, float fLength)
{
	TGfxVec2 tRay = tPos + tDir * fLength;
	TBoundingBox tBBTrace =  GetSegmentBoundingBox(tPos, tRay);
	TTraceResult tResult;
/*	tResult.bHit = false;

	for(int i=0; i< TGame::GetGame().m_tScene.iLineCount -1 ; ++i)
	{
		if(CollideBoundingBox(tBBTrace, GetSegmentBoundingBox(TGame::GetGame().m_tScene.tGround[i], TGame::GetGame().m_tScene.tGround[i+1])))
		{
		
			TGfxVec2 tIntersection;
			if(	CollideSegments(tPos, tRay, TGame::GetGame().m_tScene.tGround[i], TGame::GetGame().m_tScene.tGround[i+1], tIntersection))
			{
		
				tResult.tIntersection = tIntersection;
				tResult.bHit = true;
				return tResult;
		
			} 
		
		
		
		}
	
	}*/



	return tResult;
}
	
TTraceResult Trace2(TGfxVec2 tPos, TGfxVec2 tDir, float fLength)
{
	TGfxVec2 tRay = tPos + tDir * fLength;
	TBoundingBox tBBTrace =  GetSegmentBoundingBox(tPos, tRay);
	TTraceResult tResult;
	tResult.bHit = false;
	/*
	for(int i=0; i< TGame::GetGame().m_tScene.iLineCount -1 ; ++i)
	{
	
		
			TGfxVec2 tIntersection;
			if(	CollideSegments(tPos, tRay, TGame::GetGame().m_tScene.tGround[i], TGame::GetGame().m_tScene.tGround[i+1], tIntersection))
			{
		
				tResult.tIntersection = tIntersection;
				tResult.bHit = true;
				return tResult;
		
			} 
		
		
		
		
	
	}*/



	return tResult;
}



bool CollideSegments(const TGfxVec2 & a1, const TGfxVec2 & a2,const  TGfxVec2 & b1, const TGfxVec2 & b2,  TGfxVec2 & intersection)
{
	intersection = TGfxVec2(0,0);

	TGfxVec2 b = a2 - a1;
	TGfxVec2 d = b2 - b1;
    float bDotDPerp = b.x * d.y- b.y * d.x;
	//float bDotDPerp = b.DotProduct(d);

    // if b dot d == 0, it means the lines are parallel so have infinite intersection points
    if (bDotDPerp == 0)
        return false;

	TGfxVec2 c = b1 - a1;
    float t = (c.x * d.y - c.y * d.x) / bDotDPerp;
   if (t < 0 || t > 1)
        return false;

    float u = (c.x * b.y - c.y * b.x) / bDotDPerp;
    if (u < 0 || u > 1)
        return false;

    intersection = a1 + t * b;

    return true;
}




bool CollideCircles(TCircle & tFirst, TCircle & tOther, TCollideResult & tResult){


	TGfxVec2 tFirstPos = tFirst.tPos ;
	TGfxVec2 tOtherPos = tOther.tPos ;

	float fDistance = (tFirstPos - tOtherPos).Length();

	float fFirstRadius = tFirst.fRadius;
	float fOtherRadius = tOther.fRadius;
	float fLength = 0;
	TGfxVec2 normVec;

	if(fDistance < fFirstRadius + fOtherRadius)
	{
		fLength = (fFirstRadius + fOtherRadius) - fDistance;
		if(tFirstPos !=tOtherPos)
		{
		normVec =  (tFirstPos-tOtherPos).Normalize();
		}
		else normVec = TGfxVec2(0,0);	
	}
	
	
	tResult.fLength = fLength;
	tResult.tNormal = normVec;
	if (fLength > 0) return true; else return false;




}

bool CollideCircleBox(const TCircle & tCircle, const TBox & tBox, TCollideResult & tResult){

	





	const TGfxVec2 & tBoxCenter = tBox.tPos;
	const TGfxVec2 & tBoxRadius = tBox.tRadius;	
	const float fAngle = -tBox.fAngle;





	if (fAngle == 0){
	
	
		if ((tCircle.tPos.x + tCircle.fRadius <tBoxCenter.x - tBoxRadius.x) ||
			(tCircle.tPos.x - tCircle.fRadius >tBoxCenter.x + tBoxRadius.x) ||
			(tCircle.tPos.y + tCircle.fRadius <tBoxCenter.y - tBoxRadius.y) ||
			(tCircle.tPos.y - tCircle.fRadius >tBoxCenter.y + tBoxRadius.y)) {

			return false;
		}	
	}



	const TGfxVec2 & tCircleCenter = tCircle.tPos;
	const float & fCircleRadius = tCircle.fRadius;


	const TGfxVec2 tAxisX = TGfxVec2(cosf(fAngle),sinf(fAngle));
	const TGfxVec2 tAxisY = TGfxVec2(-sinf(fAngle),cosf(fAngle));
	
	const TGfxVec2 tAxeX = tBoxCenter + tAxisX ;
	const TGfxVec2 tAxeY = tBoxCenter + tAxisY ;

	float fDotProductX = (tCircleCenter-tBoxCenter).DotProduct(tAxeX-tBoxCenter) ;
	float fDotProductY = (tCircleCenter-tBoxCenter).DotProduct(tAxeY-tBoxCenter) ;

	const TGfxVec2 tAxeX2 = tBoxCenter + tAxisX * MClamp(fDotProductX, -tBoxRadius.x, tBoxRadius.x);
	const TGfxVec2 tAxeY2 = tBoxCenter + tAxisY * MClamp(fDotProductY, -tBoxRadius.y, tBoxRadius.y);

	const TGfxVec2 tNearestPoint = tAxeX2+tAxeY2 - tBoxCenter;

	if((tCircleCenter-tNearestPoint).Length()<fCircleRadius)
	{
		TGfxVec2 normVec;


		const TGfxVec2 tNormale = TGfxVec2(1, sin(fAngle));
		float length = fCircleRadius - (tCircleCenter-tNearestPoint).Length();

		if((tCircleCenter-tNearestPoint).Length() != 0)
		{
			normVec =  (tCircleCenter-tNearestPoint).Normalize();
		}else{
			normVec =  tCircleCenter-tNearestPoint;
		}
				
		tResult.fLength = length;
		tResult.tNormal = normVec;
		return true;


	
	}else{

		tResult.fLength = 0;
		return false;


	}

}

bool CollideCirclePolygon(const TCircle & tCircle, THitboxPolygon * tPoly, TCollideResult & tResult)
{	
	tResult.tNormal =TGfxVec2(0,0);
	tResult.fLength = 0;
	int iCount = tPoly->GetPointCount();

	TGfxVec2 tVector;
	TGfxVec2 tPos = tPoly->GetWorldPos();
	for(int i = 0; i< iCount-1; ++i)
	{
		TGfxVec2 tPointA = tPoly->GetPointCoord(i) + tPos;
		TGfxVec2 tPointB = tPoly->GetPointCoord(i+1) + tPos;
		TCollideResult tResultbis = CollideCircleSegment(tCircle, tPointA, tPointB);

		if(tResultbis.fLength>0)
		{
			tResult.tNormal += tResultbis.tNormal * tResultbis.fLength;
		}
		tResult.fLength+= tResultbis.fLength;
	}
	if (tResult.fLength > 0) return true; else return false;
	
}


bool CollideSegmentWall(const TGfxVec2 & tPos1, const TGfxVec2 & tPos2, TWall * tWall, TGfxVec2 & tRepulse)
{
	if (tWall->m_eType == WALL_BOX)
	{
		const TGfxVec2 & tUperLeft = tWall->tCornerPos[0]; 
		const TGfxVec2 & tUperRight = tWall->tCornerPos[1]; 
		const TGfxVec2 & tLowerRight = tWall->tCornerPos[2]; 
		const TGfxVec2 & tLowerLeft = tWall->tCornerPos[3];

		TGfxVec2 tIntersect;

		if (CollideSegments(tPos1, tPos2, tUperLeft, tUperRight, tRepulse))
		{
			return true;
		}
		else
		if (CollideSegments(tPos1, tPos2, tUperRight, tLowerRight, tRepulse))
		{
			return true;
		}
		else
		if (CollideSegments(tPos1, tPos2, tLowerRight, tLowerLeft, tRepulse))
		{
			return true;
		}
		else
		if (CollideSegments(tPos1, tPos2, tLowerLeft, tUperLeft, tRepulse))
		{
			return true;
		}
		return false;

	}
	else if (tWall->m_eType == WALL_CIRCLE)
	{
		TCircle tCircle;
		tCircle.tPos = tWall->m_pHitbox->GetWorldPos();
		tCircle.fRadius = tWall->m_fRadius;

		TCollideResult tResult = CollideCircleSegment(tCircle, tPos1, tPos2);

		if (tResult.fLength > 0)
			return true;
		else return false;
	}
	return false;
}


bool IsInLineOfSight(const TGfxVec2 & tPos1, const TGfxVec2 & tPos2, TWall * tWall, bool bIgnoreHole)
{
	if (tWall->m_eCollisionLayer != LAYER_WALL && bIgnoreHole) return true;
	if (tWall->m_eType == WALL_BOX)
	{
		const TGfxVec2 & tUperLeft   = tWall->tCornerPos[0];
		const TGfxVec2 & tUperRight  = tWall->tCornerPos[1];
		const TGfxVec2 & tLowerRight = tWall->tCornerPos[2];
		const TGfxVec2 & tLowerLeft  = tWall->tCornerPos[3];

		TGfxVec2 tIntersect;

		if (CollideSegments(tPos1, tPos2, tUperLeft, tUperRight, tIntersect))
		{
			return false;
		}
		else
		if (CollideSegments(tPos1, tPos2, tUperRight, tLowerRight, tIntersect))
		{
			return false;
		}
		else
		if (CollideSegments(tPos1, tPos2, tLowerRight, tLowerLeft, tIntersect))
		{
			return false;
		}
		else
		if (CollideSegments(tPos1, tPos2, tLowerLeft, tUperLeft, tIntersect))
		{
			return false;
		}
		return true;
	
	}
	else if (tWall->m_eType == WALL_CIRCLE)
	{
		TCircle tCircle;
		tCircle.tPos = tWall->m_pHitbox->GetWorldPos();
		tCircle.fRadius = tWall->m_fRadius ;

		TCollideResult tResult = CollideCircleSegment(tCircle, tPos1, tPos2);

		if (tResult.fLength > 0)	
			return false;
		else return true;
	}
	return true;
}

bool IsInLineOfSight(const TGfxVec2 & tPos1, const TGfxVec2 & tPos2,  bool bIgnoreHole)
{
	TMap * pMap = g_pGame->m_pWorld->GetCurrentMap();

	for (int iRoom = 0; iRoom < (int)pMap->m_pRooms.size(); ++iRoom)
	{
		TRoom * pRoom = pMap->m_pRooms[iRoom];
		for (int iWall = 0; iWall < pRoom->m_iWallCount   ; ++iWall)
		{
			TWall * pWall = pRoom->m_pWalls[iWall];

			if (!IsInLineOfSight(tPos1, tPos2, pWall, bIgnoreHole))
			{
				return false;
			}
		}
	}

	return true;
}



bool Collide(TTransform * tFirst, TTransform * tOther, TCollideResult & tResult){
	
	ECollisionType eFirstType = tFirst->m_pHitbox->eCollisionType;
	ECollisionType eOtherType = tOther->m_pHitbox->eCollisionType;
		

	if (tOther->m_iCollisionFlags.GetFlag(tFirst->m_eCollisionLayer) == 0) return false;
	if (!tFirst->m_bCollisionActive || !tOther->m_bCollisionActive) return false;
	
	tResult.fLength = 0;

	switch (eFirstType)
	{
	case Circle: 
					switch (eOtherType)
					{
							case Circle : {
											TCircle tFirstCircle;
											tFirstCircle.fRadius = ((THitboxCircle * )tFirst->m_pHitbox)->GetRadius();
											tFirstCircle.tPos = ((THitboxCircle * )tFirst->m_pHitbox)->GetWorldPos();

											TCircle tOtherCircle;
											tOtherCircle.fRadius = ((THitboxCircle * )tOther->m_pHitbox)->GetRadius();
											tOtherCircle.tPos = ((THitboxCircle * )tOther->m_pHitbox)->GetWorldPos();

											return CollideCircles(tFirstCircle, tOtherCircle, tResult);
											
										  }

							case Box :	 {
											TCircle tCircle;
											tCircle.fRadius = ((THitboxCircle * )tFirst->m_pHitbox)->GetRadius();
											tCircle.tPos = ((THitboxCircle * )tFirst->m_pHitbox)->GetWorldPos();

											TBox tBox;
											tBox.tRadius = ((THitboxBox * )tOther->m_pHitbox)->GetRadius();
											tBox.tPos = ((THitboxBox * )tOther->m_pHitbox)->GetWorldPos();
											tBox.fAngle = tOther->Rotation();
											return CollideCircleBox(tCircle, tBox, tResult);
											
										 }

							case Polygon :	{
												if(CollideBoundingBox(tFirst->m_pHitbox->GetBoundingBox(), tOther->m_pHitbox->GetBoundingBox()))
												{
													TCircle tCircle;
													tCircle.fRadius = ((THitboxCircle * )tFirst->m_pHitbox)->GetRadius();
													tCircle.tPos = ((THitboxCircle * )tFirst->m_pHitbox)->GetWorldPos();
													return CollideCirclePolygon( tCircle, (THitboxPolygon *) tOther->m_pHitbox, tResult);	
												}
												else return false;
												
											}

							default: return false;
										
					}
					

	case Box :
					switch (eOtherType)
					{
						case Circle:{
										TCircle tCircle;
										tCircle.fRadius = ((THitboxCircle * )tOther->m_pHitbox)->GetRadius();
										tCircle.tPos = ((THitboxCircle * )tOther->m_pHitbox)->GetWorldPos();

										TBox tBox;
										tBox.tRadius = ((THitboxBox * )tFirst->m_pHitbox)->GetRadius();
										tBox.tPos = ((THitboxBox * )tFirst->m_pHitbox)->GetWorldPos();
										tBox.fAngle = tFirst->Rotation();
										return CollideCircleBox(tCircle, tBox, tResult);
										
									}
						case Box:	
										return false;
													

						default:		return false;
										
					
					 }
	
					

	

	default : break;
			  


	}
	return false;
}