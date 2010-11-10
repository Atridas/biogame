#pragma once
class CRenderManager
{
  void                  Relase();
  
	bool                  m_bIsOk;
public:
	CRenderManager(void):m_bIsOk(false){};
  virtual ~CRenderManager(void) {Done();};

  bool                  Init();

  void                  Done            ()        {if(IsOk()) Relase(); m_bIsOk=false;};
  bool                  IsOk            () const  {return m_bIsOk;};

  void                  BeginRendering  ();
  void                  EndRendering    ();
  void                  SetUpMatrices   ();
};

