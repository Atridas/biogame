#ifndef __BOUNDING_SPHERE_H__
#define __BOUNDING_SPHERE_H__

#include "base.h"
#include "Utils/BaseControl.h"

class CBoundingSphere:
  public CBaseControl
{
public:
  CBoundingSphere(): m_vMiddlePoint(Vect3f(0.0f)), m_fRadius(0.0f)
                    {};
  ~CBoundingSphere() { Done(); };

  bool Init(Vect3f& _vMin, Vect3f& _vMax);

  const Vect3f& GetMiddlePoint() const { return m_vMiddlePoint; };
  const float GetRadius() const { return m_fRadius; };
  const float GetDimension() const { return m_fRadius * 2.0f; };

protected:
  virtual void Release();
  
private:
  void CalcMiddlePoint(Vect3f& _vMin, Vect3f& _vMax);
  void CalcRadius(Vect3f& _vMin, Vect3f& _vMax);

  Vect3f m_vMiddlePoint;
  float  m_fRadius;
};


#endif