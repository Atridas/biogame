#pragma once
#ifndef __RENDERABLE_ANIMATED_INSTANCE_MODEL_H__
#define __RENDERABLE_ANIMATED_INSTANCE_MODEL_H__

#include "base.h"
#include "AnimatedInstanceModel.h"
#include "AnimatedCoreModel.h"
#include "RenderableObject.h"
#include <XML/XMLTreeNode.h>

class CEffect;

class CRenderableAnimatedInstanceModel: public CRenderableObject
{
public:
  CRenderableAnimatedInstanceModel(const string& _szName) : CRenderableObject(_szName) {};
  virtual ~CRenderableAnimatedInstanceModel() {Done();};

  bool          Init      (const string& _szCoreName);
  virtual void  InitFromXML(CXMLTreeNode& l_XMLObject);

  void          Update(float _fElapsedTime);

  CAnimatedInstanceModel* GetAnimatedInstanceModel(){return m_pAnimatedInstanceModel;}

  virtual const vector<CMaterial*>& GetMaterials() const {return m_pAnimatedInstanceModel->GetAnimatedCoreModel()->GetMaterials();};

  //TODO
  virtual bool ChangeInstance(const string& _szName) {return true;};
    
  virtual bool IsAlphaBlended() const {return false;};
protected:
  virtual void RenderRenderableObject(CRenderManager* _pRM, const vector<CEffect*>& _vEffects);

  virtual void Release();

private:
  CAnimatedInstanceModel * m_pAnimatedInstanceModel;
};

#endif