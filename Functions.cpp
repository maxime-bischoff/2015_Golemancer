#include "flib.h"
#include "flib_vec2.h"
#include "flib_vec3.h"
#include "Const.h"


float CorrectRange(float fAngle)
{
	
	if(fAngle > PI) return 2.0f*-PI + fAngle;
	else if(fAngle< - PI) return  2.0f* PI +fAngle;
	else return fAngle;


}

float GetCorrectAngle(float fCurrentAngle, float fNewAngle, float fMinRotationSpeed)
{
	float fAngle;
	if ( CorrectRange(fCurrentAngle -fNewAngle  ) > 0)
				{
					if(CorrectRange(fCurrentAngle -fNewAngle  )> fMinRotationSpeed )
					{
						fAngle =  CorrectRange(fCurrentAngle - fMinRotationSpeed );			
				
					}else {
						fAngle = CorrectRange(fNewAngle);
					}

				}
				 else{
					 if( fabs(CorrectRange(fCurrentAngle -fNewAngle  ))> fMinRotationSpeed ){
						
						fAngle = CorrectRange(fCurrentAngle + fMinRotationSpeed );
					}else {
						fAngle = CorrectRange(fNewAngle);
					 }
				}
				 return fAngle;

}

float GetAngleDiff(float fAngle1, float fAngle2)
{
	return CorrectRange(CorrectRange(fAngle1) - CorrectRange(fAngle2) );

}

float RotationLerp(float fCurrentAngle, float fNewAngle, float fSpeed)
{
	if (fSpeed == 0)return fCurrentAngle;
		
	fCurrentAngle = CorrectRange(fCurrentAngle);
	fNewAngle = CorrectRange(fNewAngle);

	float difference = abs(fCurrentAngle - fNewAngle);
	if (difference > PI)
	{
		if (fCurrentAngle > fNewAngle)
		{
			fNewAngle += 2 * PI;
		}
		else
		{
			fCurrentAngle += 2 * PI;
		}
	}

	return (MLerp(fCurrentAngle, fNewAngle, fSpeed));
}


float MMin(float fValue1,float fValue2)
{
	if(fValue1 > fValue2) return fValue2; else return fValue1;
	

}


float MMax(float fValue1,float fValue2)
{
	if(fValue1 < fValue2) return fValue2; else return fValue1;


}

float MLerp( float fFrom, float fTo, float fWeight )
{
	return fFrom + ( fTo - fFrom ) * fWeight;
}

TGfxVec2 MLerp(const TGfxVec2 & tFrom, const TGfxVec2 & tTo, float fWeight)
{	
	return TGfxVec2(tFrom.x + (tTo.x - tFrom.x) * fWeight, tFrom.y + (tTo.y - tFrom.y) * fWeight);
}

TGfxVec3 MLerp(const TGfxVec3 & tFrom, const TGfxVec3 & tTo, float fWeight)
{
	return TGfxVec3(MLerp(tFrom.x, tTo.x, fWeight), MLerp(tFrom.y, tTo.y, fWeight), MLerp(tFrom.z, tTo.z, fWeight));
}

// :: Other Lerp :: // 
TGfxVec2 MLerpWithTime(const TGfxVec2 & tFrom, const TGfxVec2 & tTo, float fTimer)
{
	return tTo * fTimer + (1 - fTimer) * tFrom;
}

bool IsBetween(float fLimit1, float fLimit2, float fValue)
{	
	if (fValue >= MMin(fLimit1, fLimit2) && fValue <= MMax(fLimit1, fLimit2))
		return true;
	else
		return false;

}

bool IsInFov(const TGfxVec2 & tPos, float fov, float fAngle, float fDistMax, const TGfxVec2 & tTargetPos)
{
	if ((tTargetPos - tPos).SquaredLength() > fDistMax * fDistMax)
	{
		return false;
	}
	else
	{
		TGfxVec2 tDir =  tTargetPos - tPos;
		float fAngleDir = CorrectRange(atan2(tDir.y, tDir.x));

		if ((CorrectRange(fAngleDir - fAngle) < fov / 2.0f) &&
			(CorrectRange(fAngleDir - fAngle) > -(fov / 2.0f)))


			return true;
		else return false;
	}
}


float MClamp(float fValue, float fLimite1, float fLimite2)
{
	if(fValue < fLimite1)
	{
		return fLimite1;
	}
	else {
		if(fValue>fLimite2)
		{
			return fLimite2;
		} else return fValue;
	}
}


unsigned int ColorVec3ToInt(const TGfxVec3 &tColor)
{
	/*if(tColor.x> 1) tColor.x = 1;
	if(tColor.y> 1) tColor.y = 1;
	if(tColor.z> 1) tColor.z = 1;*/


	return (GfxColor(tColor.x * 255 , tColor.y * 255, tColor.z * 255, 255));
}

TGfxVec3 ColorIntToVec3(unsigned int iColor)
{
	unsigned char * pColor = reinterpret_cast<unsigned char *>(&iColor);
	TGfxVec3 tColor;
	tColor.x = pColor[ 0 ] / 255.0f;
	tColor.y = pColor[ 1 ] / 255.0f;
	tColor.z = pColor[ 2 ] / 255.0f;
	return tColor;

}


float GetDistance(TGfxVec2 tPos1, TGfxVec2 tPos2)
{			
	return (tPos1 - tPos2).Length();
}



float Attenuate(float fFull, float fNull, float fValue, float fFallOff) //Attenuate the value, regarding to the falloff (---x---)f----v----n
																		//														 1----v----0
{
	float fStrength = 1- (fFull - fValue) / (fFull - fNull);

	return ( fStrength * fFallOff);
}



/*
float CorrectRange(float fAngle)
{
	if(fAngle> M_PI) return 2*-M_PI + fAngle;
	else if(fAngle< - M_PI) return  2* M_PI +fAngle;
	else return fAngle;


}*/