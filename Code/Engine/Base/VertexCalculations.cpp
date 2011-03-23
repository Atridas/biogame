#include "VertexCalculations.h"

#include <cstdlib>

// Crear les tangents i binormals
void CalcTangentsAndBinormals(void *VtxsData, uint16 *IdxsData, size_t VtxCount,
                              size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride,
                              size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride)
{
  D3DXVECTOR3 *tan1 = new D3DXVECTOR3[VtxCount * 2];
  D3DXVECTOR3 *tan2 = tan1 + VtxCount;
  ZeroMemory(tan1, VtxCount * sizeof(D3DXVECTOR3) * 2);
  unsigned char *l_VtxAddress=(unsigned char *)VtxsData;
  for(size_t b=0;b<IdxCount;b+=3)
  {
    uint16 i1=IdxsData[b];
    uint16 i2=IdxsData[b+1];
    uint16 i3=IdxsData[b+2];
    D3DXVECTOR3 *v1=(D3DXVECTOR3 *) &l_VtxAddress[i1*VertexStride+GeometryStride];
    D3DXVECTOR3 *v2=(D3DXVECTOR3 *) &l_VtxAddress[i2*VertexStride+GeometryStride];
    D3DXVECTOR3 *v3=(D3DXVECTOR3 *) &l_VtxAddress[i3*VertexStride+GeometryStride];
    D3DXVECTOR2 *w1=(D3DXVECTOR2 *) &l_VtxAddress[i1*VertexStride+TextureCoordsStride];
    D3DXVECTOR2 *w2=(D3DXVECTOR2 *) &l_VtxAddress[i2*VertexStride+TextureCoordsStride];
    D3DXVECTOR2 *w3=(D3DXVECTOR2 *) &l_VtxAddress[i3*VertexStride+TextureCoordsStride];
    float x1=v2->x-v1->x;
    float x2=v3->x-v1->x;
    float y1=v2->y-v1->y;
    float y2=v3->y-v1->y;
    float z1=v2->z-v1->z;
    float z2=v3->z-v1->z;
    float s1=w2->x-w1->x;
    float s2=w3->x-w1->x;
    float t1=w2->y-w1->y;
    float t2=w3->y-w1->y;
    float r = 1.0F / (s1 * t2 - s2 * t1);
    D3DXVECTOR3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
    D3DXVECTOR3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);
    assert(i1<VtxCount);
    assert(i2<VtxCount);
    assert(i3<VtxCount);
    tan1[i1] += sdir;
    tan1[i2] += sdir;
    tan1[i3] += sdir;
    tan2[i1] += tdir;
    tan2[i2] += tdir;
    tan2[i3] += tdir;
  }
  for (size_t b=0;b<VtxCount;++b)
  {
    D3DXVECTOR3 *l_NormalVtx=(D3DXVECTOR3 *) &l_VtxAddress[b*VertexStride+NormalStride];
    D3DXVec3Normalize(l_NormalVtx,l_NormalVtx);
    D3DXVECTOR3 *l_TangentVtx=(D3DXVECTOR3 *) &l_VtxAddress[b*VertexStride+TangentStride];
    D3DXVECTOR4 *l_TangentVtx4=(D3DXVECTOR4 *) &l_VtxAddress[b*VertexStride+TangentStride];
    D3DXVECTOR3 *l_BiNormalVtx=(D3DXVECTOR3 *) &l_VtxAddress[b*VertexStride+BiNormalStride];
    const D3DXVECTOR3& t=tan1[b];
    // Gram-Schmidt orthogonalize
    D3DXVECTOR3 l_VAl=t-(*l_NormalVtx)*D3DXVec3Dot(l_NormalVtx, &t);
    D3DXVec3Normalize(l_TangentVtx,&l_VAl);
    //tangent[a] = (t - n * Dot(n, t)).Normalize();
    // Calculate handedness
    D3DXVECTOR3 l_Cross;
    D3DXVec3Cross(&l_Cross,l_NormalVtx,l_TangentVtx);
    l_TangentVtx4->w=(D3DXVec3Dot(&l_Cross,&tan2[b])< 0.0f ) ? -1.0f : 1.0f;
    //tangent[a].w = (Dot(Cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
    D3DXVec3Cross(l_BiNormalVtx,l_NormalVtx,l_TangentVtx);
  }
  delete[] tan1;
}

