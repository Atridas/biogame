#pragma once
#ifndef __NAMED_H__
#define __NAMED_H__

#include "base.h"

/**
 * Classe d'identificació.
 * Aquesta classe proporciona d'una identificació i la gestió d'aquesta a aquelles classes que en derivin.
**/
class CNamed
{
public:
  /**
   * Constructor per defecte.
   * Constructor de la classe. Permet especificar l'identificació que rebrà aquesta.
   * @param _szName Identificació que rebrà la classe.
  **/
  CNamed(const string& _szName):m_szName(_szName) {};
  /**
   * Destructor.
  **/
  virtual ~CNamed(void) {};

  /**
   * Mètode per obtenir l'identificació.
   * @return Identificació de la classe.
  **/
  const string&     GetName() const                 {return m_szName;};

protected:
  /**
   * Mètode per redefinir l'identificació.
  **/
  void              SetName(const string& _szName)  {m_szName = _szName;};

private:
  /**
   * Identificació de la classe.
  **/
  string m_szName;
};

#endif
