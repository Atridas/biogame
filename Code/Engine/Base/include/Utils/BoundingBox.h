#ifndef __BOUNDING_BOX_H__
#define __BOUNDING_BOX_H__

#include "base.h"
#include "Utils/BaseControl.h"

class CBoundingBox:
  public CBaseControl
{
public:
  CBoundingBox();
  ~CBoundingBox() { Done(); };

  bool Init(Vect3f& _vMin, Vect3f& _vMax);

  void TranslateBox(const Vect3f& _Translation);
  void RotateBox(float _fPitch, float _fYaw, float _fRoll);
  void DoPitch(float _fPitch);
  void DoYaw(float _fYaw);
  void DoRoll(float _fRoll);
  Vect3f MiddlePoint();
  
  bool GetCollisional () { return m_bCollisional; };
  float GetMaxDistanceFromMiddle();
  Vect3f GetMin() { return m_vBox[0]; };
  Vect3f GetMax() { return m_vBox[7]; };
  Vect3f GetInitMin() { return m_vInitMin; };
  Vect3f GetInitMax() { return m_vInitMax; };
  Vect3f* GetBox() { return m_vBox; };
  Vect3f GetOriginDimension() { return m_vOriginDimension; };
  Vect3f GetCurrentDimension() { return m_vCurrentDimension; };

  void SetCollisional ( bool _bCollisional) { m_bCollisional = _bCollisional; };

protected:
  virtual void Release();
  
private:
  void   RecalcCurrentDimension();
  Vect3f m_vInitMin;
  Vect3f m_vInitMax;

  Vect3f m_vOriginDimension;
  Vect3f m_vCurrentDimension;

  Vect3f m_vBox[8];
  bool   m_bCollisional;
};

#endif