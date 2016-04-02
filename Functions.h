#pragma once 




float CorrectRange(float fAngle);
float GetCorrectAngle(float fCurrentAngle, float fNewAngle, float fMinRotationSpeed);
float GetAngleDiff(float fAngle1, float fAngle2);
float RotationLerp(float fCurrentAngle, float fNewAngle, float fSpeed);
float MMin(float fValue1, float fValue2);
float MMax(float fValue1, float fValue2);
bool IsBetween(float fLimit1, float fLimit2, float fValue);
bool IsInFov(const TGfxVec2 & tPos, float fov, float fAngle, float fDistMax, const TGfxVec2 & tTargetPos);
float MClamp(float fValue, float fLimite1, float fLimite2);
float MLerp( float fFrom, float fTo, float fWeight );
TGfxVec2 MLerp(const TGfxVec2 & tFrom, const TGfxVec2 & tTo, float fWeight);
TGfxVec3 MLerp(const TGfxVec3 & tFrom, const TGfxVec3 & tTo, float fWeight);
TGfxVec2 MLerpWithTime(const TGfxVec2 & tFrom, const TGfxVec2 & tTo, float fWeight);
float Attenuate(float fFull, float fNull, float fValue, float fFallOff);


unsigned int ColorVec3ToInt(const struct TGfxVec3 &tColor);
