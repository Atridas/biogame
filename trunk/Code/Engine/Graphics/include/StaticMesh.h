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
 * Classe de Malla est�tica.
 * Classe que cont� i controla una malla est�tica carregada des d'un fitxer mesh.
**/
class CStaticMesh:
  public CBaseControl
{
protected:
  /**
   * Vector de V�rtexs pels diferents conjunts de v�rtexs que formen la malla.
  **/
  vector<CRenderableVertexs*> m_RVs;
  /**
   * Vector de Textures que cont� la malla.
  **/
  vector<vector<CTexture *>> m_Textures;
  /**
   * Path relatiu al fitxer mesh.
  **/
  string m_szFileName;
  /**
   * Nombre de v�rtexs i triangles de la malla.
  **/
  unsigned int m_iNumVertexs, m_iNumFaces;
  /**
   * M�tode d'alliberament de recursos.
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
   * M�tode d'alliberament de recursos.
  **/
  virtual void Release();
  /**
   * M�tode de c�rrega.
   * Aquest m�tode carrega una malla d'un fitxer mesh i la instancia.
   * @param _szFileName Path relatiu al fitxer mesh.
   * @return True si s'ha carregat correctament, false sino.
  **/
  bool Load(const string &_szFileName);
  /**
   * M�tode de rec�rrega.
   * Aquest m�tode recarrega la malla del fitxer especificat anteriorment.
   * @return True si s'ha carregat correctament, false sino.
   * @see Load (const string &_szFileName)
  **/
  bool ReLoad() {Unload(); return Load(m_szFileName);};
  /**
   * M�tode de render.
   * Aquest m�tode renderitza la malla.
   * @param _pRM Render Manager.
  **/
  void Render(CRenderManager *_pRM) const;
  //bool LoadSergi(const string &_szFileName);
};
#endif