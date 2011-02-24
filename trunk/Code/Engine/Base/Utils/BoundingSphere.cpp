#include "Utils/BoundingSphere.h"


bool CBoundingSphere::Init(Vect3f& _vMin, Vect3f& _vMax)
{
  CalcMiddlePoint(_vMin, _vMax);
  CalcRadius(_vMin, _vMax);

  SetOk(true);

  return IsOk();
}

void CBoundingSphere::CalcMiddlePoint(Vect3f& _vMin, Vect3f& _vMax)
{
  Vect3f l_vMidVector = _vMax - _vMin;
  l_vMidVector *= 0.5f;
  m_vMiddlePoint = l_vMidVector + _vMin;
}

void CBoundingSphere::CalcRadius(Vect3f& _vMin, Vect3f& _vMax)
{
  m_fRadius = _vMin.Distance(_vMax) * 0.5f;
}

void CBoundingSphere::Release()
{

}

