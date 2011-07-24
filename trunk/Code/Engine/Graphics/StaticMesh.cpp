#include "Core.h"
#include "RenderManager.h"
#include "StaticMesh.h"
#include "VertexsStructs.h"
#include "Texture.h"
#include "TextureManager.h"
#include "EffectManager.h"

#include "Material.h"
#include "StaticMeshEmptyMaterial.h"
#include "DiffuseTextureDecorator.h"
#include "NormalTextureDecorator.h"
#include "LightmapTextureDecorator.h"
#include "SpecularTextureDecorator.h"
#include "GlowTextureDecorator.h"
#include "ParallaxPropertyDecorator.h"
#include "BumpPropertyDecorator.h"
#include "SpecularPropertyDecorator.h"
#include "AlphaDecorator.h"
#include "GlowPropertyDecorator.h"
#include "EnvironmentTextureDecorator.h"
#include "EnvirondmentPropertyDecorator.h"

#include <IndexedVertexs.h>
#include <base.h>
#include <fstream>

#include "VertexCalculations.h"

#define HEADER 0xAAAA
#define FOOTER 0xFFFF

bool CStaticMesh::Load(const CXMLTreeNode& _XMLTreeNode)
{
  /** Format fitxer
    *
    *  unsigned short              -> HEADER(MASK = 0xAAAA)
    *  unsigned short              -> MaterialCount. nombre de materials
    *
    *  for MaterialCount
    *  	unsigned short            -> VERTEX_TYPE.
    *   unsigned short            -> nombre de textures a llegir
    *   for MaterialTextures
    *     unsigned short            -> TEXTURE_TYPE.
    *     unsigned short            -> Longitud de la cadena.
    *     char[]                    -> string: path.
    *   unsigned short            -> nombre de propietats a llegir
    *   for MaterialProperty
    *     unsigned short            -> PROPERTY_TYPE.
    *     float[]                   -> values
    *
    *  for MaterialCount
    *  	unsigned short            -> VertexCount. Nombre de vèrtexs.
    *   VERTEX_STRUCT[]           -> Vèrtexs. Llista de vèrtexs. Format depenent de VERTEX_TYPE.
    *  	unsigned short            -> IndexCount. Nombre d'índexs.
    *   unsigned short[]          -> Índexs. Llista d'índexs de vèrtexs.
    *  
    *  unsigned short             -> FOOTER(MASK = 0xFFFF)
    *
    * Fi Format fitxer
   **/
  string l_szFileName = _XMLTreeNode.GetPszISOProperty("path","");
  m_vMax              = _XMLTreeNode.GetVect3fProperty("max",Vect3f(0.0f));
  m_vMin              = _XMLTreeNode.GetVect3fProperty("min",Vect3f(0.0f));

  m_szFileName = l_szFileName;

  SetOk(Load());
  return IsOk();
}

