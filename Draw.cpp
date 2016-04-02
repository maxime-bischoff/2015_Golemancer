#include "Const.h"

#ifndef TFA_FRESH
void DrawOrientedCircle(TGfxSprite * pLineSprite, const TGfxVec2 & tPos, float fAngle, float fRadius, unsigned int iColor, int iSegment )
{
	//tPos.y *=-1;
	GfxLineSpriteSetDrawingColor(pLineSprite, iColor);

	GfxLineSpriteJumpTo(pLineSprite, tPos.x, tPos.y);
	//float lvlDetail = ( (2*PI)  /  (2*PI*fRadius ) ) * 20.0f  ;	
	float detail =( 2.0f * PI )/ iSegment ;

	for (float angle = fAngle; angle<2.0f*PI + fAngle; angle += detail)
	{
		float iPosX = cosf(angle) * fRadius + tPos.x;
		float iPosY = sinf(angle)* fRadius + tPos.y;
		GfxLineSpriteLineTo(pLineSprite, iPosX, iPosY);

	}

	float iPosX = cosf(fAngle) * fRadius + tPos.x;
	float iPosY = sinf(fAngle)* fRadius + tPos.y;
	GfxLineSpriteLineTo(pLineSprite, iPosX, iPosY);


}

void DrawCircle(TGfxSprite * pLineSprite, TGfxVec2  tPos, float fRadius, unsigned int iColor, int iSegment )
{


	GfxLineSpriteSetDrawingColor(pLineSprite, iColor);

	float iPosX = 1.0f * fRadius + tPos.x;
	float iPosY = 0.0f* fRadius + tPos.y;
	//float fAngle = 0.0f;
	GfxLineSpriteJumpTo(pLineSprite, iPosX, iPosY);
	//float lvlDetail = (2*M_PI)  /  (2*M_PI*fRadius )  ;	

	//for(float angle= 0; angle<2*M_PI ; angle+=lvlDetail*10.0f)
	float detail = PI / iSegment;
	for (float theta = 0.0f; theta<6.28f; theta += detail)
	{
		float iPosX = cosf(theta) * fRadius + tPos.x;
		float iPosY = sinf(theta)* fRadius + tPos.y;
		GfxLineSpriteLineTo(pLineSprite, iPosX, iPosY);

	}

	GfxLineSpriteLineTo(pLineSprite, iPosX, iPosY);
}

void DrawPartialCircle(TGfxSprite * pLineSprite, const TGfxVec2 & tPos, float fAngle, float fRadius, float fRange, unsigned int iColor, int iSegment )
{
	GfxLineSpriteSetDrawingColor(pLineSprite, iColor);

	GfxLineSpriteJumpTo(pLineSprite, tPos.x, tPos.y);
	float lvlDetail =  fRange / (iSegment*1.0f);

	for (float angle = fAngle - fRange / 2.0f; angle < fAngle + fRange / 2.0f; angle += lvlDetail)
	{
		float iPosX = cosf(angle) * fRadius + tPos.x;
		float iPosY = sinf(angle)* fRadius + tPos.y;
		GfxLineSpriteLineTo(pLineSprite, iPosX, iPosY);

	}

	float iPosX = cosf(fAngle + fRange / 2) * fRadius + tPos.x;
	float iPosY = sinf(fAngle + fRange / 2)* fRadius + tPos.y;
	GfxLineSpriteLineTo(pLineSprite, iPosX, iPosY);

	GfxLineSpriteLineTo(pLineSprite, tPos.x, tPos.y);
}


void DrawCamera(struct TGfxSprite * pLineSprite, const  TGfxVec2 & tPos, float fFOV, const  TGfxVec2 & tLookat,float fSize, unsigned int iColor)
{
	

	GfxLineSpriteSetDrawingColor(pLineSprite, iColor);
	GfxLineSpriteJumpTo(pLineSprite, tPos.x, tPos.y);

	TGfxVec2 tCamDir = tLookat - tPos;
	float fAngle = atan2f(tCamDir.y, tCamDir.x);

	fFOV = fFOV * PI / 180.0f;
	GfxLineSpriteLineTo(pLineSprite, tPos.x + cosf(fAngle - fFOV / 2.0f) * fSize, tPos.y + sinf(fAngle - fFOV / 2.0f) * fSize);
	GfxLineSpriteLineTo(pLineSprite, tPos.x + cosf(fAngle + fFOV / 2.0f) * fSize, tPos.y + sinf(fAngle + fFOV / 2.0f) * fSize);
	GfxLineSpriteLineTo(pLineSprite, tPos.x, tPos.y);


	DrawCircle(pLineSprite, tLookat, 0.2f, iColor);


}


