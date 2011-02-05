#include "Core.h"
#include "RenderManager.h"
#include "StaticMesh.h"
#include "VertexsStructs.h"
#include "Texture.h"
#include "TextureManager.h"
#include "EffectManager.h"
#include "EffectTechnique.h"
#include "EffectMaterial.h"
#include <IndexedVertexs.h>
#include <base.h>
#include <fstream>

#include "VertexCalculations.h"

#define HEADER 0xAAAA
#define FOOTER 0xFFFF

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
  //l_pusTextureNum = new uint16[l_usNumMaterials];

  m_vMaterials.clear();
  for(int i = 0; i < l_usNumMaterials; i++)
  {
    m_vMaterials.push_back(new CEffectMaterial());
    m_vMaterials[i]->Init(l_File);
    l_pusVertexType[i] = m_vMaterials[i]->GetVertexType();

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
      CalcTangentsAndBinormalsBis( l_pVertexBuffer, l_pIndexList, l_VertexCount,
                                l_IndexCount, sizeof(TNORMALTANGENTBINORMALTEXTUREDVERTEX), 0, sizeof(float)*3,
                                sizeof(float)*7, sizeof(float)*11, sizeof(float)*15);

      l_RenderableVertexs = new CIndexedVertexs<TNORMALTANGENTBINORMALTEXTUREDVERTEX>(  l_pRenderManager,
                                                                                        l_pVertexBuffer,
                                                                                        l_pIndexList,
                                                                                        l_VertexCount, 
                                                                                        l_IndexCount);
    } else if(l_pusVertexType[i] == TNORMALTANGENTBINORMALTEXTURED2VERTEX::GetVertexType())
    {
      CalcTangentsAndBinormalsBis( l_pVertexBuffer, l_pIndexList, l_VertexCount,
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

    vector<CEffectMaterial*>::const_iterator l_ItMaterialArray = m_vMaterials.begin();

    while(l_ItRV != l_EndRV) 
    {

      //---------------------------- shaders -----------------------
      
      CEffectTechnique* l_pEffectTechnique = (*l_ItMaterialArray)->GetEffectTechnique(_pRM);

      if(l_pEffectTechnique)
      {
        if(l_pEffectTechnique->BeginRender(*l_ItMaterialArray))
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
      ++l_ItMaterialArray;
    }
}

void CStaticMesh::Release()
{
  {
    vector<CRenderableVertexs*>::iterator l_It = m_RVs.begin();
    vector<CRenderableVertexs*>::iterator l_End = m_RVs.end();

    while(l_It != l_End) 
    {
      CHECKED_DELETE(*l_It)
      ++l_It;
    }
  }
  {
    vector<CEffectMaterial*>::iterator l_It = m_vMaterials.begin();
    vector<CEffectMaterial*>::iterator l_End = m_vMaterials.end();

    while(l_It != l_End) 
    {
      CHECKED_DELETE(*l_It)
      ++l_It;
    }
  }

  m_vMaterials.clear();

  m_RVs.clear();
}
