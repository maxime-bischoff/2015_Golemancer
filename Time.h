#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED
class TTime
{
public:
	TTime(void);
	~TTime(void);

	static float m_fTimeScale;

	static void  SetTimeScale( float fScale );
	static float GetTimeScale(){ return m_fTimeScale; };
};
float DeltaTime();
float UnscaleDeltaTime();
#endif