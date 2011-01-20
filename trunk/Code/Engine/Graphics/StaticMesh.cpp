#include "Core.h"
#include "RenderManager.h"
#include "StaticMesh.h"
#include "VertexsStructs.h"
#include "Texture.h"
#include "TextureManager.h"
#include "EffectManager.h"
#include "EffectTechnique.h"
#include <IndexedVertexs.h>
#include <base.h>
#include <fstream>

#define HEADER 0xAAAA
#define FOOTER 0xFFFF


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

bool CStaticMesh::Load(const string &_szFileName)
{
  /** Format fitxer
    *
    *  unsigned short              -> HEADER(MASK = 0xAAAA)
    *  unsigned short              -> MaterialCount. nombre de materials
    *
    *  for MaterialCount
    *  	unsigned short            -> VERTEX_TYPE.
    *   unsigned short            -> nombre de propietats a llegir
    *   [unsigned short, string]  -> Diffuse. u16: Longitud de la cadena u16. string: path.
    *   [unsigned short, string]  -> Bumpmap. u16: Longitud de la cadena u16. string: path.
    *   [unsigned short, string]  -> Lightmap. u16: Longitud de la cadena u16. string: path.
    *   [unsigned short, string]  -> Environment. u16: Longitud de la cadena u16. string: path.
    *
    *  for MaterialCount
    *  	unsigned short            -> VertexCount. Nombre de vèrtexs.
    *   for VertexCount
    *     VERTEX_STRUCT           -> Vèrtexs. Llista de vèrtexs. Format depenent de VERTEX_TYPE.
    *
    *  	unsigned short            -> IndexCount. Nombre d'índexs.
    *  	for IndexCount
    *  		unsigned short          -> Índexs. Llista d'índexs de vèrtexs.
    *  
    *  unsigned short             -> FOOTER(MASK = 0xFFFF)
    *
    * Fi Format fitxer
   **/
  LOGGER->AddNewLog(ELL_INFORMATION, "CStaticMesh::Load \"%s\"", _szFileName.c_str());
  
  fstream l_File;
  uint16 l_usHelper = 0;
  uint16 l_usNumMaterials = 0;
  uint16* l_pusVertexType = 0;
  uint16* l_pusTextureNum = 0;

  CRenderManager* l_pRenderManager = CORE->GetRenderManager();

  CTextureManager* l_pTextureManager = l_pRenderManager->GetTextureManager();
  
  l_File.open(_szFileName, fstream::in | fstream::binary );
  if(!l_File.is_open())
  {
    LOGGER->AddNewLog(ELL_WARNING, "CStaticMesh::Load no s'ha pogut obrir el fitxer.");
    CHECKED_DELETE_ARRAY(l_pusVertexType);
    CHECKED_DELETE_ARRAY(l_pusTextureNum);
    return false;
  }

  // ----------------------------- HEADER -----------------------------------------
  l_File.read((char*)&l_usHelper, sizeof(uint16));

  if(l_usHelper != HEADER)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CStaticMesh::Load reading file with incorrect header");
    CHECKED_DELETE_ARRAY(l_pusVertexType);
    CHECKED_DELETE_ARRAY(l_pusTextureNum);
    l_File.close();
    return false;
  }
  
  // ----------------------------- MATERIALS --------------------------------------
  //num materials
  l_File.read((char*)&l_usNumMaterials, sizeof(uint16));
  
  l_pusVertexType = new uint16[l_usNumMaterials];
  l_pusTextureNum = new uint16[l_usNumMaterials];


  for(int i = 0; i < l_usNumMaterials; i++)
  {
    
    m_Textures.push_back(vector<CTexture*>());
    //Vertex Type
    l_File.read((char*)&(l_pusVertexType[i]), sizeof(uint16));
    //l_File.read((char*)&(l_pusTextureNum[i]), sizeof(uint16));
    l_pusTextureNum[i] = GetTextureNum(l_pusVertexType[i]);

    for(int j = 0; j < l_pusTextureNum[i]; j++)
    {
      uint16 l_usTextLen = 0;
      l_File.read((char*)&l_usTextLen, sizeof(uint16));
      char* l_pcTexture = new char[++l_usTextLen];
    
      l_File.read(l_pcTexture, sizeof(char) * l_usTextLen);

      m_Textures[i].push_back(l_pTextureManager->GetResource(l_pcTexture));
      if(m_Textures[i][j] == 0)
      {
        //No hi ha logger ja que ja el posa la textura
        CHECKED_DELETE_ARRAY(l_pusVertexType);
        CHECKED_DELETE_ARRAY(l_pusTextureNum);
        delete l_pcTexture;
        l_File.close();
        return false;
      }



      delete l_pcTexture;
    }

  }

  
  // ----------------------------- BUFFERS ----------------------------------------
  for(int i = 0; i < l_usNumMaterials; i++)
  {
    uint16 l_VertexCount;
    uint16 l_usVertexSize = GetVertexSize(l_pusVertexType[i]);
  
    l_File.read((char*)&l_VertexCount, sizeof(uint16));

    char* l_pVertexBuffer = new char[l_VertexCount*l_usVertexSize];
    l_File.read(&l_pVertexBuffer[0], l_usVertexSize*l_VertexCount);
  
  
    uint32 l_IndexCount;

    l_File.read((char*)&l_IndexCount, sizeof(uint32));

    /*TNORMALTANGENTBINORMALTEXTURED2VERTEX aux[24], *aux2 = (TNORMALTANGENTBINORMALTEXTURED2VERTEX*)l_pVertexBuffer;
    for(int k = 0; k < 24; k++)
    {
      aux[k] = aux2[k];
    }*/

    uint16 * l_pIndexList = new uint16[l_IndexCount];
    l_File.read((char *)&l_pIndexList[0], sizeof(uint16)*l_IndexCount);

    //CIndexedVertexs<SNORMALTEXTUREDVERTEX> *l_IndexedVertexs=new CIndexedVertexs<SNORMALTEXTUREDVERTEX>(RENDER_MANAGER, (SNORMALTEXTUREDVERTEX*)l_pVertexBuffer, l_pIndexList, l_VertexCount, l_IndexCount);
    CRenderableVertexs* l_RenderableVertexs = 0;

    if(l_pusVertexType[i] == SNORMALTEXTUREDVERTEX::GetVertexType())
    {
      l_RenderableVertexs = new CIndexedVertexs<SNORMALTEXTUREDVERTEX>( l_pRenderManager,
                                                                        l_pVertexBuffer,
                                                                        l_pIndexList,
                                                                        l_VertexCount, 
                                                                        l_IndexCount);
    } else if(l_pusVertexType[i] == SNORMALTEXTURED2VERTEX::GetVertexType())
    {
      l_RenderableVertexs = new CIndexedVertexs<SNORMALTEXTURED2VERTEX>(  l_pRenderManager,
                                                                          l_pVertexBuffer,
                                                                          l_pIndexList,
                                                                          l_VertexCount, 
                                                                          l_IndexCount);
    } else if(l_pusVertexType[i] == SDIFFUSEVERTEX::GetVertexType())
    {
      l_RenderableVertexs = new CIndexedVertexs<SDIFFUSEVERTEX>(  l_pRenderManager,
                                                                  l_pVertexBuffer,
                                                                  l_pIndexList,
                                                                  l_VertexCount, 
                                                                  l_IndexCount);
    } else if(l_pusVertexType[i] == STEXTUREDVERTEX::GetVertexType())
    {
      l_RenderableVertexs = new CIndexedVertexs<STEXTUREDVERTEX>( l_pRenderManager,
                                                                        l_pVertexBuffer,
                                                                        l_pIndexList,
                                                                        l_VertexCount, 
                                                                        l_IndexCount);
    } else if(l_pusVertexType[i] == STEXTURED2VERTEX::GetVertexType())
    {
      l_RenderableVertexs = new CIndexedVertexs<STEXTURED2VERTEX>(  l_pRenderManager,
                                                                    l_pVertexBuffer,
                                                                    l_pIndexList,
                                                                    l_VertexCount, 
                                                                    l_IndexCount);
    } else if(l_pusVertexType[i] == SNORMALDIFFUSEVERTEX::GetVertexType())
    {
      l_RenderableVertexs = new CIndexedVertexs<SNORMALDIFFUSEVERTEX>(  l_pRenderManager,
                                                                        l_pVertexBuffer,
                                                                        l_pIndexList,
                                                                        l_VertexCount, 
                                                                        l_IndexCount);
    } else if(l_pusVertexType[i] == TNORMALTANGENTBINORMALTEXTUREDVERTEX::GetVertexType())
    {
      CalcTangentsAndBinormals( l_pVertexBuffer, l_pIndexList, l_VertexCount,
                                l_IndexCount, sizeof(TNORMALTANGENTBINORMALTEXTUREDVERTEX), 0, sizeof(float)*3,
                                sizeof(float)*7, sizeof(float)*11, sizeof(float)*15);

      l_RenderableVertexs = new CIndexedVertexs<TNORMALTANGENTBINORMALTEXTUREDVERTEX>(  l_pRenderManager,
                                                                                        l_pVertexBuffer,
                                                                                        l_pIndexList,
                                                                                        l_VertexCount, 
                                                                                        l_IndexCount);
    } else if(l_pusVertexType[i] == TNORMALTANGENTBINORMALTEXTURED2VERTEX::GetVertexType())
    {
      CalcTangentsAndBinormals( l_pVertexBuffer, l_pIndexList, l_VertexCount,
                                l_IndexCount, sizeof(TNORMALTANGENTBINORMALTEXTURED2VERTEX), 0, sizeof(float)*3,
                                sizeof(float)*7, sizeof(float)*11, sizeof(float)*15);

      l_RenderableVertexs = new CIndexedVertexs<TNORMALTANGENTBINORMALTEXTURED2VERTEX>( l_pRenderManager,
                                                                                        l_pVertexBuffer,
                                                                                        l_pIndexList,
                                                                                        l_VertexCount, 
                                                                                        l_IndexCount);
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "CStaticMesh::Load unrecognized vertex type %#hx", l_pusVertexType[i]);
      CHECKED_DELETE_ARRAY(l_pusVertexType);
      CHECKED_DELETE_ARRAY(l_pusTextureNum);
      l_File.close();
      delete l_pVertexBuffer;
      delete l_pIndexList;
      return false;
    }

    m_iNumVertexs += l_VertexCount;
    m_iNumFaces   += l_IndexCount / 3;
    
    m_RVs.push_back(l_RenderableVertexs);
  
    delete l_pVertexBuffer;
    delete l_pIndexList;
  }


  l_File.read((char*)&l_usHelper, sizeof(uint16));

  if(l_usHelper != FOOTER)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CStaticMesh::Load reading file with incorrect footer");
    CHECKED_DELETE_ARRAY(l_pusVertexType);
    CHECKED_DELETE_ARRAY(l_pusTextureNum);
    l_File.close();
    return false;
  }
  
  l_File.close();
  m_szFileName = _szFileName;
  CHECKED_DELETE_ARRAY(l_pusVertexType);
  CHECKED_DELETE_ARRAY(l_pusTextureNum);

  SetOk(true);
  return IsOk();
}

