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
 * Classe de Malla est�tica.
 * Classe que cont� i controla una malla est�tica carregada des d'un fitxer mesh.
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
   * M�tode d'alliberament de recursos.
  **/
  virtual void Release();
  /**
   * M�tode de c�rrega.
   * Aquest m�tode carrega una malla d'un fitxer mesh i la instancia.
   * @param _szFileName Path relatiu al fitxer mesh.
   * @return True si s'ha carregat correctament, false sino.
  **/
  bool Load(const CXMLTreeNode& _XMLTreeNode);
  /**
   * M�tode de rec�rrega.
   * Aquest m�tode recarrega la malla del fitxer especificat anteriorment.
   * @return True si s'ha carregat correctament, false sino.
   * @see Load (const string &_szFileName)
  **/
  bool Reload() {Done(); SetOk(Load()); return IsOk();};
  /**
   * M�tode de render.
   * Aquest m�tode renderitza la malla.
   * @param _pRM Render Manager.
  **/
  void Render(CRenderManager *_pRM, bool _bInstanced = false) const;
  /**
   * Retorn de les coordenades m�nimes de la malla.
   * @return Les coordenades m�nimes de la malla.
  **/
  Vect3f GetMin() const { return m_vMin; };
  /**
   * Retorn de les coordenades m�ximes de la malla.
   * @return Les coordenades m�ximes de la malla.
  **/
  Vect3f GetMax() const { return m_vMax; };

  vector<uint32> GetIndexBuffer ()  {return m_vIndexBuffer;};
  vector<Vect3f> GetVertexBuffer ()  {return m_vVertexBuffer;};
  
  bool IsAlphaBlended() const;

protected:

  bool Load();
  /**
   * M�tode d'alliberament de recursos.
  **/
  void Unload() {Release();};
  /**
   * Vector de V�rtexs pels diferents conjunts de v�rtexs que formen la malla.
  **/
  vector<CRenderableVertexs*> m_RVs;
  /**
   * Vector de Textures que cont� la malla.
  **/
  //vector<CEffectMaterial*> m_vMaterials;

  vector<CMaterial*> m_vMaterials;
  /**
   * Path relatiu al fitxer mesh.
  **/
  string m_szFileName;
  /**
   * Nombre de v�rtexs i triangles de la malla.
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