#pragma once

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "StateMachine.h"

//desideix a kin estat pasa segons els succesos/accions i el enemic, i li pasa al state machine.
// Es el tipus enemic (ej: Militar), que fa un m_pStateMachine -> ChangeState(new CStatePatrol());
class CState
{
public:
  CState(void);
  ~CState(void);

	enum TStateType
  {
	  PATROLER=0, // vigilancia
	  ATTACK,
    ALARM_ACTIVATE,
		ESCUDO_ACTIVATE,
		SEEK // persegir
  };

	void SelectState(TStateType _TSelectState);

protected:
	TStateType		m_TSelectState;
};
#endif
