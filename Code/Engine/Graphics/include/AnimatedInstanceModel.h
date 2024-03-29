#pragma once
#ifndef __ANIMATED_INSTANCE_MODEL_H__
#define __ANIMATED_INSTANCE_MODEL_H__

#include "base.h"
#include "AnimatedCoreModel.h"

#include <d3d9.h>

// Forward declarations -------------
class CRenderManager;
class CalModel;
class CEffect;
// ----------------------------------


/**
 * Classe AnimatedInstanceModel.
 * Classe que representa una inst�ncia a un AnimatedCoreModel. Prenent aquest AnimatedCoreModel ser� capa� d'animar-lo i renderitzar-lo.
**/
class CAnimatedInstanceModel:
  public CBaseControl
{
public:
  /**
   * Constructor per defecte.
  **/
  CAnimatedInstanceModel(void): m_pCalModel(0),m_pAnimatedCoreModel(0),
                                m_pVB(0),m_pIB(0),
                                m_iNumVtxs(0),m_iNumFaces(0),
                                m_szAnimationState(""),
                                m_fAnimationParameter(0)
                                //,m_pEffectTechnique(0)
                                {};
  /**
   * Destructor.
   * Allibera els recursos abans de destruir-se.
  **/
  ~CAnimatedInstanceModel(void) {Done();};

  /**
   * TODO: M�tode de pintat per pantalla.
   * Aquest m�tode s'encarregar� de fer un pintat del model especificat via hardware, sense c�lculs adicionals.
   * @param _pRM RenderManager.
  **/
  void Render                 (CRenderManager *_pRM, const vector<CEffect*>& _vEffects);
  /**
   * M�tode de pintat per pantalla via software.
   * Aquest m�tode s'encarregar� de fer un pintat del model especificat via software, efectuant els c�lculs a RAM.
   * @param _pRM RenderManager.
  **/
  void RenderModelBySoftware  (CRenderManager *_pRM);
  /**
   * M�tode d'actualitzaci� del model.
   * Aquest m�tode s'encarregar� d'actualitzar l'animaci� activa segons el temps que hagi transcorregut.
   * @param _fElapsedTime Temps que ha transcorregut desde l'�ltima execuci�.
  **/
  void Update                 (float _fElapsedTime);
  /**
   * M�tode d'inicialitzaci� de la instancia.
   * Aquest m�tode inicialitzar� la instancia segons l'AnimatedCoreModel proporcionat, si aquest �s correcte la instancia quedar� Ok.
   * @param _pAnimatedCoreModel AnimatedCoreModel a instanciar.
  **/
  void Initialize             (CAnimatedCoreModel *_pAnimatedCoreModel);
  /**
   * M�tode de preparaci� de la instancia per a D3D.
   * Aquest m�tode inicialitzar� l'IndexBuffer, el VertexBuffer i les Textures per al seu us a D3D.
   * @param _pRM RenderManager.
  **/
  void InitD3D                (CRenderManager *_pRM);
  /**
   * M�tode d'alliberaci� de recursos.
  **/
  void Release                ();
  /**
   * M�tode per executar una animaci� del model.
   * Aquest m�tode inicialitzar� l'execuci� d'una animaci� del model instanciat.
   * @param _iId Identificador de l'animaci�.
   * @param _fTime Duraci� de l'animaci�.
  **/
  void RemoveAction          (int _iId);
  void ExecuteAction         (int _iId, float _fTime, float _fWeight = 1.0f, bool _bAutoLock = false);
  /**
   * M�tode per ajuntar animacions.
   * Aquest m�tode inicialitzar� l'execuci� d'una animaci� del model instanciat, juntant-la amb altres animacions que tingu�s actives.
   * @param _iId Identificador de l'animaci�.
   * @param _fTime Duraci� de l'animaci�.
  **/
  void BlendCycle             (int _iId, float _fTime, float _fWeight = 1.0f);
  void BlendCycle             (const std::string &AnimationName, float _fTime, float _fWeight = 1.0f);
  /**
   * M�tode per acabar les animacions actives.
   * Aquest m�tode acabar� les animacions actives que tingu�s el model, fent que a final de cicle no es repeteixin m�s.
   * @param _fTime Temps fins a acabar-se.
  **/
  void ClearCycle             (int _iId, float _fTime);
  void ClearCycle             (const std::string &AnimationName, float _fTime);

  int GetAnimationCount       ();
  CAnimatedCoreModel*       GetAnimatedCoreModel() {return m_pAnimatedCoreModel;};
  CalModel*       GetAnimatedCalModel() {return m_pCalModel;};


  void SetAnimationState(const string& _szAnimationState);
  void SetAnimationParameter(float _fAnimationParameter);


  int GetAnimationId          (const std::string &AnimationName) const;

private:
  /**
   * M�tode per carregar el VertexBuffer i IndexBuffer.
   * Aquest m�tode carrega el VertexBuffer i l'IndexBuffer a GRAM per al seu us amb D3D.
   * @param _pRM RenderManager.
  **/
  bool LoadVertexBuffer(CRenderManager *_pRM);
  /**
   * M�tode per carregar textures.
   * Aquest m�tode carrega les textures a GRAM per al seu us amb D3D.
   * @param _pRM RenderManager.
  **/
  //void LoadTextures(CRenderManager *_pRM);

  /**
   * CalModel del model a instanciar.
  **/
  CalModel*                 m_pCalModel;
  /**
   * AnimatedCoreModel a instanciar.
  **/
  CAnimatedCoreModel*       m_pAnimatedCoreModel;
  /**
   * AnimatedCoreModel a instanciar.
  **/
  //CEffectTechnique*         m_pEffectTechnique;
  /**
   * Llista de materials del model.
  **/
  //vector<CEffectMaterial*>        m_vTextureList;
  /**
   * VertexBuffer.
  **/
  LPDIRECT3DVERTEXBUFFER9   m_pVB;
  /**
   * IndexBuffer.
  **/
  LPDIRECT3DINDEXBUFFER9    m_pIB;
  /**
   * Nombre de Vertexs del model.
  **/
  int                       m_iNumVtxs;
  /**
   * Nombre de Cares del model.
  **/
  int                       m_iNumFaces;


  //Coses d'animation states
  typedef CAnimatedCoreModel::SAnimation       SAnimation;
  typedef CAnimatedCoreModel::SAction          SAction;
  typedef CAnimatedCoreModel::SCycle           SCycle;
  typedef CAnimatedCoreModel::SAnimationState  SAnimationState;
  typedef CAnimatedCoreModel::SAnimationChange SAnimationChange;
  
  
  float ExecuteChange(const SAnimationChange& _AnimationChange);//retorna el fade
  void  ExecuteAction(const SAction& _Action);
  void  BlendCycle(const SCycle& _Cycle, float _fBlendTime);
  void  ClearCycle(const SCycle& _Cycle, float _fBlendTime);

  string m_szAnimationState;
  float  m_fAnimationParameter;
};

#endif
