#include "include\State.h"


CState::CState(void)
{
}


CState::~CState(void)
{
}


void CState::SelectState(TStateType _TSelectState)
{
	m_TSelectState = _TSelectState;
	switch ( m_TSelectState )
	{

  case PATROLER : 
    
    break;

  case ATTACK : 
    
    break;

	case ALARM_ACTIVATE : 
    
    break;

	case ESCUDO_ACTIVATE : 
    
    break;

	case SEEK : 
    
    break;

  default : 
		m_TSelectState = _TSelectState;
  }

	
}