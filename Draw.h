#pragma once

#ifndef TFA_FRESH
struct TGfxVec2;
void DrawOrientedCircle(struct TGfxSprite * pLineSprite, const  TGfxVec2 & tPos, float fAngle, float fRadius, unsigned int iColor, int iSegment = 10);

void DrawCircle(struct TGfxSprite * pLineSprite, struct TGfxVec2  tPos, float fRadius, unsigned int iColor, int iSegment = 10);

void DrawPartialCircle(TGfxSprite * pLineSprite, const TGfxVec2 & tPos, float fAngle, float fRadius, float fRange, unsigned int iColor, int iSegment = 10);

void DrawCamera(struct TGfxSprite * pLineSprite, const  TGfxVec2 & tPos, float fFOV, const  TGfxVec2 & tLookat,float fSize, unsigned int iColor);



void DrawBox(TGfxSprite * pLineSprite, const  TGfxVec2 & tPos, float fAngle, const TGfxVec2  & tRadius, unsigned int iColor);
void DrawCrossBox(TGfxSprite * pLineSprite, const  TGfxVec2 & tPos, float fAngle, const TGfxVec2  & tRadius, unsigned int iColor);


void DrawGraduation(TGfxSprite * pLineSprite, float fUnit, float fWidth, unsigned int iColor, TGfxVec2 tPos = TGfxVec2(0, 0));

void DrawGrid(TGfxSprite * pLineSprite, float fGridWidth, float fUnit1, float fUnit2,
				unsigned int iUnit1Color = GfxColor(50, 50, 50, 255),
				unsigned int iUnit2Color = GfxColor(128, 128, 128, 255),
				unsigned int iOriginColor = GfxColor(200,0,0,255),
				TGfxVec2 tPos = TGfxVec2(0, 0));

void DrawArrow(TGfxSprite * pLineSprite, const TGfxVec2 & tStart, const TGfxVec2 & tEnd, float fSize,  unsigned int iColor = EGfxColor_White);


void DrawShatteredCircle(TGfxSprite * pLineSprite, const  TGfxVec2 & tPos, float fAngle, float fRadius, unsigned int iColor);
#endif