// http://www.terathon.com/code/tangent.html
void CalcTangentsAndBinormalsBis( void *VtxsData, uint16 *IdxsData, size_t VtxCount,
                                  size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride,
                                  size_t TangentStride, size_t BitangentStride, size_t TextureCoordsStride)
{
  D3DXVECTOR3 *tangent   = new D3DXVECTOR3[VtxCount*2];
  D3DXVECTOR3 *bitangent = tangent + VtxCount;
  ZeroMemory(tangent, VtxCount * sizeof(D3DXVECTOR3) * 2);
  unsigned char *l_VtxAddress=(unsigned char *)VtxsData;
  for(size_t b=0;b<IdxCount;b+=3)
  {
    uint16 i1=IdxsData[b];
    uint16 i2=IdxsData[b+1];
    uint16 i3=IdxsData[b+2];
    D3DXVECTOR3 *v1=(D3DXVECTOR3 *) &l_VtxAddress[i1*VertexStride+GeometryStride];
    D3DXVECTOR3 *v2=(D3DXVECTOR3 *) &l_VtxAddress[i2*VertexStride+GeometryStride];
    D3DXVECTOR3 *v3=(D3DXVECTOR3 *) &l_VtxAddress[i3*VertexStride+GeometryStride];
    D3DXVECTOR2 *w1=(D3DXVECTOR2 *) &l_VtxAddress[i1*VertexStride+TextureCoordsStride];
    D3DXVECTOR2 *w2=(D3DXVECTOR2 *) &l_VtxAddress[i2*VertexStride+TextureCoordsStride];
    D3DXVECTOR2 *w3=(D3DXVECTOR2 *) &l_VtxAddress[i3*VertexStride+TextureCoordsStride];
    
    D3DXVECTOR3 Q1, Q2;
    D3DXVec3Subtract(&Q1, v2, v1);
    D3DXVec3Subtract(&Q2, v3, v1);
    
    float s1 = w2->x - w1->x;
    float t1 = w2->y - w1->y;
    float s2 = w3->x - w1->x;
    float t2 = w3->y - w1->y;
    
    D3DXVECTOR3 tangentDir  ((t2 * Q1.x - t1 * Q2.x), (t2 * Q1.y - t1 * Q2.y), (t2 * Q1.z - t1 * Q2.z));
    D3DXVECTOR3 bitangentDir((s1 * Q2.x - s2 * Q1.x), (s1 * Q2.y - s2 * Q1.y), (s1 * Q2.z - s2 * Q1.z));
    
    D3DXVec3Normalize(&tangentDir,&tangentDir);
    D3DXVec3Normalize(&bitangentDir,&bitangentDir);
    
    tangent[i1]   += tangentDir;
    tangent[i2]   += tangentDir;
    tangent[i3]   += tangentDir;

    bitangent[i1] += bitangentDir;
    bitangent[i2] += bitangentDir;
    bitangent[i3] += bitangentDir;
  }
  for (size_t b=0;b<VtxCount;++b)
  {
    D3DXVECTOR3 *l_NormalVtx=(D3DXVECTOR3 *) &l_VtxAddress[b*VertexStride+NormalStride];
    D3DXVECTOR3 *l_TangentVtx=(D3DXVECTOR3 *) &l_VtxAddress[b*VertexStride+TangentStride];
    D3DXVECTOR3 *l_BitangentVtx=(D3DXVECTOR3 *) &l_VtxAddress[b*VertexStride+BitangentStride];

    // Gram-Schmidt orthogonalize
    *l_TangentVtx   = tangent[b]   - D3DXVec3Dot(l_NormalVtx,  &tangent[b]  ) * (*l_NormalVtx)  / D3DXVec3Dot(l_NormalVtx,  l_NormalVtx );
    *l_BitangentVtx = bitangent[b] - D3DXVec3Dot(l_NormalVtx,  &bitangent[b]) * (*l_NormalVtx)  / D3DXVec3Dot(l_NormalVtx,  l_NormalVtx )
                                   - D3DXVec3Dot(l_TangentVtx, &bitangent[b]) * (*l_TangentVtx) / D3DXVec3Dot(l_TangentVtx, l_TangentVtx );

    
    D3DXVec3Normalize(l_NormalVtx,   l_NormalVtx);
    D3DXVec3Normalize(l_TangentVtx,  l_TangentVtx);
    D3DXVec3Normalize(l_BitangentVtx,l_BitangentVtx);
  }
  delete[] tangent;
}



