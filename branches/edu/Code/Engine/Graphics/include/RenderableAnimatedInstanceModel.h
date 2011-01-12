#pragma once
#ifndef __RENDERABLE_ANIMATED_INSTANCE_MODEL_H__
#define __RENDERABLE_ANIMATED_INSTANCE_MODEL_H__

#include "base.h"
#include "AnimatedInstanceModel.h"
#include "RenderableObject.h"

class CRenderableAnimatedInstanceModel: public CRenderableObject
{
  public:
    CRenderableAnimatedInstanceModel(const string& _szName);
    virtual ~CRenderableAnimatedInstanceModel() {Done();};

    bool          Init      (const string& _szCoreName);
    CAnimatedInstanceModel* GetAnimatedInstanceModel(){return m_pAnimatedInstanceModel;}

    //Methods
    virtual void  RenderRenderableObject(CRenderManager* _pRM);

  protected:
    virtual void          Release                   ();

  private:
    CAnimatedInstanceModel * m_pAnimatedInstanceModel;
};

#endif