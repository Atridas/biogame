#pragma once
#ifndef __STATIC_MESH_H__
#define __STATIC_MESH_H__

#include <base.h>
#include <XML/XMLTreeNode.h>

//forward declarations---------------
class CRenderableVertexs;
class CEffectMaterial;
class CRenderManager;
class CMaterial;
//---------------------------------

/**
 * Classe de Malla estàtica.
 * Classe que conté i controla una malla estàtica carregada des d'un fitxer mesh.
**/
class CStaticMesh:
  public CBaseControl
{
public:
  /**
   * Constructor per defecte.
  **/
  CStaticMesh(): m_iNumVertexs(0),m_iNumFaces(0),m_szFileName(""),m_vMax(Vect3f(0.0f)),m_vMin(Vect3f(0.0f)){};
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
  bool Load(const CXMLTreeNode& _XMLTreeNode);
  /**
   * Mètode de recàrrega.
   * Aquest mètode recarrega la malla del fitxer especificat anteriorment.
   * @return True si s'ha carregat correctament, false sino.
   * @see Load (const string &_szFileName)
  **/
  bool Reload() {Done(); SetOk(Load()); return IsOk();};
  /**
   * Mètode de render.
   * Aquest mètode renderitza la malla.
   * @param _pRM Render Manager.
  **/
  void Render(CRenderManager *_pRM, bool _bInstanced = false) const;
  /**
   * Retorn de les coordenades mínimes de la malla.
   * @return Les coordenades mínimes de la malla.
  **/
  Vect3f GetMin() const { return m_vMin; };
  /**
   * Retorn de les coordenades máximes de la malla.
   * @return Les coordenades máximes de la malla.
  **/
  Vect3f GetMax() const { return m_vMax; };

  vector<uint32> GetIndexBuffer ()  {return m_vIndexBuffer;};
  vector<Vect3f> GetVertexBuffer ()  {return m_vVertexBuffer;};
  
  bool IsAlphaBlended() const;

protected:

  bool Load();
  /**
   * Mètode d'alliberament de recursos.
  **/
  void Unload() {Release();};
  /**
   * Vector de Vèrtexs pels diferents conjunts de vèrtexs que formen la malla.
  **/
  vector<CRenderableVertexs*> m_RVs;
  /**
   * Vector de Textures que conté la malla.
  **/
  //vector<CEffectMaterial*> m_vMaterials;

  vector<CMaterial*> m_vMaterials;
  /**
   * Path relatiu al fitxer mesh.
  **/
  string m_szFileName;
  /**
   * Nombre de vèrtexs i triangles de la malla.
  **/
  unsigned int m_iNumVertexs, m_iNumFaces;
  /**
   * Valors max i min de cada coordenada.
  **/
  Vect3f m_vMax, m_vMin;

  vector<uint32> m_vIndexBuffer;
  vector<Vect3f> m_vVertexBuffer;
};
#endif