void DrawShatteredCircle(TGfxSprite * pLineSprite, const  TGfxVec2 & tPos, float fAngle, float fRadius, unsigned int iColor )
{
	GfxLineSpriteSetDrawingColor(pLineSprite, iColor);

	GfxLineSpriteJumpTo(pLineSprite, tPos.x, tPos.y);
	float lvlDetail = (2.0f*PI) / (2.0f*PI*20);
	fAngle = 0;
	for (float angle = fAngle; angle<2.0f*PI + fAngle; angle += lvlDetail)
	{
		if (fmod(angle, 6.28f / 7.0f) > 0.5)
		{

			float iPosX = cosf(angle) * fRadius + tPos.x;
			float iPosY = sinf(angle)* fRadius + tPos.y;
			GfxLineSpriteLineTo(pLineSprite, iPosX, iPosY);
		}
		else{
			float iPosX = cosf(angle) * fRadius + tPos.x;
			float iPosY = sinf(angle)* fRadius + tPos.y;
			GfxLineSpriteJumpTo(pLineSprite, iPosX, iPosY);

		}

	}


}











void DrawBox(TGfxSprite * pLineSprite, const  TGfxVec2 & tPos, float fAngle, const TGfxVec2  & tRadius, unsigned int iColor)
{
	
	if (fAngle == 0)
	{
		const TGfxVec2 & tCenter = tPos;


		GfxLineSpriteSetDrawingColor(pLineSprite, iColor);




		GfxLineSpriteSetDrawingColor(pLineSprite, iColor);
		const TGfxVec2 tCornerUR = TGfxVec2(tCenter.x + tRadius.x, tCenter.y + tRadius.y);
		const TGfxVec2 tCornerDR = TGfxVec2(tCenter.x + tRadius.x, tCenter.y - tRadius.y);
		const TGfxVec2 tCornerDL = TGfxVec2(tCenter.x - tRadius.x, tCenter.y - tRadius.y);
		const TGfxVec2 tCornerUL = TGfxVec2(tCenter.x - tRadius.x, tCenter.y + tRadius.y);

		GfxLineSpriteJumpTo(pLineSprite, tCornerUR.x, tCornerUR.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerDR.x, tCornerDR.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerDL.x, tCornerDL.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerUL.x, tCornerUL.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerUR.x, tCornerUR.y);


	}
	else{

		const TGfxVec2 & tCenter = tPos;


		//TGfxVec2 tCornerUR, tCornerDR, tCornerDL, tCornerUL;

		const TGfxVec2 tAxisX = TGfxVec2(cosf(fAngle), -sinf(fAngle));
		const TGfxVec2 tAxisY = TGfxVec2(sinf(fAngle), cosf(fAngle));


		GfxLineSpriteSetDrawingColor(pLineSprite, iColor);
		const TGfxVec2 tCornerUR = tCenter + tAxisX * tRadius.x + tAxisY * tRadius.y;
		const TGfxVec2 tCornerDR = tCenter + tAxisX * tRadius.x - tAxisY * tRadius.y;
		const TGfxVec2 tCornerDL = tCenter - tAxisX * tRadius.x - tAxisY * tRadius.y;
		const TGfxVec2 tCornerUL = tCenter - tAxisX * tRadius.x + tAxisY * tRadius.y;

		GfxLineSpriteJumpTo(pLineSprite, tCornerUR.x, tCornerUR.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerDR.x, tCornerDR.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerDL.x, tCornerDL.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerUL.x, tCornerUL.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerUR.x, tCornerUR.y);
	}


}

void DrawCrossBox(TGfxSprite * pLineSprite, const  TGfxVec2 & tPos, float fAngle, const TGfxVec2  & tRadius, unsigned int iColor)
{

	if (fAngle == 0)
	{
		const TGfxVec2 & tCenter = tPos;

		GfxLineSpriteSetDrawingColor(pLineSprite, iColor);

		GfxLineSpriteSetDrawingColor(pLineSprite, iColor);
		const TGfxVec2 tCornerUR = TGfxVec2(tCenter.x + tRadius.x, tCenter.y + tRadius.y);
		const TGfxVec2 tCornerDR = TGfxVec2(tCenter.x + tRadius.x, tCenter.y - tRadius.y);
		const TGfxVec2 tCornerDL = TGfxVec2(tCenter.x - tRadius.x, tCenter.y - tRadius.y);
		const TGfxVec2 tCornerUL = TGfxVec2(tCenter.x - tRadius.x, tCenter.y + tRadius.y);

		GfxLineSpriteJumpTo(pLineSprite, tCornerUR.x, tCornerUR.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerDR.x, tCornerDR.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerDL.x, tCornerDL.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerUL.x, tCornerUL.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerUR.x, tCornerUR.y);

		GfxLineSpriteJumpTo(pLineSprite, tCornerUL.x, tCornerUL.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerDR.x, tCornerDR.y);
		GfxLineSpriteJumpTo(pLineSprite, tCornerUR.x, tCornerUR.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerDL.x, tCornerDL.y);
		


	}
	else{

		const TGfxVec2 & tCenter = tPos;


		//TGfxVec2 tCornerUR, tCornerDR, tCornerDL, tCornerUL;

		const TGfxVec2 tAxisX = TGfxVec2(cosf(fAngle), -sinf(fAngle));
		const TGfxVec2 tAxisY = TGfxVec2(sinf(fAngle), cosf(fAngle));


		GfxLineSpriteSetDrawingColor(pLineSprite, iColor);
		const TGfxVec2 tCornerUR = tCenter + tAxisX * tRadius.x + tAxisY * tRadius.y;
		const TGfxVec2 tCornerDR = tCenter + tAxisX * tRadius.x - tAxisY * tRadius.y;
		const TGfxVec2 tCornerDL = tCenter - tAxisX * tRadius.x - tAxisY * tRadius.y;
		const TGfxVec2 tCornerUL = tCenter - tAxisX * tRadius.x + tAxisY * tRadius.y;

		GfxLineSpriteJumpTo(pLineSprite, tCornerUR.x, tCornerUR.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerDR.x, tCornerDR.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerDL.x, tCornerDL.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerUL.x, tCornerUL.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerUR.x, tCornerUR.y);

		GfxLineSpriteJumpTo(pLineSprite, tCornerUL.x, tCornerUL.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerDR.x, tCornerDR.y);
		GfxLineSpriteJumpTo(pLineSprite, tCornerUR.x, tCornerUR.y);
		GfxLineSpriteLineTo(pLineSprite, tCornerDL.x, tCornerDL.y);
	}


}

