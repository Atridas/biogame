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
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// Vertex Cache Optimization ----------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------

struct SVCO_Triangle {
  bool added;
  float score;
  uint16 vertexs[3];
};

struct SVCO_Vertex {
  int cachePos;
  float score;
  int useInTris;
  int unusedTris;
  vector<uint16> tris;
};

#define FVS_CacheDecayPower     1.5f
#define FVS_LastTriScore        0.75f
#define FVS_ValenceBoostScale   2.0f
#define FVS_ValenceBoostPower   0.5f
#define FVS_MaxSizeVertexCache  32
float FindVertexScore ( const SVCO_Vertex &_VData )
{
  if ( _VData.unusedTris == 0 )
  {
    // No tri needs this vertex!
    return -1.0f;
  }
  float l_fScore = 0.0f;
  int l_iCachePosition = _VData.cachePos;
  if ( l_iCachePosition < 0 )
  {
    // Vertex is not in FIFO cache - no score.
  }
  else
  {
    if ( l_iCachePosition < 3 )
    {
      // This vertex was used in the last triangle,
      // so it has a fixed score, whichever of the three
      // it's in. Otherwise, you can get very different
      // answers depending on whether you add
      // the triangle 1,2,3 or 3,1,2 - which is silly.
      l_fScore = FVS_LastTriScore;
    }
    else
    {
      assert ( l_iCachePosition < FVS_MaxSizeVertexCache );
      // Points for being high in the cache.
      const float Scaler = 1.0f / ( FVS_MaxSizeVertexCache - 3 );
      l_fScore = 1.0f - ( l_iCachePosition - 3 ) * Scaler;
      l_fScore = powf ( l_fScore, FVS_CacheDecayPower );
    }
  }

  // Bonus points for having a low number of tris still to
  // use the vert, so we get rid of lone verts quickly.
  float l_fValenceBoost = powf ( (float)_VData.unusedTris, -FVS_ValenceBoostPower );
  l_fScore += FVS_ValenceBoostScale * l_fValenceBoost;
  return l_fScore;
}

float FindTriangleScore ( const SVCO_Triangle& _VTriangle, SVCO_Vertex *_VData )
{
  return _VData[_VTriangle.vertexs[0]].score + _VData[_VTriangle.vertexs[1]].score + _VData[_VTriangle.vertexs[2]].score;
}


void CopyVertex(void* _pDstBuffer, void* _pSrcBuffer, size_t _iVStride, uint32 _iDstIndex, uint32 _iSrcIndex)
{
  memcpy((void*)&(((char*)_pDstBuffer)[_iDstIndex * _iVStride]), (void*)&(((char*)_pSrcBuffer)[_iSrcIndex * _iVStride]), _iVStride);
}

struct DiscartedVertices {
  int a, b, c;
};

DiscartedVertices UpdateCache(int* _Cache, uint32 _V1, uint32 _V2, uint32 _V3)
{
  DiscartedVertices l_dv;
  l_dv.a = _V1;
  l_dv.b = _V2;
  l_dv.c = _V3;

  uint16 l_iReplaced = 0;
  for(uint16 i = 0; i < FVS_MaxSizeVertexCache; ++i)
  {
    if(_V1 == _Cache[i] || _V2 == _Cache[i] || _V3 == _Cache[i])
    {
      l_iReplaced++;
      _Cache[i] = l_dv.a;
      l_dv.a = l_dv.b;
      l_dv.b = l_dv.c;
      l_dv.c = -1;
    }
    else
    {
      int aux = _Cache[i];
      _Cache[i] = l_dv.a;
      l_dv.a = l_dv.b;
      l_dv.b = l_dv.c;
      l_dv.c = aux;
    }
  }
  return l_dv;
}