bool CStaticMesh::Load()
{
  bool l_bRecalc      = false;

  Vect3f l_vMax, l_vMin = Vect3f(0.0f);

  LOGGER->AddNewLog(ELL_INFORMATION, "CStaticMesh::Load \"%s\"", m_szFileName.c_str());
  if(m_vMax == Vect3f(0.0f) && m_vMin == Vect3f(0.0f))
  {
    l_bRecalc = true;
    LOGGER->AddNewLog(ELL_INFORMATION, "CStaticMesh::Load calculate min & max");
  }
  
  fstream l_File;
  uint16 l_usHelper = 0;
  uint16 l_usNumMaterials = 0;
  uint16* l_pusVertexType = 0;
  uint16* l_pusTextureNum = 0;

  CRenderManager* l_pRenderManager = CORE->GetRenderManager();

  CTextureManager* l_pTextureManager = CORE->GetTextureManager();
  
  l_File.open(m_szFileName, fstream::in | fstream::binary );
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

  /*for(int i = 0; i < l_usNumMaterials; i++)
  {
    m_vMaterials.push_back(new CEffectMaterial());
    m_vMaterials[i]->Init(l_File);
    l_pusVertexType[i] = m_vMaterials[i]->GetVertexType();

  }*/

  uint16 l_usNumTextures = 0;
  uint16 l_usNumProperties = 0;
  uint16 l_usTextureType = 0;
  uint16 l_usPropertyType = 0;
  uint16 l_usStrLength = 0;
  float l_fValue = 0.0f;

  CTexture* l_pTexture = 0;
  
  for(int j = 0; j < l_usNumMaterials; ++j)
  {
    l_File.read((char*)&l_pusVertexType[j], sizeof(uint16));
    l_File.read((char*)&l_usNumTextures, sizeof(uint16));

    CMaterial* l_pMaterial = new CStaticMeshEmptyMaterial();

    for(int i = 0; i < l_usNumTextures; ++i)
    {
      l_File.read((char*)&l_usTextureType, sizeof(uint16));
      l_File.read((char*)&l_usStrLength, sizeof(uint16));
      char* l_szPath = new char[++l_usStrLength];
      memset(l_szPath,0,sizeof(char)*(l_usStrLength));
      l_File.read(l_szPath, sizeof(char)*l_usStrLength);

      if(l_usTextureType == ENVIRONMENT_MATERIAL_MASK)
      {
        l_pTexture = l_pTextureManager->GetCubeTexture(l_szPath);
      }
      else
      {
        l_pTexture = l_pTextureManager->GetResource(l_szPath);
      }

      delete[] l_szPath;

      switch(l_usTextureType)
      {
        case DIFFUSE_MATERIAL_MASK:
          l_pMaterial = new CDiffuseTextureDecorator(l_pMaterial,l_pTexture);
          break;
        case NORMALMAP_MATERIAL_MASK:
          l_pMaterial = new CNormalTextureDecorator(l_pMaterial,l_pTexture);
          break;
        case LIGHTMAP_MATERIAL_MASK:
          l_pMaterial = new CLightmapTextureDecorator(l_pMaterial,l_pTexture);        
          break;
        case SPECULARMAP_MATERIAL_MASK:
          l_pMaterial = new CSpecularTextureDecorator(l_pMaterial,l_pTexture);
          break;
        case GLOW_MATERIAL_MASK:
          l_pMaterial = new CGlowTextureDecorator(l_pMaterial,l_pTexture);
          break;
        case ENVIRONMENT_MATERIAL_MASK:
          l_pMaterial = new CEnvironmentTextureDecorator(l_pMaterial,l_pTexture);
          break;
        default:
          break;
      }
    }

    l_File.read((char*)&l_usNumProperties, sizeof(uint16));

    for(int i = 0; i < l_usNumProperties; ++i)
    {
      l_File.read((char*)&l_usPropertyType, sizeof(uint16));
      float l_fGlossiness, l_fSpecularLevel;
      switch(l_usPropertyType)
      {
        case PARALLAX_PROPERTY_TYPE:
          l_File.read((char*)&l_fValue, sizeof(float));
          l_pMaterial = new CParallaxPropertyDecorator(l_pMaterial,l_fValue);
          break;
        case BUMP_PROPERTY_TYPE:
          l_File.read((char*)&l_fValue, sizeof(float));
          l_pMaterial = new CBumpPropertyDecorator(l_pMaterial,l_fValue);
          break;
        case SPECULAR_PROPERTY_TYPE:
          l_File.read((char*)&l_fGlossiness, sizeof(float));
          l_File.read((char*)&l_fSpecularLevel, sizeof(float));
          l_pMaterial = new CSpecularPropertyDecorator(l_pMaterial,l_fGlossiness,l_fSpecularLevel);
          break;
        case ALPHA_PROPERTY_TYPE:
          l_pMaterial = new CAlphaPropertyDecorator( l_pMaterial );
          break;
        case GLOW_PROPERTY_TYPE:
          l_File.read((char*)&l_fValue, sizeof(float));
          l_pMaterial = new CGlowPropertyDecorator(l_pMaterial,l_fValue);
          break;
        case RADIOSITY_NORMAL_PROPERTY_TYPE:
          l_pMaterial->ActivateRadiosityNormal(CMaterial::RADIOSITY_NORMAL);
          break;
        case ENVIRONMENT_PROPERTY_TYPE:
          l_File.read((char*)&l_fValue, sizeof(float));
          l_pMaterial = new CEnvirondmentPropertyDecorator(l_pMaterial,l_fValue);
        default:
          break;
      }
    }

    m_vMaterials.push_back(l_pMaterial);
  }

  m_vIndexBuffer.clear();
  m_vVertexBuffer.clear();

  int l_i = 0;
  // ----------------------------- BUFFERS ----------------------------------------
  for(int i = 0; i < l_usNumMaterials; i++)
  {

    uint16 l_VertexCount;
    l_File.read((char*)&l_VertexCount, sizeof(uint16));

   /* if (l_VertexCount <= 0)
    {*/

    /*m_vMaterials.push_back(new CEffectMaterial());
    m_vMaterials[l_i]->Init(l_File);
    l_pusVertexType[l_i] = m_vMaterials[l_i]->GetVertexType();*/
    
    uint16 l_usVertexSize = GetVertexSize(l_pusVertexType[l_i]);
  
   

    char* l_pVertexBuffer = new char[l_VertexCount*l_usVertexSize];
    l_File.read(&l_pVertexBuffer[0], l_usVertexSize*l_VertexCount);

    //Recalc min max per cada material
    if(l_bRecalc)
    {
      CalcMinMaxCoord(l_pVertexBuffer, 0, l_usVertexSize, l_VertexCount, l_vMin, l_vMax);

      m_vMin.SetIfMinComponents(l_vMin);
      m_vMax.SetIfMaxComponents(l_vMax);
    }

    
  
    uint32 l_IndexCount;

    l_File.read((char*)&l_IndexCount, sizeof(uint32));

    /*TNORMALTANGENTBINORMALTEXTURED2VERTEX aux[24], *aux2 = (TNORMALTANGENTBINORMALTEXTURED2VERTEX*)l_pVertexBuffer;
    for(int k = 0; k < 24; k++)
    {
      aux[k] = aux2[k];
    }*/

    uint16 * l_pIndexList = new uint16[l_IndexCount];
    l_File.read((char *)&l_pIndexList[0], sizeof(uint16)*l_IndexCount);

    
    //Optimitzar l'ordre
    VertexCacheOptimisation( l_pVertexBuffer, l_pIndexList, l_VertexCount, l_IndexCount, l_usVertexSize );

    //CIndexedVertexs<SNORMALTEXTUREDVERTEX> *l_IndexedVertexs=new CIndexedVertexs<SNORMALTEXTUREDVERTEX>(RENDER_MANAGER, (SNORMALTEXTUREDVERTEX*)l_pVertexBuffer, l_pIndexList, l_VertexCount, l_IndexCount);
    CRenderableVertexs* l_RenderableVertexs = 0;

    //CARREGUEM LES VERSIONS SIMPLES DELS VB I IB PER LA FISICA
    vector<uint32> l_vIndexBuffer = CalcSimpleIndexBuffer(l_pIndexList,sizeof(uint32),l_IndexCount);
    vector<Vect3f> l_vVertexBuffer = CalcSimpleVertexBuffer(l_pVertexBuffer,l_usVertexSize,l_VertexCount);

    int l_vOldVertexNum = m_vVertexBuffer.size();
    for(uint32 k= 0; k < l_vIndexBuffer.size(); ++k)
    {
      m_vIndexBuffer.push_back(l_vIndexBuffer[k] + l_vOldVertexNum);
    }

    for(uint32 k= 0; k < l_vVertexBuffer.size(); ++k)
    {
      m_vVertexBuffer.push_back(l_vVertexBuffer[k]);
    }

    if(l_pusVertexType[l_i] == SNORMALTEXTUREDVERTEX::GetVertexType())
    {
      l_RenderableVertexs = new CIndexedVertexs<SNORMALTEXTUREDVERTEX>( l_pRenderManager,
                                                                        l_pVertexBuffer,
                                                                        l_pIndexList,
                                                                        l_VertexCount, 
                                                                        l_IndexCount);
    } else if(l_pusVertexType[l_i] == SNORMALTEXTURED2VERTEX::GetVertexType())
    {
      l_RenderableVertexs = new CIndexedVertexs<SNORMALTEXTURED2VERTEX>(  l_pRenderManager,
                                                                          l_pVertexBuffer,
                                                                          l_pIndexList,
                                                                          l_VertexCount, 
                                                                          l_IndexCount);
    } else if(l_pusVertexType[l_i] == SDIFFUSEVERTEX::GetVertexType())
    {
      l_RenderableVertexs = new CIndexedVertexs<SDIFFUSEVERTEX>(  l_pRenderManager,
                                                                  l_pVertexBuffer,
                                                                  l_pIndexList,
                                                                  l_VertexCount, 
                                                                  l_IndexCount);
    } else if(l_pusVertexType[l_i] == STEXTUREDVERTEX::GetVertexType())
    {
      l_RenderableVertexs = new CIndexedVertexs<STEXTUREDVERTEX>( l_pRenderManager,
                                                                        l_pVertexBuffer,
                                                                        l_pIndexList,
                                                                        l_VertexCount, 
                                                                        l_IndexCount);
    } else if(l_pusVertexType[l_i] == STEXTURED2VERTEX::GetVertexType())
    {
      l_RenderableVertexs = new CIndexedVertexs<STEXTURED2VERTEX>(  l_pRenderManager,
                                                                    l_pVertexBuffer,
                                                                    l_pIndexList,
                                                                    l_VertexCount, 
                                                                    l_IndexCount);
    } else if(l_pusVertexType[l_i] == SNORMALDIFFUSEVERTEX::GetVertexType())
    {
      l_RenderableVertexs = new CIndexedVertexs<SNORMALDIFFUSEVERTEX>(  l_pRenderManager,
                                                                        l_pVertexBuffer,
                                                                        l_pIndexList,
                                                                        l_VertexCount, 
                                                                        l_IndexCount);
    } else if(l_pusVertexType[l_i] == TNORMALTANGENTBINORMALTEXTUREDVERTEX::GetVertexType())
    {
      CalcTangentsAndBinormalsBis( l_pVertexBuffer, l_pIndexList, l_VertexCount,
                                l_IndexCount, sizeof(TNORMALTANGENTBINORMALTEXTUREDVERTEX), 0, sizeof(float)*3,
                                sizeof(float)*7, sizeof(float)*11, sizeof(float)*15);

      l_RenderableVertexs = new CIndexedVertexs<TNORMALTANGENTBINORMALTEXTUREDVERTEX>(  l_pRenderManager,
                                                                                        l_pVertexBuffer,
                                                                                        l_pIndexList,
                                                                                        l_VertexCount, 
                                                                                        l_IndexCount);
    } else if(l_pusVertexType[l_i] == TNORMALTANGENTBINORMALTEXTURED2VERTEX::GetVertexType())
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

    l_i++;
    
    
    //FI FOR
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
  CHECKED_DELETE_ARRAY(l_pusVertexType);
  CHECKED_DELETE_ARRAY(l_pusTextureNum);

  return true;
}

bool CStaticMesh::ReloadTextures()
{
  vector<CMaterial*>::const_iterator l_it  = m_vMaterials.cbegin();
  vector<CMaterial*>::const_iterator l_end = m_vMaterials.cend();

  bool l_bIsOk = true;

  for(; l_it != l_end; ++l_it)
  {
    if(!(*l_it)->ReloadTextures() )
    {
      l_bIsOk = false;
    }
  }

  return l_bIsOk;
}

void CStaticMesh::Render(CRenderManager *_pRM, bool _bInstanced) const
{
    vector<CRenderableVertexs*>::const_iterator l_ItRV = m_RVs.begin();
    vector<CRenderableVertexs*>::const_iterator l_EndRV = m_RVs.end();

    vector<CMaterial*>::const_iterator l_ItMaterialArray = m_vMaterials.begin();

    CEffectManager* m_pEM = CORE->GetEffectManager();

    while(l_ItRV != l_EndRV) 
    {
      CEffect* l_pEffect = m_pEM->ActivateMaterial(*l_ItMaterialArray);

      (*l_ItRV)->Render(_pRM,l_pEffect);

      ++l_ItRV;
      ++l_ItMaterialArray;
    }
}

bool CStaticMesh::IsAlphaBlended() const
{
  vector<CMaterial*>::const_iterator l_it  = m_vMaterials.cbegin();
  vector<CMaterial*>::const_iterator l_end = m_vMaterials.cend();

  for(; l_it != l_end; ++l_it)
  {
    if( (*l_it)->HasAlphaBlending() )
    {
      return true;
    }
  }
  return false;
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
  /*{
    vector<CEffectMaterial*>::iterator l_It = m_vMaterials.begin();
    vector<CEffectMaterial*>::iterator l_End = m_vMaterials.end();

    while(l_It != l_End) 
    {
      CHECKED_DELETE(*l_It)
      ++l_It;
    }
  }*/

  //m_vMaterials.clear();

  {
    vector<CMaterial*>::iterator l_It = m_vMaterials.begin();
    vector<CMaterial*>::iterator l_End = m_vMaterials.end();

    while(l_It != l_End) 
    {
      CHECKED_DELETE(*l_It)
      ++l_It;
    }
  }

  m_vMaterials.clear();

  m_RVs.clear();
}