void CStaticMesh::Render(CRenderManager *_pRM) const
{
    vector<CRenderableVertexs*>::const_iterator l_ItRV = m_RVs.begin();
    vector<CRenderableVertexs*>::const_iterator l_EndRV = m_RVs.end();

    vector<vector<CTexture*>>::const_iterator l_ItTextureArray = m_Textures.begin();

    while(l_ItRV != l_EndRV) 
    {
      /*if(l_ItTextureArray != m_Textures.end())
      {
        vector<CTexture*>::const_iterator l_ItTexture = (*l_ItTextureArray).begin();
        vector<CTexture*>::const_iterator l_EndTexture = (*l_ItTextureArray).end();
        int stage = 0;
        while(l_ItTexture != l_EndTexture)
        {
          (*l_ItTexture)->Activate(stage);
          ++l_ItTexture;
          ++stage;
        }
        ++l_ItTextureArray;
      }*/
      (*l_ItRV)->ActivateTextures(*l_ItTextureArray);

      //(*l_ItRV)->Render(_pRM);  // Fixed Pipeline render

      //---------------------------- shaders -----------------------
      CEffectManager* l_pEffectManager = _pRM->GetEffectManager();
      uint16 l_iVertexType = (*l_ItRV)->GetVertexType();
      string l_szName = l_pEffectManager->GetTechniqueEffectNameByVertexDefault(l_iVertexType);
      CEffectTechnique* l_pEffectTechnique = l_pEffectManager->GetEffectTechnique(l_szName);
      if(l_pEffectTechnique)
      {
        if(l_pEffectTechnique->BeginRender())
        {
          (*l_ItRV)->Render(_pRM,l_pEffectTechnique);
        } else {
          (*l_ItRV)->Render(_pRM);
        }
      } else {
        (*l_ItRV)->Render(_pRM);
      }

      //------------------------------------------------------------

      ++l_ItRV;
    }
}

void CStaticMesh::Release()
{
    vector<CRenderableVertexs*>::iterator l_It = m_RVs.begin();
    vector<CRenderableVertexs*>::iterator l_End = m_RVs.end();

    while(l_It != l_End) 
    {
      CHECKED_DELETE(*l_It)
      ++l_It;
    }

    //el texture manager ja s'encarregarà de petar les textures
    m_Textures.clear();

    m_RVs.clear();
}
