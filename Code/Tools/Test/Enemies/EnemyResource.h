#pragma once

#include <string>

class CEnemyResource
{
private:
  void                  Relase();
  
	bool                  m_bIsOk;
public:
  CEnemyResource(void);
  ~CEnemyResource(void) {Done();};

  bool                  Init(const char* _pcId, const char* _pcPathMalla);
  void                  Done() {if(IsOk()) Relase(); m_bIsOk=false;};
  bool                  IsOk() const {return m_bIsOk;};



private:

  std::string           m_szPathMalla;
  std::string           m_szId;
};

