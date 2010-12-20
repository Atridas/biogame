#pragma once
#ifndef __NAMED_H__
#define __NAMED_H__

class CNamed
{
public:
  CNamed(const string& _szName):m_szName(_szName) {};
  virtual ~CNamed(void) {};

  const string&     GetName() const                 {return m_szName;};

protected:
  void              SetName(const string& _szName)  {m_szName = _szName;};

private:
  string m_szName;
};

#endif
