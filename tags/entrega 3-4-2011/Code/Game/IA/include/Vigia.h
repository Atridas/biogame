#pragma once

#ifndef __VIGIA_H__
#define __VIGIA_H__

#include "EnemyInstance.h"


class CVigia: public CEnemyInstance
{
public:

  CVigia(const string& _name) : CEnemyInstance(_name),
                                  m_fAlarmActivate(false),
                                  m_fEscudoActivate(false)
                                  {m_Type = CEnemy::MINER;};

  


  ~CVigia(void){};

  virtual void InitTemplate(CXMLTreeNode& _XMLParams);
  virtual void InitInstance(CXMLTreeNode& _XMLParams);

  //virtual void Render(CRenderManager* _pRM) const;
  
  // els set s'hauran de calcular
  void SetAlarmActivate(bool _fAlarmActivate){m_fAlarmActivate = _fAlarmActivate;};
  void SetEscudoActivate(bool _fEscudoActivate){m_fEscudoActivate = _fEscudoActivate;};

  bool GetAlarActivate() const {return m_fAlarmActivate;};
  bool GetEscudoActivate() const {return m_fEscudoActivate;};

protected:
  void Release() {};

  
  bool m_fAlarmActivate;
  bool m_fEscudoActivate;
 
};
#endif