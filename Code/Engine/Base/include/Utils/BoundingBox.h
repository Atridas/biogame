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
  float GetMaxSideLength();
  const Vect3f& GetMin() const { return m_vBox[0]; };
  const Vect3f& GetMax() const { return m_vBox[7]; };
  const Vect3f& GetInitMin() const { return m_vInitMin; };
  const Vect3f& GetInitMax() const { return m_vInitMax; };
  const Vect3f* GetBox() const { return m_vBox; };
  const Vect3f& GetDimension() const { return m_vDimension; };

  void SetCollisional ( bool _bCollisional) { m_bCollisional = _bCollisional; };

protected:
  virtual void Release();
  
private:
  void   Translate(const Vect3f& _vTranslation);
  
  Vect3f m_vInitMin;
  Vect3f m_vInitMax;

  Vect3f m_vTranslation;

  Vect3f m_vDimension;

  Vect3f m_vBox[8];
  bool   m_bCollisional;
};

#endif