void DrawGraduation(TGfxSprite * pLineSprite, float fUnit, float fWidth, unsigned int iColor, TGfxVec2 tPos){

	GfxLineSpriteSetDrawingColor(pLineSprite, iColor);

	for (int i = 0; i< fWidth / fUnit; ++i)
	{
		GfxLineSpriteJumpTo(pLineSprite, -fWidth + tPos.x, (i + 1)* fUnit + tPos.y);
		GfxLineSpriteLineTo(pLineSprite, fWidth + tPos.x, (i + 1) * fUnit + tPos.y);
		GfxLineSpriteJumpTo(pLineSprite, -fWidth + tPos.x, (-i - 1) * fUnit + tPos.y);
		GfxLineSpriteLineTo(pLineSprite, fWidth + tPos.x, (-i - 1) * fUnit + tPos.y);
	}

	for (int i = 0; i< fWidth / fUnit; ++i)
	{
		GfxLineSpriteJumpTo(pLineSprite, (i + 1) * fUnit + tPos.x, -fWidth + tPos.y);
		GfxLineSpriteLineTo(pLineSprite, (i + 1)* fUnit + tPos.x, fWidth + tPos.y);
		GfxLineSpriteJumpTo(pLineSprite, (-i - 1)* fUnit + tPos.x, -fWidth + tPos.y);
		GfxLineSpriteLineTo(pLineSprite, (-i - 1)* fUnit + tPos.x, fWidth + tPos.y);
	}
}

void DrawGrid(TGfxSprite * pLineSprite, float  fGridWidth, float  fUnit1, float fUnit2, unsigned int iUnit1Color , unsigned int iUnit2Color , unsigned int iOriginColor , TGfxVec2 tPos)
{
	DrawGraduation(pLineSprite, fUnit1, fGridWidth, iUnit1Color, tPos);
	DrawGraduation(pLineSprite, fUnit2, fGridWidth, iUnit2Color, tPos);


	GfxLineSpriteSetDrawingColor(pLineSprite, iOriginColor);
	GfxLineSpriteJumpTo(pLineSprite, -fGridWidth, 0);
	GfxLineSpriteLineTo(pLineSprite, fGridWidth, 0);
	GfxLineSpriteJumpTo(pLineSprite, 0, -fGridWidth);
	GfxLineSpriteLineTo(pLineSprite, 0, fGridWidth);

}

void DrawArrow(TGfxSprite * pLineSprite, const TGfxVec2 & tStart,const TGfxVec2 & tEnd, float fSize, unsigned int iColor)
{
	GfxLineSpriteSetDrawingColor(pLineSprite, iColor);
	GfxLineSpriteJumpTo(pLineSprite, tStart.x, tStart.y);
	GfxLineSpriteLineTo(pLineSprite, tEnd.x, tEnd.y);
	TGfxVec2 tVector = tStart - tEnd;
	float fAngle = atan2f(tVector.y, tVector.x);

	float fOpenArrow = PI / 7.0f;
	GfxLineSpriteJumpTo(pLineSprite, tEnd.x, tEnd.y);
	GfxLineSpriteLineTo(pLineSprite, cosf(fAngle - fOpenArrow) * fSize + tEnd.x, sinf(fAngle - fOpenArrow) * fSize + tEnd.y);

	GfxLineSpriteJumpTo(pLineSprite, tEnd.x, tEnd.y);
	GfxLineSpriteLineTo(pLineSprite, cosf(fAngle + fOpenArrow) * fSize + tEnd.x, sinf(fAngle + fOpenArrow) * fSize + tEnd.y);

}
#endif