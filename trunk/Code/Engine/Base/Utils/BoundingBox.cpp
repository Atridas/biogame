#include "Utils/BoundingBox.h"

CBoundingBox::CBoundingBox()
{
  m_fMaxSideLenght = 0.0f;
  m_vDimension = m_vMiddlePoint = Vect3f(0.0f);

  for(int i = 0; i < 8; i++)
    m_vBox[i] = Vect3f(0.0f);

}

bool CBoundingBox::Init(Vect3f& _vMin, Vect3f& _vMax)
{
  CalcBox(_vMin, _vMax);
  CalcMiddlePoint();
  CalcDimension();
  CalcMaxSide();

  SetOk(true);

  return IsOk();
}

void CBoundingBox::CalcBox(Vect3f& _vMin, Vect3f& _vMax)
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

  m_vBox[0] = Vect3f(_vMin.x,_vMin.y,_vMin.z);
  m_vBox[1] = Vect3f(_vMax.x,_vMin.y,_vMin.z);
  m_vBox[2] = Vect3f(_vMin.x,_vMin.y,_vMax.z);
  m_vBox[3] = Vect3f(_vMax.x,_vMin.y,_vMax.z);
  m_vBox[4] = Vect3f(_vMin.x,_vMax.y,_vMin.z);
  m_vBox[5] = Vect3f(_vMax.x,_vMax.y,_vMin.z);
  m_vBox[6] = Vect3f(_vMin.x,_vMax.y,_vMax.z);
  m_vBox[7] = Vect3f(_vMax.x,_vMax.y,_vMax.z);
}

void CBoundingBox::CalcMiddlePoint()
{
  Vect3f l_vMidVector = m_vBox[7] - m_vBox[0];
  l_vMidVector *= 0.5f;
  m_vMiddlePoint = l_vMidVector + m_vBox[0];
}

void CBoundingBox::CalcDimension()
{
  float l_fSideLengthX = m_vBox[0].Distance(m_vBox[1]);
  float l_fSideLengthY = m_vBox[0].Distance(m_vBox[4]);
  float l_fSideLengthZ = m_vBox[0].Distance(m_vBox[2]);

  m_vDimension = Vect3f(l_fSideLengthX,l_fSideLengthY,l_fSideLengthZ);
}

void CBoundingBox::CalcMaxSide()
{
  float l_fMax = m_vDimension.x;

  if(l_fMax < m_vDimension.y)
     l_fMax = m_vDimension.y;
  if(l_fMax < m_vDimension.z)
     l_fMax = m_vDimension.z;

  m_fMaxSideLenght = l_fMax;
}


/*void CBoundingBox::TranslateBox(const Vect3f& _vTranslation)
{
  m_vTranslation += _vTranslation;

  Translate(_vTranslation);
}

void CBoundingBox::RotateBox(float _fPitch, float _fYaw, float _fRoll)
{
  Translate(m_vTranslation*-1);

  for(int i = 0; i < 8; i++)
  {
    m_vBox[i] = m_vBox[i].RotateX(_fPitch);
    m_vBox[i] = m_vBox[i].RotateY(_fPitch);
    m_vBox[i] = m_vBox[i].RotateZ(_fPitch);
  }

  Translate(m_vTranslation);
}

void CBoundingBox::DoYaw(float _fYaw)
{
  Translate(m_vTranslation*-1);

  for(int i = 0; i < 8; i++)
    m_vBox[i] = m_vBox[i].RotateY(_fYaw);

  Translate(m_vTranslation);
}

void CBoundingBox::DoPitch(float _fPitch)
{
  Translate(m_vTranslation*-1);

  for(int i = 0; i < 8; i++)
    m_vBox[i] = m_vBox[i].RotateZ(_fPitch);

  Translate(m_vTranslation);
}

void CBoundingBox::DoRoll(float _fRoll)
{
  Translate(m_vTranslation*-1);

  for(int i = 0; i < 8; i++)
    m_vBox[i] = m_vBox[i].RotateX(_fRoll);

  Translate(m_vTranslation);
}

void CBoundingBox::Translate(const Vect3f& _vTranslation)
{
  for(int i = 0; i < 8; i++)
  {
    m_vBox[i].x += _vTranslation.x;
    m_vBox[i].y += _vTranslation.y;
    m_vBox[i].z += _vTranslation.z;
  }
}*/

void CBoundingBox::Release()
{
}
