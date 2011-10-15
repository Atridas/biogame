//----------------------------------------------------------------------------------
// CTimer class
// Author: Enric Vergara
//
// Description:
// Contiene toda la informacion relacionada con el tiempo, es decir:
//	- FPS: los frames por segundo a que va la aplicacion
//	- ElapsedTime: el tiempor que ha pasado dsd la ultima llamad a Update
//----------------------------------------------------------------------------------
#pragma once 
#ifndef INC_TIMERMANAGER_H_
#define INC_TIMERMANAGER_H_

class CTimer
{

public:

	CTimer(uint32 avgSamples, float _fMaxElapsed);
	~CTimer();

	void			Update					();
  void      Reset           ()       { m_fRelativeTime = 0.0f; m_uSamples = 0; m_uIndex = 0; }; //Resets the relative time

	float			GetElapsedTime	() const { return m_fElapsedTime*m_fBulletTimeFactor; };
  float			GetTotalTime  	() const { return m_fTotalTime; };
  float			GetRelativeTime	() const { return m_fRelativeTime; };
	float			GetFPS					() const { return m_fFPS; };
  void      SetBulletTimeFactor(float _fFactor);

private:

	float			m_fElapsedTime; // ElapsedTime
  float     m_fBulletTimeFactor;
  float     m_fTotalTime;   // Time since the app started
  float     m_fRelativeTime;// Time since last's reset
	float			m_fFPS;					// FPS
	float			m_fLastFps;
	float			m_fFPSTime;
	double		m_dLastTime;
	uint32		m_uFPSCount;
	float*		m_Deltas;				// array of instant delta times
	uint32		m_uSamples;			// deltas arrays length
	uint32		m_uMaxSamples;	// deltas arrays length
	uint32		m_uIndex;				// current array position
  float     m_fMaxElapsed;
};

#endif //INC_TIMERMANAGER_H_