#include "Const.h"


float TTime::m_fTimeScale(1.0f);

TTime::TTime(void)
{
}

void TTime::SetTimeScale(float fScale)
{

	if(fScale<0) fScale = 0;
	m_fTimeScale = fScale;

}

TTime::~TTime(void)
{
}

float DeltaTime(){

	return GfxTimeFrameGetCurrentDurationInSeconds() * TTime::GetTimeScale();


}

float UnscaleDeltaTime(){

	return GfxTimeFrameGetCurrentDurationInSeconds() ;


}