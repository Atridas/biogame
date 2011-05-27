#include "Utils/BoundingBox.h"
class CalVector;

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

bool CBoundingBox::Init(Vect3f _vPoints[8])
{
  m_vBox[0] = Vect3f(_vPoints[0].x,_vPoints[0].y,_vPoints[0].z);
  m_vBox[1] = Vect3f(_vPoints[1].x,_vPoints[1].y,_vPoints[1].z);
  m_vBox[2] = Vect3f(_vPoints[2].x,_vPoints[2].y,_vPoints[2].z);
  m_vBox[3] = Vect3f(_vPoints[3].x,_vPoints[3].y,_vPoints[3].z);
  m_vBox[4] = Vect3f(_vPoints[4].x,_vPoints[4].y,_vPoints[4].z);
  m_vBox[5] = Vect3f(_vPoints[5].x,_vPoints[5].y,_vPoints[5].z);
  m_vBox[6] = Vect3f(_vPoints[6].x,_vPoints[6].y,_vPoints[6].z);
  m_vBox[7] = Vect3f(_vPoints[7].x,_vPoints[7].y,_vPoints[7].z);

  CalcMiddlePoint();
  CalcDimension2();

  /*m_vBox[0] -= m_vMiddlePoint;
  m_vBox[1] -= m_vMiddlePoint;
  m_vBox[2] -= m_vMiddlePoint;
  m_vBox[3] -= m_vMiddlePoint;
  m_vBox[4] -= m_vMiddlePoint;
  m_vBox[5] -= m_vMiddlePoint;
  m_vBox[6] -= m_vMiddlePoint;
  m_vBox[7] -= m_vMiddlePoint;*/

  //CalcMiddlePoint();
  

  /*m_vBox[0] += m_vDimension*0.5;
  m_vBox[1] += m_vDimension*0.5;
  m_vBox[2] += m_vDimension*0.5;
  m_vBox[3] += m_vDimension*0.5;
  m_vBox[4] += m_vDimension*0.5;
  m_vBox[5] += m_vDimension*0.5;
  m_vBox[6] += m_vDimension*0.5;
  m_vBox[7] += m_vDimension*0.5;*/

  //CalcMiddlePoint();
  //CalcMaxSide();

  SetOk(true);

  return IsOk();
}

bool CBoundingBox::Init(Vect3f _vDimensions)
{
  CalcBox(_vDimensions * -.5f, _vDimensions *.5f);
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


void CBoundingBox::CalcDimension2()
{
  float l_fSideLengthX = m_vBox[0].Distance(m_vBox[4]);
  float l_fSideLengthY = m_vBox[0].Distance(m_vBox[1]);
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
