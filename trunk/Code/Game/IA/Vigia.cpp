#include "Vigia.h"


void CVigia::InitTemplate(CXMLTreeNode& _XMLParams)
{
  CEnemy::InitTemplate(_XMLParams);
  m_fAlarmActivate = (_XMLParams.GetBoolProperty("alarm_activate",false));
  m_fEscudoActivate = (_XMLParams.GetBoolProperty("escudo_activate",false));
  CEnemy::InitTemplate(_XMLParams);
	m_fAlarmActivate = (_XMLParams.GetBoolProperty("alarm_activate",false));
	m_fEscudoActivate = (_XMLParams.GetBoolProperty("escudo_activate",false));
}

void CVigia::InitInstance(CXMLTreeNode& _XMLParams)
{
  CEnemyInstance::InitInstance(_XMLParams);
}
