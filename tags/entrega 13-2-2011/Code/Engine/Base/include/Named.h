#pragma once
#ifndef __NAMED_H__
#define __NAMED_H__

#include "base.h"

/**
 * Classe d'identificaci�.
 * Aquesta classe proporciona d'una identificaci� i la gesti� d'aquesta a aquelles classes que en derivin.
**/
class CNamed
{
public:
  /**
   * Constructor per defecte.
   * Constructor de la classe. Permet especificar l'identificaci� que rebr� aquesta.
   * @param _szName Identificaci� que rebr� la classe.
  **/
  CNamed(const string& _szName):m_szName(_szName) {};
  /**
   * Destructor.
  **/
  virtual ~CNamed(void) {};

  /**
   * M�tode per obtenir l'identificaci�.
   * @return Identificaci� de la classe.
  **/
  const string&     GetName() const                 {return m_szName;};

protected:
  /**
   * M�tode per redefinir l'identificaci�.
  **/
  void              SetName(const string& _szName)  {m_szName = _szName;};

private:
  /**
   * Identificaci� de la classe.
  **/
  string m_szName;
};

#endif
