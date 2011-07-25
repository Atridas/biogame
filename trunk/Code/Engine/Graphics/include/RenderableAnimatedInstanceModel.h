#pragma once
#ifndef __RENDERABLE_ANIMATED_INSTANCE_MODEL_H__
#define __RENDERABLE_ANIMATED_INSTANCE_MODEL_H__

#include "base.h"
#include "AnimatedInstanceModel.h"
#include "RenderableObject.h"
#include <XML/XMLTreeNode.h>

class CRenderableAnimatedInstanceModel: public CRenderableObject
{
  public:
    CRenderableAnimatedInstanceModel(const string& _szName);
    virtual ~CRenderableAnimatedInstanceModel() {Done();};

    bool          Init      (const string& _szCoreName);
    virtual void  InitFromXML(CXMLTreeNode& l_XMLObject);

    //Methods
    virtual void  RenderRenderableObject(CRenderManager* _pRM);

    CAnimatedInstanceModel* GetAnimatedInstanceModel(){return m_pAnimatedInstanceModel;}

    //TODO
    virtual bool ChangeInstance(const string& _szName) {return true;};
    
    virtual bool IsAlphaBlended() const {return false;};
  protected:
    virtual void          Release                   ();
    void                  Update                    (float _fElapsedTime);

  private:
    CAnimatedInstanceModel * m_pAnimatedInstanceModel;
};

#endif