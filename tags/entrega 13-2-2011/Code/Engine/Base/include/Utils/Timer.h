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

	CTimer(uint32 avgSamples);
	~CTimer();

	void			Update					();
  void      Reset           ()       { m_fRelativeTime = 0.0f; }; //Resets the relative time

	float			GetElapsedTime	() const { return m_fElapsedTime; }
  float			GetTotalTime  	() const { return m_fTotalTime; }
  float			GetRelativeTime	() const { return m_fRelativeTime; }
	float			GetFPS					() const { return m_fFPS; }

private:

	float			m_fElapsedTime; // ElapsedTime
  float     m_fTotalTime;   // Time since the app started
  float     m_fRelativeTime;// Time since last's reset
	float			m_fFPS;					// FPS
	float			m_fLastFps;
	float			m_fFPSTime;
	double		m_dLastTime;
	uint32		m_uFPSCount;
	float*		m_Deltas;				// array of instant delta times
	uint32		m_uSamples;			// deltas arrays length
	uint32		m_uIndex;				// current array position
};

#endif //INC_TIMERMANAGER_H_