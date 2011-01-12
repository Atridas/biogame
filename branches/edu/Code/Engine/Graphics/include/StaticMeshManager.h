#pragma once
#ifndef __STATICMESH_MANAGER_H__
#define __STATICMESH_MANAGER_H__

#include <Utils/MapManager.h>
#include <set>
#include "StaticMesh.h"

/**
 * Classe Manager de Malles est�tiques.
 * Classe que s'encarrega de carregar i administrar les diferents Malles est�tiques de l'engine.
**/
class CStaticMeshManager:
  public CMapManager<CStaticMesh>
{
public:
  /**
   * Constructor per defecte.
   * Aquest constructor deixa la classe en Ok.
  **/
  CStaticMeshManager()  {SetOk(true);};
  /**
   * Destructor.
   * Allibera els recursos abans d'eliminar-se.
  **/
  ~CStaticMeshManager() {Done();};
  /**
   * M�tode de c�rrega.
   * Aquest m�tode pren un fitxer XML en el qual s'especifiquin les diferents malles a carregar i les carrega.
   * @params _szFileName Path relatiu al fitxer XML.
   * @return True si ha carregat correctament, false sino.
  **/
  bool Load(const string &_szFileName);
  /**
   * M�tode de c�rrega.
   * Aquest m�tode pren un vector de fitxers XML en els qual s'especifiquin les diferents malles a carregar i les carrega.
   * @params _XMLs Vector que cont� paths relatius als fitxers XML.
   * @return True si ha carregat correctament, false sino.
  **/
  bool Load(const vector<string>& _XMLs);
  /**
   * M�tode de rec�rrega.
   * Aquest m�tode recarrega tots els mesh segons els fitxers XML proporcionats anteriorment.
   * @return True si ha carregat correctament, false sino.
   * @see Load(const string &_szFileName)
   * @see Load(const vector<string>& _XMLs)
  **/
  bool Reload();

private:
  /**
   * M�tode de c�rrega.
   * Aquest m�tode pren un fitxer XML en el qual s'especifiquin les diferents malles a carregar i les carrega. Adicionalment controlar si s'informa o no de repeticions de malla.
   * @params _szFileName Path relatiu al fitxer XML.
   * @params _bReload Si val true no avisar� quan es trobi una malla repetida. En cas contrari avisar�.
   * @return True si ha carregat correctament, false sino.
  **/
  bool Load(const string &_szFileName, bool _bReload);

  /**
   * Conjunt de fitxers XML que contenen Paths a malles.
  **/
  set<string> m_vXMLFiles;
};

#endif