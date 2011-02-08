#include "Utils/BoundingBox.h"


BoundingBox::BoundingBox()
{
  for(int i = 0; i < 8; i++)
    m_vBox[i] = Vect3f(0.0f);
}

bool BoundingBox::Init(Vect3f _vMin, Vect3f _vMax)
{
  /*            4                    5
               _____________________
              /|                  /|
             / |                 / |
            /  |                /  |
           /   |               /   |
          /    |              /    |
         /     |             /     |
      6 /___________________/ 7    |
        |      | 0          |      | 1
        |      |___________ |______|
        |     /             |     / 
        |    /              |    /  
        |   /               |   /   
        |  /                |  /    
        | /                 | /     
      2 |/__________________|/ 3 
  */

  m_vBox[0] = (_vMin.x,_vMin.y,_vMin.z);
  m_vBox[1] = (_vMax.x,_vMin.y,_vMin.z);
  m_vBox[2] = (_vMin.x,_vMin.y,_vMax.z);
  m_vBox[3] = (_vMax.x,_vMin.y,_vMax.z);
  m_vBox[4] = (_vMin.x,_vMax.y,_vMin.z);
  m_vBox[5] = (_vMax.x,_vMax.y,_vMin.z);
  m_vBox[6] = (_vMin.x,_vMax.y,_vMax.z);
  m_vBox[7] = (_vMax.x,_vMax.y,_vMax.z);

  SetOk(true);

  return IsOk();
}

bool BoundingBox::Init(char* _pVertexBuffer, unsigned short _usGeometryOffset, unsigned short _usVertexSize, unsigned short _usVertexCount)
{
  Vect3f l_vMin = Vect3f(0.0f);
  Vect3f l_vMax = Vect3f(0.0f);
  float* l_vCurrent = 0;
  unsigned int l_uiBase = 0;

  //init at first vertex
  l_vCurrent = (float*)(_pVertexBuffer + _usGeometryOffset);
  l_vMin.x = l_vCurrent[0];
  l_vMin.y = l_vCurrent[1];
  l_vMin.z = l_vCurrent[2];
  l_vMax.x = l_vCurrent[0];
  l_vMax.y = l_vCurrent[1];
  l_vMax.z = l_vCurrent[2];

  for(int i = 0; i < _usVertexCount; i++)
  {
    //locate the pointer at the current vector's (x,y,z)
    l_vCurrent = (float*)(_pVertexBuffer + l_uiBase + _usGeometryOffset);

    //min
    if(l_vCurrent[0] < l_vMin.x)
      l_vMin.x = l_vCurrent[0];
    if(l_vCurrent[1] < l_vMin.y)
      l_vMin.y = l_vCurrent[0];
    if(l_vCurrent[2] < l_vMin.z)
      l_vMin.z = l_vCurrent[0];

    //max
    if(l_vCurrent[0] > l_vMax.x)
      l_vMax.x = l_vCurrent[0];
    if(l_vCurrent[1] > l_vMax.y)
      l_vMax.y = l_vCurrent[0];
    if(l_vCurrent[2] > l_vMax.z)
      l_vMax.z = l_vCurrent[0];

    l_uiBase += _usVertexSize;
  }

  return Init(l_vMin, l_vMax);
}

void BoundingBox::Release()
{
   for(int i = 0; i < 8; i++)
    m_vBox[i] = Vect3f(0.0f);
}