int RecomputeScores(int* _Cache, const DiscartedVertices& _dv, SVCO_Vertex* _vVertex, SVCO_Triangle* _vTriangles, uint16 _iNumTriangles)
{
  //Recompute Vertex scores
  for(uint16 i = 0; i < FVS_MaxSizeVertexCache; ++i)
  {
    if(_Cache[i] < 0) break; //No hi ha res més a la cache

    SVCO_Vertex* l_pVertex = _vVertex + _Cache[i];
    l_pVertex->cachePos = i;
    l_pVertex->score = FindVertexScore(*l_pVertex);
  }
  if(_dv.a >= 0)
  {
    SVCO_Vertex* l_pVertex = _vVertex + _dv.a;
    l_pVertex->cachePos = -1;
    l_pVertex->score = FindVertexScore(*l_pVertex);
  }
  if(_dv.b >= 0)
  {
    SVCO_Vertex* l_pVertex = _vVertex + _dv.b;
    l_pVertex->cachePos = -1;
    l_pVertex->score = FindVertexScore(*l_pVertex);
  }
  if(_dv.c >= 0)
  {
    SVCO_Vertex* l_pVertex = _vVertex + _dv.c;
    l_pVertex->cachePos = -1;
    l_pVertex->score = FindVertexScore(*l_pVertex);
  }

  //Recompute Triangle Scores
  int l_iBestTriangle = -1;
  for(uint16 i = 0; i < FVS_MaxSizeVertexCache; ++i)
  {
    if(_Cache[i] < 0) break; //No hi ha res més a la cache
    SVCO_Vertex* l_pVertex = _vVertex + _Cache[i];
    for(uint16 j = 0; j < l_pVertex->tris.size(); ++j)
    {
      SVCO_Triangle* l_pTriangle = _vTriangles + l_pVertex->tris[j];
      if(!l_pTriangle->added)
      {
        l_pTriangle->score = FindTriangleScore(*l_pTriangle, _vVertex);
        if(l_iBestTriangle < 0 || l_pTriangle->score > _vTriangles[l_iBestTriangle].score)
        {
          l_iBestTriangle = l_pVertex->tris[j];
        }
      }
    }
  }
  if(_dv.a >= 0)
  {
    SVCO_Vertex* l_pVertex = _vVertex + _dv.a;
    for(uint16 j = 0; j < l_pVertex->tris.size(); ++j)
    {
      SVCO_Triangle* l_pTriangle = _vTriangles + l_pVertex->tris[j];
      if(!l_pTriangle->added)
      {
        l_pTriangle->score = FindTriangleScore(*l_pTriangle, _vVertex);
        if(l_iBestTriangle < 0 || l_pTriangle->score > _vTriangles[l_iBestTriangle].score)
        {
          l_iBestTriangle = l_pVertex->tris[j];
        }
      }
    }
  }
  if(_dv.b >= 0)
  {
    SVCO_Vertex* l_pVertex = _vVertex + _dv.b;
    for(uint16 j = 0; j < l_pVertex->tris.size(); ++j)
    {
      SVCO_Triangle* l_pTriangle = _vTriangles + l_pVertex->tris[j];
      if(!l_pTriangle->added)
      {
        l_pTriangle->score = FindTriangleScore(*l_pTriangle, _vVertex);
        if(l_iBestTriangle < 0 || l_pTriangle->score > _vTriangles[l_iBestTriangle].score)
        {
          l_iBestTriangle = l_pVertex->tris[j];
        }
      }
    }
  }
  if(_dv.c >= 0)
  {
    SVCO_Vertex* l_pVertex = _vVertex + _dv.c;
    for(uint16 j = 0; j < l_pVertex->tris.size(); ++j)
    {
      SVCO_Triangle* l_pTriangle = _vTriangles + l_pVertex->tris[j];
      if(!l_pTriangle->added)
      {
        l_pTriangle->score = FindTriangleScore(*l_pTriangle, _vVertex);
        if(l_iBestTriangle < 0 || l_pTriangle->score > _vTriangles[l_iBestTriangle].score)
        {
          l_iBestTriangle = l_pVertex->tris[j];
        }
      }
    }
  }

  if(l_iBestTriangle < 0)
  {
    for(uint16 i = 0; i < _iNumTriangles; ++i)
    {
      if(!_vTriangles[i].added && (l_iBestTriangle < 0 || _vTriangles[i].score > _vTriangles[l_iBestTriangle].score))
      {
        l_iBestTriangle = i;
      }
    }
  }

  return l_iBestTriangle;
}

