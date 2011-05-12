#pragma once
#include "Material.h"

class CMaterialDecorator : public CMaterial
{
public:
  CMaterialDecorator(CMaterial* _pDecoratedMaterial) : m_pDecoratedMaterial(_pDecoratedMaterial) {};
  virtual ~CMaterialDecorator() {CHECKED_DELETE(m_pDecoratedMaterial)};

protected:
  CMaterial* m_pDecoratedMaterial;
};

