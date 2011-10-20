#pragma once
#ifndef __ANIMATED_CORE_MODEL_H__
#define __ANIMATED_CORE_MODEL_H__

#include "base.h"
#include "Named.h"
#include "Utils/BoundingBox.h"
#include "Utils/BoundingSphere.h"

#define MAXBONES 29

// Forward declarations -------------
class CalModel;
class CalCoreModel;
class CalHardwareModel;
class CRenderableVertexs;
class CMaterial;
class CXMLTreeNode;
class CAnimatedInstanceModel;
// ----------------------------------


/**
 * Classe AnimatedCoreModel.
 * Classe que fa d'interf�cie per a models animats, en aquest cas s'ha adaptat per representar un model Cal3D.
**/
class CAnimatedCoreModel:
  public CBaseControl,
  public CNamed
{
public:

  struct SAnimation {
    int iId;
    float fWeight;
    bool bFromParameter, bFromComplementaryParameter;
    float fFadeOnChange;

    SAnimation():iId(0),fWeight(0),bFromParameter(false),bFromComplementaryParameter(false),fFadeOnChange(0) {};
    bool operator <(const SAnimation& _other) const { return iId < _other.iId; };
  };

  struct SCycle: SAnimation {
  };

  struct SAction: SAnimation {
    bool bBlock, bStop;
    float fFadeIn, fFadeOut;

    SAction():bBlock(false),bStop(false),fFadeIn(.3f),fFadeOut(.3f){};
  };

  struct SAnimationState {
    float fDefaultFadeIn, fDefaultFadeOut;
    set<SCycle> Cycles;
    set<SAction> OnEnter;
    set<SAction> OnExit;

    SAnimationState():fDefaultFadeIn(0),fDefaultFadeOut(0){};
  };

  struct SAnimationChange {
    float fFade;
    set<SAction> Actions;

    SAnimationChange():fFade(0){}
  };

public:
  /**
   * Constructor per defecte.
  **/
  CAnimatedCoreModel(const string& _szName): 
                            CNamed(_szName),m_pCalCoreModel(0),
                            m_pRenderableVertexs(0),m_pCalHardwareModel(0),
                            m_szSkeletonFilename(""),
                            m_szPath(""),m_iNumFaces(0), m_iNumVtxs(0),
                            m_szDefaultAnimationState("")
                                            {};
  /**
   * Destructor.
   * Allibera els recursos abans de destruir-se.
  **/
  ~CAnimatedCoreModel(void)             {Done();};

  /**
   * Getter del model animat.
   * @return El CalCoreModel que s'est� representant.
  **/
  CalCoreModel *GetCoreModel            ( )                         { return m_pCalCoreModel; };
  /**
   * Getter de textura.
   * Aquest m�tode retorna l'identificador de la textura de l'�ndex sol�licitat.
   * @param _uiId �ndex de la textura sol�licitada.
   * @return Identificador de la textura.
  **/
  //const string & GetTextureName         ( size_t _uiId ) const      { return m_vTextureFilenameList[_uiId]; };
  /**
   * Getter del nombre de textures.
   * Aquest m�tode retorna el nombre de textures que cont� el model representat.
   * @return Nombre de textures del model.
  **/
  //size_t GetNumTextures                 ( ) const                   { return m_vTextureFilenameList.size(); };

  const vector<CMaterial*>& GetMaterials() const { return m_vMaterials;};

  /**
   * Getter del model cal3d.
   * Aquest m�tode retorna model de cal3d representat, preparat per la seva renderitzaci� via hardware.
   * @return El model cal3d hardware.
  **/
  CalHardwareModel* GetCalHardwareModel ( ) const                   { return m_pCalHardwareModel; };
  /**
   * Getter del vertex i �ndex buffer.
   * Aquest m�tode retorna la classe RenderableVertexs que representa el vertex i l'index buffer del model.
   * @return La classe RenderableVertexs que representa al model.
  **/
  CRenderableVertexs* GetRenderableVertexs ( ) const                { return m_pRenderableVertexs; };


  const CBoundingBox&    GetBoundingBox   () const { return m_BoundingBox; };
  const CBoundingSphere& GetBoundingSphere() const { return m_BoundingSphere; };


  /**
   * M�tode de c�rrega desde fitxer.
   * Aquest m�tode carrega el model cal3d que es troba al path proporcionat. Si la c�rrega s'ha efectuat correctament quedar� en ok.
   * @return True si tot ha anat b�, false sino.
  **/
  bool Load                             (const std::string &_szPath);
  /**
   * M�tode per carregar el VertexBuffer i IndexBuffer.
   * Aquest m�tode carrega el VertexBuffer i l'IndexBuffer a GRAM per al seu us amb D3D.
   * @param _pRM RenderManager.
  **/
  bool LoadVertexBuffer                ();
  /**
   * M�tode de rec�rrega desde fitxer.
   * Aquest m�tode recarrega el model cal3d anteriorment carregat. Si la c�rrega s'ha efectuat correctament quedar� en ok.
   * @return True si tot ha anat b�, false sino.
   * @see Load(const std::string &_szPath)
  **/
  bool Reload                           ()                          { ReloadAnimaionStates(); return ReloadTextures(); } //Release(); return Load(m_szPath);};
  /**
   * M�tode de rec�rrega desde fitxer.
   * Aquest m�tode recarrega el model cal3d del nou fitxer especificat. Si la c�rrega s'ha efectuat correctament quedar� en ok.
   * @return True si tot ha anat b�, false sino.
  **/
  bool Reload                           (const std::string &_szPath){ Release(); return Load( _szPath);};
  
  void ReloadAnimaionStates();

  int  GetAnimationCount                ();

protected:
  /**
   * M�tode d'alliberament de recursos.
  **/
  void Release                          ();

private:

  void ComputeBoundings(CXMLTreeNode& _xmlInit);
  /**
   * M�tode de c�rrega de la malla.
  **/
  bool LoadMesh                         ();
  /**
   * M�tode de c�rrega de l'esquelet.
  **/
  bool LoadSkeleton                     ();
  /**
   * M�tode de c�rrega d'animacions.
   * @param _szName Identificador de l'animaci�.
   * @param _szFileName Nom del fitxer on es troba l'animaci�.
  **/
  bool LoadAnimation                   (const string& _szName, const std::string& _szFilename);

  bool ReloadTextures();

  /**
   * Model Cal3D representat.
  **/
  CalCoreModel*             m_pCalCoreModel;
  /**
   * Model Cal3D hardware representat.
  **/
  CalHardwareModel*         m_pCalHardwareModel;
  /**
   * Malla i �ndexs del model.
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
   * Path del fitxer que cont� la malla.
  **/
  vector<string>            m_vMeshFilenames;
  /**
   * Path del fitxer que cont� l'esquelet.
  **/
  string                    m_szSkeletonFilename;
  /**
   * Vector de paths de les textures del model.
  **/
  vector<CMaterial*>        m_vMaterials;
  /**
   * Path del model.
  **/
  string                    m_szPath;
  
  CBoundingBox              m_BoundingBox;
  CBoundingSphere           m_BoundingSphere;

  string m_szDefaultAnimationState;
  map<string, SAnimationState>               m_AnimationStates;
  map<string, map<string, SAnimationChange>> m_AnimationChanges;

  friend class CAnimatedInstanceModel;

  void LoadAnimationStates(CXMLTreeNode& _xmlAnimationStates);
  void LoadAnimationChanges(CXMLTreeNode& _xmlAnimationChanges);
  bool LoadAnimationFromState(CXMLTreeNode &_xmlAnimation, CAnimatedCoreModel::SAnimation &Animation_);
  bool LoadActionFromState(CXMLTreeNode &_xmlAction, CAnimatedCoreModel::SAction &Action_);
  bool LoadAnimationState(CXMLTreeNode &_xmlAnimationState, CAnimatedCoreModel::SAnimationState &AnimationState_);
  bool LoadAnimationChange(CXMLTreeNode &_xmlAnimationChange, CAnimatedCoreModel::SAnimationChange &AnimationChange_);
};

#endif
