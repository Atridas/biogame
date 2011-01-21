#pragma once
#ifndef __ANIMATED_CORE_MODEL_H__
#define __ANIMATED_CORE_MODEL_H__

#include "base.h"
#include "Named.h"

#define MAXBONES 29

// Forward declarations -------------
class CalModel;
class CalCoreModel;
class CalHardwareModel;
class CRenderableVertexs;
// ----------------------------------


/**
 * Classe AnimatedCoreModel.
 * Classe que fa d'interfície per a models animats, en aquest cas s'ha adaptat per representar un model Cal3D.
**/
class CAnimatedCoreModel:
  public CBaseControl,
  public CNamed
{
public:
  /**
   * Constructor per defecte.
  **/
  CAnimatedCoreModel(const string& _szName): 
                            CNamed(_szName),m_pCalCoreModel(0),
                            m_pRenderableVertexs(0),m_pCalHardwareModel(0),
                            m_szMeshFilename(""),m_szSkeletonFilename(""),
                            m_szPath(""),m_iNumFaces(0), m_iNumVtxs(0)
                                            {};
  /**
   * Destructor.
   * Allibera els recursos abans de destruir-se.
  **/
  ~CAnimatedCoreModel(void)             {Done();};

  /**
   * Getter del model animat.
   * @return El CalCoreModel que s'està representant.
  **/
  CalCoreModel *GetCoreModel            ( )                         { return m_pCalCoreModel; };
  /**
   * Getter de textura.
   * Aquest mètode retorna l'identificador de la textura de l'índex sol·licitat.
   * @param _uiId Índex de la textura sol·licitada.
   * @return Identificador de la textura.
  **/
  const string & GetTextureName         ( size_t _uiId ) const      { return m_vTextureFilenameList[_uiId]; };
  /**
   * Getter del nombre de textures.
   * Aquest mètode retorna el nombre de textures que conté el model representat.
   * @return Nombre de textures del model.
  **/
  size_t GetNumTextures                 ( ) const                   { return m_vTextureFilenameList.size(); };
  /**
   * Getter del model cal3d.
   * Aquest mètode retorna model de cal3d representat, preparat per la seva renderització via hardware.
   * @return El model cal3d hardware.
  **/
  CalHardwareModel* GetCalHardwareModel ( ) const                   { return m_pCalHardwareModel; };
  /**
   * Getter del vertex i índex buffer.
   * Aquest mètode retorna la classe RenderableVertexs que representa el vertex i l'index buffer del model.
   * @return La classe RenderableVertexs que representa al model.
  **/
  CRenderableVertexs* GetRenderableVertexs ( ) const                { return m_pRenderableVertexs; };
  /**
   * Mètode de càrrega desde fitxer.
   * Aquest mètode carrega el model cal3d que es troba al path proporcionat. Si la càrrega s'ha efectuat correctament quedarà en ok.
   * @return True si tot ha anat bé, false sino.
  **/
  bool Load                             (const std::string &_szPath);
  /**
   * Mètode per carregar el VertexBuffer i IndexBuffer.
   * Aquest mètode carrega el VertexBuffer i l'IndexBuffer a GRAM per al seu us amb D3D.
   * @param _pRM RenderManager.
  **/
  bool LoadVertexBuffer                ();
  /**
   * Mètode de recàrrega desde fitxer.
   * Aquest mètode recarrega el model cal3d anteriorment carregat. Si la càrrega s'ha efectuat correctament quedarà en ok.
   * @return True si tot ha anat bé, false sino.
   * @see Load(const std::string &_szPath)
  **/
  bool Reload                           ()                          { Release(); return Load(m_szPath);};
  /**
   * Mètode de recàrrega desde fitxer.
   * Aquest mètode recarrega el model cal3d del nou fitxer especificat. Si la càrrega s'ha efectuat correctament quedarà en ok.
   * @return True si tot ha anat bé, false sino.
  **/
  bool Reload                           (const std::string &_szPath){ Release(); return Load( _szPath);};

protected:
  /**
   * Mètode d'alliberament de recursos.
  **/
  void Release                          ();

private:
  /**
   * Mètode de càrrega de la malla.
  **/
  bool LoadMesh                         ();
  /**
   * Mètode de càrrega de l'esquelet.
  **/
  bool LoadSkeleton                     ();
  /**
   * Mètode de càrrega d'animacions.
   * @param _szName Identificador de l'animació.
   * @param _szFileName Nom del fitxer on es troba l'animació.
  **/
  bool LoadAnimation                   (const string& _szName, const std::string& _szFilename);

  /**
   * Model Cal3D representat.
  **/
  CalCoreModel*             m_pCalCoreModel;
  /**
   * Model Cal3D hardware representat.
  **/
  CalHardwareModel*         m_pCalHardwareModel;
  /**
   * Malla i índexs del model.
  **/
  CRenderableVertexs*       m_pRenderableVertexs;
  /**
   * Nombre de Vertexs del model.
  **/
  int                       m_iNumVtxs;
  /**
   * Nombre de Cares del model.
  **/
  int                       m_iNumFaces;
  /**
   * Path del fitxer que conté la malla.
  **/
  string                    m_szMeshFilename;
  /**
   * Path del fitxer que conté l'esquelet.
  **/
  string                    m_szSkeletonFilename;
  /**
   * Vector de paths de les textures del model.
  **/
  vector<std::string>       m_vTextureFilenameList;
  /**
   * Path del model.
  **/
  string                    m_szPath;
};

#endif
