#pragma once
#ifndef __ANIMATED_INSTANCE_MODEL_H__
#define __ANIMATED_INSTANCE_MODEL_H__

#include "base.h"

#include <d3d9.h>

// Forward declarations -------------
class CRenderManager;
class CalModel;
class CAnimatedCoreModel;
class CEffectMaterial;
class CEffectTechnique;
// ----------------------------------


/**
 * Classe AnimatedInstanceModel.
 * Classe que representa una instància a un AnimatedCoreModel. Prenent aquest AnimatedCoreModel serà capaç d'animar-lo i renderitzar-lo.
**/
class CAnimatedInstanceModel:
  public CBaseControl
{
public:
  /**
   * Constructor per defecte.
  **/
  CAnimatedInstanceModel(void): m_pCalModel(0),m_pAnimatedCoreModel(0),
                                m_iCurrentCycle(0),m_iCurrentAnimationId(0),
                                m_fCurrentAnimationTime(0),m_pVB(0),m_pIB(0),
                                m_iNumVtxs(0),m_iNumFaces(0)//,m_pEffectTechnique(0)
                                {};
  /**
   * Destructor.
   * Allibera els recursos abans de destruir-se.
  **/
  ~CAnimatedInstanceModel(void) {Done();};

  /**
   * TODO: Mètode de pintat per pantalla.
   * Aquest mètode s'encarregarà de fer un pintat del model especificat via hardware, sense càlculs adicionals.
   * @param _pRM RenderManager.
  **/
  void Render                 (CRenderManager *_pRM);
  /**
   * Mètode de pintat per pantalla via software.
   * Aquest mètode s'encarregarà de fer un pintat del model especificat via software, efectuant els càlculs a RAM.
   * @param _pRM RenderManager.
  **/
  void RenderModelBySoftware  (CRenderManager *_pRM);
  /**
   * Mètode d'actualització del model.
   * Aquest mètode s'encarregarà d'actualitzar l'animació activa segons el temps que hagi transcorregut.
   * @param _fElapsedTime Temps que ha transcorregut desde l'última execució.
  **/
  void Update                 (float _fElapsedTime);
  /**
   * Mètode d'inicialització de la instancia.
   * Aquest mètode inicialitzarà la instancia segons l'AnimatedCoreModel proporcionat, si aquest és correcte la instancia quedarà Ok.
   * @param _pAnimatedCoreModel AnimatedCoreModel a instanciar.
  **/
  void Initialize             (CAnimatedCoreModel *_pAnimatedCoreModel);
  /**
   * Mètode de preparació de la instancia per a D3D.
   * Aquest mètode inicialitzarà l'IndexBuffer, el VertexBuffer i les Textures per al seu us a D3D.
   * @param _pRM RenderManager.
  **/
  void InitD3D                (CRenderManager *_pRM);
  /**
   * Mètode d'alliberació de recursos.
  **/
  void Release                ();
  /**
   * Mètode per executar una animació del model.
   * Aquest mètode inicialitzarà l'execució d'una animació del model instanciat.
   * @param _iId Identificador de l'animació.
   * @param _fTime Duració de l'animació.
  **/
  void ExecuteAction          (int _iId, float _fTime);
  /**
   * Mètode per ajuntar animacions.
   * Aquest mètode inicialitzarà l'execució d'una animació del model instanciat, juntant-la amb altres animacions que tingués actives.
   * @param _iId Identificador de l'animació.
   * @param _fTime Duració de l'animació.
  **/
  void BlendCycle             (int _iId, float _fTime);
  void BlendCycle             (const std::string &AnimationName, float _fTime);
  /**
   * Mètode per acabar les animacions actives.
   * Aquest mètode acabarà les animacions actives que tingués el model, fent que a final de cicle no es repeteixin més.
   * @param _fTime Temps fins a acabar-se.
  **/
  void ClearCycle             (float _fTime);
  /**
   * Getter del cicle d'animació actual.
   * @return Cicle d'animació actual.
  **/
  int GetCurrentCycle         () const              { return m_iCurrentCycle; };
  /**
   * Getter de l'animació actual.
   * @return L'animació actual.
  **/
  int GetCurrentAction        () const              { return m_iCurrentAnimationId; };

  int GetAnimationCount       ();

private:
  /**
   * Mètode per carregar el VertexBuffer i IndexBuffer.
   * Aquest mètode carrega el VertexBuffer i l'IndexBuffer a GRAM per al seu us amb D3D.
   * @param _pRM RenderManager.
  **/
  bool LoadVertexBuffer(CRenderManager *_pRM);
  /**
   * Mètode per carregar textures.
   * Aquest mètode carrega les textures a GRAM per al seu us amb D3D.
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
   * Cicle actual d'animació.
  **/
  int                       m_iCurrentCycle;
  /**
   * Animació actual.
  **/
  int                       m_iCurrentAnimationId;
  /**
   * Temps transcorregut des de l'inici de l'animació actual.
  **/
  float                     m_fCurrentAnimationTime;
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
};

#endif
