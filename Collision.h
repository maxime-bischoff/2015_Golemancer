#pragma once 


class TTransform;
struct TGfxVec2;
class TWall;

struct TCollideResult
{
	TTransform * self;
	TTransform * other;
	TGfxVec2 tNormal;
	float fLength;
};

struct TTraceResult
{
	TTransform * self;
	TTransform * other;
	TGfxVec2 tNormal;
	TGfxVec2 tIntersection;
	bool bHit;
	
};

struct TCircle
{
	float fRadius;	
	TGfxVec2 tPos;
	
};

struct TBox
{
	TGfxVec2 tRadius;	
	TGfxVec2 tPos;	
	float fAngle;
};

struct  TBoundingBox
{
	struct TGfxVec2 bl; // Bottom Left corner
	struct TGfxVec2 ur; // Up Right Corner


};

struct TSegment
{
	TGfxVec2 t1;	
	TGfxVec2 t2;	

};

TTraceResult Trace(TGfxVec2 tPos, TGfxVec2 tDir, float fLength);
TTraceResult Trace2(TGfxVec2 tPos, TGfxVec2 tDir, float fLength);
bool CollideBoundingBox(const TBoundingBox & b1,const TBoundingBox & b2);
bool IsInBoundingBox(const TGfxVec2 & tPos, const TBoundingBox & bb);
TBoundingBox GetSegmentBoundingBox(TGfxVec2 a1, TGfxVec2 a2);
bool  CollideCircleSegment(TCircle tCircle, TGfxVec2 p1, TGfxVec2 p2, TCollideResult & tResult);
bool CollideSegments(const TGfxVec2 & a1, const TGfxVec2 & a2,const  TGfxVec2 & b1, const TGfxVec2 & b2,  TGfxVec2 & intersection);
bool GetIntersection(TGfxVec2 a1, TGfxVec2 a2, TGfxVec2 b1, TGfxVec2 b2, float &fX, float &fY);
bool CollideCircles(TCircle & tFirst, TCircle & tOther, TCollideResult & tResult); 
bool  CollideSegmentWall(const TGfxVec2 & tPos1, const TGfxVec2 & tPos2, TWall * tWall, TGfxVec2 & tRepulse);
TCollideResult CollideSegment_AABB(const TGfxVec2 & p1, const TGfxVec2 & p2, TBox & tOBB);

TCollideResult CollideSegment_OBB(TBox & tAABB, TBox & tOBB);

bool CollideCircleBox(const TCircle & tCircle,const  TBox & tBox, TCollideResult & tResult);
bool Collide(TTransform * tFirst, TTransform * tOher, TCollideResult & tResult);

bool IsInLineOfSight(const TGfxVec2 & tPos1, const TGfxVec2 & tPos2, TWall * tWall, bool bIgnoreHole = true);
bool IsInLineOfSight(const TGfxVec2 & tPos1, const TGfxVec2 & tPos2, bool bIgnoreHole = true);


