#pragma once

#include <map>
#include <vector>

#include "EnemyResource.h"
#include "EnemyInstance.h"

class CEnemyManager
{
public:
  typedef std::map<std::string,CEnemyResource*>  tResources;
  typedef std::vector<CEnemyInstance*>           tInstances;
private:

  void                  Release();
  
	bool                  m_bIsOk;

public:
	CEnemyManager(void);
  virtual ~CEnemyManager(void) {Done();};

  bool                  Init(const char* _pcFile);
  void                  Done() {if(IsOk()) Release(); m_bIsOk=false;};
  bool                  IsOk() const {return m_bIsOk;};

private:
  tResources            m_Resources;
  tInstances            m_Instances;
};

