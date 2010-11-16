#pragma once

#ifndef __BONES_MANERES_H__
#define __BONES_MANERES_H__

class CBonesManeres
{
public:
  CBonesManeres():m_bIsOk(false){};
  
  void                  Done            ()        {if(IsOk()) {Relase(); m_bIsOk=false;}};
  bool                  IsOk            () const  {return m_bIsOk;};
protected:
  bool                  m_bIsOk;
  virtual void          Relase					()=0;
};

#endif