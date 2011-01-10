#pragma once
#ifndef __STATIC_MESH_H__
#define __STATIC_MESH_H__

#include <base.h>

//forward declarations---------------
class CRenderableVertexs;
class CTexture;
class CRenderManager;
//---------------------------------

/**
 * Classe de Malla estàtica.
 * Classe que conté i controla una malla estàtica carregada des d'un fitxer mesh.
**/
class CStaticMesh:
  public CBaseControl
{
protected:
  /**
   * Vector de Vèrtexs pels diferents conjunts de vèrtexs que formen la malla.
  **/
  vector<CRenderableVertexs*> m_RVs;
  /**
   * Vector de Textures que conté la malla.
  **/
  vector<vector<CTexture *>> m_Textures;
  /**
   * Path relatiu al fitxer mesh.
  **/
  string m_szFileName;
  /**
   * Nombre de vèrtexs i triangles de la malla.
  **/
  unsigned int m_iNumVertexs, m_iNumFaces;
  /**
   * Mètode d'alliberament de recursos.
  **/
  void Unload() {Release();};
public:
  /**
   * Constructor per defecte.
  **/
  CStaticMesh(): m_iNumVertexs(0),m_iNumFaces(0),m_szFileName(""){};
  /**
   * Destructor.
   * Allibera els recursos abans de destruir-se.
  **/
  ~CStaticMesh() {Done();};
  /**
   * Mètode d'alliberament de recursos.
  **/
  virtual void Release();
  /**
   * Mètode de càrrega.
   * Aquest mètode carrega una malla d'un fitxer mesh i la instancia.
   * @param _szFileName Path relatiu al fitxer mesh.
   * @return True si s'ha carregat correctament, false sino.
  **/
  bool Load(const string &_szFileName);
  /**
   * Mètode de recàrrega.
   * Aquest mètode recarrega la malla del fitxer especificat anteriorment.
   * @return True si s'ha carregat correctament, false sino.
   * @see Load (const string &_szFileName)
  **/
  bool ReLoad() {Unload(); return Load(m_szFileName);};
  /**
   * Mètode de render.
   * Aquest mètode renderitza la malla.
   * @param _pRM Render Manager.
  **/
  void Render(CRenderManager *_pRM) const;
  //bool LoadSergi(const string &_szFileName);
};
#endif