#include "Utils/BoundingBox.h"

CBoundingBox::CBoundingBox()
{
  m_vInitMin = Vect3f(0.0f);
  m_vInitMax = Vect3f(0.0f);

  for(int i = 0; i < 8; i++)
    m_vBox[i] = Vect3f(0.0f);

  m_bCollisional = true;
}

bool CBoundingBox::Init(Vect3f& _vMin, Vect3f& _vMax)
{

  //crear la box
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

  //establir el vector min i max inicials.
  m_vInitMin = _vMin;
  m_vInitMax = _vMax;

  m_vBox[0] = Vect3f(_vMin.x,_vMin.y,_vMin.z);
  m_vBox[1] = Vect3f(_vMax.x,_vMin.y,_vMin.z);
  m_vBox[2] = Vect3f(_vMin.x,_vMin.y,_vMax.z);
  m_vBox[3] = Vect3f(_vMax.x,_vMin.y,_vMax.z);
  m_vBox[4] = Vect3f(_vMin.x,_vMax.y,_vMin.z);
  m_vBox[5] = Vect3f(_vMax.x,_vMax.y,_vMin.z);
  m_vBox[6] = Vect3f(_vMin.x,_vMax.y,_vMax.z);
  m_vBox[7] = Vect3f(_vMax.x,_vMax.y,_vMax.z);

  //Dimensions
  float l_fSideLengthX = m_vBox[0].Distance(m_vBox[1]);
  float l_fSideLengthY = m_vBox[0].Distance(m_vBox[4]);
  float l_fSideLengthZ = m_vBox[0].Distance(m_vBox[2]);

  m_vOriginDimension = Vect3f(l_fSideLengthX,l_fSideLengthY,l_fSideLengthZ);
  m_vCurrentDimension = m_vOriginDimension;

  SetOk(true);

  return IsOk();
}

float CBoundingBox::GetMaxDistanceFromMiddle()
{
  float l_fMax = m_vOriginDimension.x;
  if(l_fMax < m_vOriginDimension.y)
    l_fMax = m_vOriginDimension.y;
  if(l_fMax < m_vOriginDimension.z)
    l_fMax = m_vOriginDimension.z;

  return l_fMax;
}

Vect3f CBoundingBox::MiddlePoint()
{
  Vect3f l_vMidVector = m_vBox[7] - m_vBox[0];
  l_vMidVector *= 0.5f;
  return l_vMidVector + m_vBox[0];
}

void CBoundingBox::TranslateBox(const Vect3f& _vTranslation)
{
  for(int i = 0; i < 8; i++)
  {
    m_vBox[i].x += _vTranslation.x;
    m_vBox[i].y += _vTranslation.y;
    m_vBox[i].z += _vTranslation.z;
  }
}

void CBoundingBox::RotateBox(float _fPitch, float _fYaw, float _fRoll)
{
  for(int i = 0; i < 8; i++)
  {
    m_vBox[i] = m_vBox[i].RotateX(_fPitch);
    m_vBox[i] = m_vBox[i].RotateY(_fPitch);
    m_vBox[i] = m_vBox[i].RotateZ(_fPitch);
  }
  //actualitzar dimensions
  RecalcCurrentDimension();
}

void CBoundingBox::DoYaw(float _fYaw)
{
  for(int i = 0; i < 8; i++)
    m_vBox[i] = m_vBox[i].RotateY(_fYaw);
  //actualitzar dimensions
  RecalcCurrentDimension();
}

void CBoundingBox::DoPitch(float _fPitch)
{
  for(int i = 0; i < 8; i++)
    m_vBox[i] = m_vBox[i].RotateX(_fPitch);
  //actualitzar dimensions
  RecalcCurrentDimension();
}

void CBoundingBox::DoRoll(float _fRoll)
{
  for(int i = 0; i < 8; i++)
    m_vBox[i] = m_vBox[i].RotateZ(_fRoll);
  //actualitzar dimensions
  RecalcCurrentDimension();
}

void CBoundingBox::RecalcCurrentDimension()
{
  float l_fSideLengthX = m_vBox[0].Distance(m_vBox[1]);
  float l_fSideLengthY = m_vBox[0].Distance(m_vBox[4]);
  float l_fSideLengthZ = m_vBox[0].Distance(m_vBox[2]);

  m_vCurrentDimension = Vect3f(l_fSideLengthX,l_fSideLengthY,l_fSideLengthZ);
}

void CBoundingBox::Release()
{
}