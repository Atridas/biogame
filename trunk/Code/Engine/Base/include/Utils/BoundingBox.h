#ifndef __BOUNDING_BOX_H__
#define __BOUNDING_BOX_H__

#include "base.h"
#include "Utils/BaseControl.h"

class BoundingBox:
  public CBaseControl
{
public:
  BoundingBox();
  ~BoundingBox() { Done(); };
  bool Init(Vect3f _vMin, Vect3f _vMax);
  bool Init(char* _pVertexBuffer, unsigned short _usGeometryOffset, unsigned short _usVertexSize, unsigned short _usVertexCount);

  Vect3f GetMin()  { return m_vBox[0]; };
  Vect3f GetMax()  { return m_vBox[7]; };
  Vect3f* GetBox() { return m_vBox; };
protected:
  virtual void Release();
  
private:
  Vect3f m_vBox[8];
};

#endif