void CreatePoissonBlur16x2( float pfKernel_[32] )
{
  
  //Creem nombres aleatoris de 0 a 1
  for(int i = 0; i < 32; i++)
  {
    pfKernel_[i] = ((float)rand() / (float)RAND_MAX);
  }

  
  //fem una caixa 4x4 de nombres aleatoris en caselles
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 4; j++)
    {
      pfKernel_[2*(i*4+j)  ] += i;
      pfKernel_[2*(i*4+j)+1] += j;
    }
  }
  
  //escalar a una caixa 1x1
  for(int i = 0; i < 32; i++)
  {
    pfKernel_[i] *= 0.25f;
  }

  //ho posem a un cercle
  for(int i = 0; i < 16; i++)
  {
    float u = pfKernel_[i*2  ];
    float v = pfKernel_[i*2+1];
    
    float x = sqrt(u) * cos(v * 2 * FLOAT_PI_VALUE);
    float y = sqrt(u) * sin(v * 2 * FLOAT_PI_VALUE);
    
    pfKernel_[i*2  ] = x;
    pfKernel_[i*2+1] = y;
  }
  
}

void CalcMinMaxCoord(char* _pVertexBuffer, unsigned short _usGeometryOffset, unsigned short _usVertexSize, unsigned short _usVertexCount, Vect3f& _vMin, Vect3f& _vMax)
{
  float* l_vCurrent = 0;
  unsigned int l_uiBase = 0;

  //init at first vertex
  l_vCurrent = (float*)(_pVertexBuffer + _usGeometryOffset);
  _vMin.x = l_vCurrent[0];
  _vMin.y = l_vCurrent[1];
  _vMin.z = l_vCurrent[2];
  _vMax.x = l_vCurrent[0];
  _vMax.y = l_vCurrent[1];
  _vMax.z = l_vCurrent[2];

  for(int i = 0; i < _usVertexCount; i++)
  {
    //locate the pointer at the current vector's (x,y,z)
    l_vCurrent = (float*)(_pVertexBuffer + l_uiBase + _usGeometryOffset);

    //min
    if(l_vCurrent[0] < _vMin.x)
      _vMin.x = l_vCurrent[0];
    if(l_vCurrent[1] < _vMin.y)
      _vMin.y = l_vCurrent[1];
    if(l_vCurrent[2] < _vMin.z)
      _vMin.z = l_vCurrent[2];

    //max
    if(l_vCurrent[0] > _vMax.x)
      _vMax.x = l_vCurrent[0];
    if(l_vCurrent[1] > _vMax.y)
      _vMax.y = l_vCurrent[1];
    if(l_vCurrent[2] > _vMax.z)
      _vMax.z = l_vCurrent[2];

    l_uiBase += _usVertexSize;
  }
}


vector<Vect3f> CalcSimpleVertexBuffer(char* _pVertexBuffer, unsigned short _usVertexSize, unsigned short _usVertexCount)
{
  unsigned int l_uiBase = 0;
  vector<Vect3f> l_vBuffer;

  for(int i = 0; i < _usVertexCount; i++)
  {
    D3DXVECTOR3 *v1=(D3DXVECTOR3 *) &_pVertexBuffer[i*_usVertexSize];
    l_vBuffer.push_back(Vect3f(v1->x,v1->y,v1->z));
  }
  return l_vBuffer;
}

vector<uint32> CalcSimpleIndexBuffer(uint16* _pIndexBuffer, unsigned short _usIndexSize, unsigned short _usIndexCount)
{
  unsigned int l_uiBase = 0;
  vector<uint32> l_vBuffer;
  uint32 l_uiIndex;

  for(int i = 0; i < _usIndexCount; i++)
  {
    l_uiIndex = (uint32) _pIndexBuffer[i];
    l_vBuffer.push_back(l_uiIndex);
    //l_uiBase += _usIndexSize;
  }

  return l_vBuffer;
}