void VertexCacheOptimisation( void *_VData, uint16 *_IData, size_t _iVCount,
                              size_t _iICount, size_t _iVStride              )
{
  uint16         l_iNumTriangles     = _iICount / 3;
  char*          l_vData             = new char[_iVStride * _iVCount];
  SVCO_Vertex*   l_vVertex           = new SVCO_Vertex[_iVCount];
  SVCO_Triangle* l_vTriangles        = new SVCO_Triangle[l_iNumTriangles];
  uint16 *       l_iTrianglesInOrder = new uint16[l_iNumTriangles];
  int*           l_MappedIndexes     = new int[_iVCount];
  for(uint16 i = 0; i < _iVCount; ++i) l_MappedIndexes[i] = -1;


  // Initialization ------------------------------------------------------------------------------------------
  memcpy((void*)l_vData,_VData,_iVStride*_iVCount); //Copiem les dades a un buffer per separat (després les voldrem reordenar)

  // Inicialitzem els vertexos
  for(uint16 i = 0; i < _iVCount; ++i)
  {
    //CopyVertex(l_vData,_VData,_iVStride,i,i);
    l_vVertex[i].cachePos   = -1;
    l_vVertex[i].useInTris  = 0;
    l_vVertex[i].unusedTris = 0;
  }


  //Inicialitzem els triangles
  for(uint16 i = 0; i < l_iNumTriangles; ++i)
  {
    l_vTriangles[i].added      = false;
    // Per cada vertex
    for(uint16 j = 0; j < 3; ++j)
    {
      //n'agafem l'index
      uint32 l_iVertexIndex = _IData[i*3 + j];
      l_vTriangles[i].vertexs[j] = l_iVertexIndex;

      //indiquem al vertex corresponent que ès adjacent a un triangle.
      l_vVertex[l_iVertexIndex].useInTris  ++;
      l_vVertex[l_iVertexIndex].unusedTris ++;

      l_vVertex[l_iVertexIndex].tris.push_back(i);
    }
  }

  //Calculem les puntuacions per vertex...
  for(uint16 i = 0; i < _iVCount; ++i)
  {
    l_vVertex[i].score = FindVertexScore(l_vVertex[i]);
  }

  //... i per triangle (ja que hi som, agafem el millor triangle)
  int l_iBestTriangle = 0;
  for(uint16 i = 0; i < l_iNumTriangles; ++i)
  {
    l_vTriangles[i].score = FindTriangleScore(l_vTriangles[i],l_vVertex);
    if(l_vTriangles[i].score > l_vTriangles[l_iBestTriangle].score)
    {
      l_iBestTriangle = i;
    }
  }

  // Main Body ---------------------------------------------------------------------------------------------------
  int l_Cache[FVS_MaxSizeVertexCache];
  
  // inicialitzem la cache
  for(uint16 i = 0; i < FVS_MaxSizeVertexCache; ++i)
  {
    l_Cache[i] = -1;
  }

  //El cos s'ha de fer un cop per triangle (busquem sempre el millor triangle a afegim i l'afegim, mai es fa backtracking ni lookahead)
  for(uint16 i = 0; i < l_iNumTriangles; ++i)
  {
    assert(!l_vTriangles[l_iBestTriangle].added);
    l_iTrianglesInOrder[i] = l_iBestTriangle;
    if(i == l_iNumTriangles-1) break; // És l'últim triangle, no cal que recalculem res!

    // Setejem com a afegit el triangle
    l_vTriangles[l_iBestTriangle].added = true;

    // Indiquem als vertexos utilitzats que ja els fa servir un triangle menys
    l_vVertex[l_vTriangles[l_iBestTriangle].vertexs[0]].unusedTris--;
    l_vVertex[l_vTriangles[l_iBestTriangle].vertexs[1]].unusedTris--;
    l_vVertex[l_vTriangles[l_iBestTriangle].vertexs[2]].unusedTris--;

    // Actualitzem la cache amb els 3 vertexos nous (ens retorna els descartats)
    DiscartedVertices l_dv = UpdateCache(l_Cache, 
                                          l_vTriangles[l_iBestTriangle].vertexs[0],
                                          l_vTriangles[l_iBestTriangle].vertexs[1],
                                          l_vTriangles[l_iBestTriangle].vertexs[2]);

    // Actualitzem les puntuacions i agafem el millor triangle.
    l_iBestTriangle = RecomputeScores(l_Cache, l_dv, l_vVertex, l_vTriangles, l_iNumTriangles);
  }


  // Reordering pre-transform cache -------------------------------------------------------------
  uint16 l_iVertexPos = 0;
  for(uint16 i = 0; i < l_iNumTriangles; ++i)
  {
    SVCO_Triangle* l_pTriangle = l_vTriangles + l_iTrianglesInOrder[i];

    for(uint16 j = 0; j < 3; ++j)
    {
      if(l_MappedIndexes[l_pTriangle->vertexs[j]] < 0)
      {
        //No està afegit al nou vertex buffer
        // 1 - Afegim les dades
        CopyVertex(_VData,l_vData,_iVStride,l_iVertexPos,l_pTriangle->vertexs[j]);
        // 2 - Afegim el "mapejat" del vell index al nou index
        l_MappedIndexes[l_pTriangle->vertexs[j]] = l_iVertexPos;
        // 3 - Actualitzem la posició al nou buffer
        ++l_iVertexPos;
      }
      // Copiem els indexos reordenats
      _IData[i * 3 + j] = l_MappedIndexes[l_pTriangle->vertexs[j]];
    }
  }

  //netejem
  delete[] l_MappedIndexes;
  delete[] l_iTrianglesInOrder;
  delete[] l_vTriangles;
  delete[] l_vVertex;
  delete[] l_vData;
}

// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------
