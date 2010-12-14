#pragma once
#ifndef __STATIC_MESH_H__
#define __STATIC_MESH_H__

#include <base.h>

//forward declarations---------------
class CRenderableVertexs;
class CTexture;
class CRenderManager;
//---------------------------------

class CStaticMesh: public CBaseControl
{
protected:
  vector<CRenderableVertexs*> m_RVs;
  vector<vector<CTexture *>> m_Textures;
  string m_szFileName;
  unsigned int m_iNumVertexs, m_iNumFaces;
  void Unload() {Release();};
public:
  CStaticMesh(): m_iNumVertexs(0),m_iNumFaces(0),m_szFileName(""){};
  ~CStaticMesh() {Done();};
  virtual void Release();
  bool Load (const string &_szFileName);
  bool LoadSergi(const string &_szFileName);
  bool ReLoad () {Unload(); return LoadSergi(m_szFileName);};
  void Render (CRenderManager *_pRM) const;
private:
  int GetVertexSize(unsigned short _uiVertexType);
};
//-----------------Format dels fitxers .mesh----------------
//  [] Designa opcional.
//
//  VERTEX_TYPE fa servir el mateix format que a VertexsStructs.h
//
//  VERTEX_STRUCT segueix el format de VertexsStructs.h. depenent del VERTEX_TYPE.
//
//  Per facilitar la lectura s'han afegit for, comes, espais i intros.
//    Al fitxer, però, simplement hi apareix el contingut sense separacions.
//-----------------Format fitxer Sergi----------------------
//  unsigned short                                            -> HEADER(MASK = 0xAAAA)
//  unsigned short                                            -> MaterialCount. nombre de materials
//
//  for MaterialCount
//  	unsigned short                                          -> VERTEX_TYPE.
//    unsigned short                                          -> nombre de propietats a llegir
//    [unsigned short, string]                                -> Diffuse. u16: Longitud de la cadena u16. string: path.
//    [unsigned short, string]                                -> Bumpmap. u16: Longitud de la cadena u16. string: path.
//    [unsigned short, string]                                -> Lightmap. u16: Longitud de la cadena u16. string: path.
//    [unsigned short, string]                                -> Environment. u16: Longitud de la cadena u16. string: path.
//
//  for MaterialCount
//  	unsigned short                                          -> VertexCount. Nombre de vèrtexs.
//    for VertexCount
//      VERTEX_STRUCT                                         -> Vèrtexs. Llista de vèrtexs. Format depenent de VERTEX_TYPE.
//
//  	unsigned short                                          -> IndexCount. Nombre d'índexs.
//  	for IndexCount
//  		unsigned short                                        -> Índexs. Llista d'índexs de vèrtexs.
//  
//  unsigned short                                            -> FOOTER(MASK = 0xFFFF)
//------------------Fi Format fitxer Sergi------------------
//
//------------------Format fitxer Isaac---------------------
//  unsigned short                                            -> HEADER(MASK = 0xAAAA)
//  unsigned short                                            -> MaterialCount. nombre de materials
//
//  for MaterialCount
//  	unsigned short                                          -> VERTEX_TYPE.
//    unsigned short                                          -> nombre de propietats a llegir
//    [unsigned short, string]                                -> Diffuse. u16: Longitud de la cadena u16. string: path.
//    [unsigned short, string]                                -> Bumpmap. u16: Longitud de la cadena u16. string: path.
//    [unsigned short, string]                                -> Lightmap. u16: Longitud de la cadena u16. string: path.
//    [unsigned short, string]                                -> Environment. u16: Longitud de la cadena u16. string: path.
//
//  for MaterialCount
//  	unsigned short                                          -> VertexCount. Nombre de vèrtexs.
//    for VertexCount
//      VERTEX_STRUCT                                         -> Vèrtexs. Llista de vèrtexs. Format depenent de VERTEX_TYPE.
//
//  	unsigned short                                          -> IndexCount. Nombre d'índexs.
//  	for IndexCount
//  		unsigned short                                        -> Índexs. Llista d'índexs de vèrtexs.
//  
//  unsigned short                                            -> FOOTER(MASK = 0xFFFF)
//------------------Fi Format fitxer Isaac------------------
#endif