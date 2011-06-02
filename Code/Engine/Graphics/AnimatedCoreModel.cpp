#include "AnimatedCoreModel.h"
#include "VertexsStructs.h"
#include "IndexedVertexs.h"
#include "VertexCalculations.h"
#include "TextureManager.h"

#include "Material.h"
#include "AnimatedMeshEmptyMaterial.h"
#include "DiffuseTextureDecorator.h"
#include "NormalTextureDecorator.h"
#include "LightmapTextureDecorator.h"
#include "SpecularTextureDecorator.h"
#include "GlowTextureDecorator.h"
#include "ParallaxPropertyDecorator.h"
#include "BumpPropertyDecorator.h"
#include "GlowPropertyDecorator.h"

#include <cal3d/cal3d.h>
#include <XML/XMLTreeNode.h>


void CAnimatedCoreModel::Release()
{
  CHECKED_DELETE(m_pRenderableVertexs);
  CHECKED_DELETE(m_pCalHardwareModel);
  CHECKED_DELETE(m_pCalCoreModel);
  m_szMeshFilename      = "";
  m_szSkeletonFilename  = "";
  
  vector<CMaterial*>::iterator l_it  = m_vMaterials.begin();
  vector<CMaterial*>::iterator l_end = m_vMaterials.end();
  while(l_it != l_end)
  {
    CHECKED_DELETE(*l_it);
    ++l_it;
  }
  m_vMaterials.clear();
}

bool CAnimatedCoreModel::Load(const std::string &_szPath)
{
  m_szPath = _szPath;
  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::Load Carregant el Animated Model \"%s\"", _szPath.c_str());

  SetOk(false);

  CXMLTreeNode l_treeActor;
  if(l_treeActor.LoadFile(_szPath.c_str()))
  {
    const char* l_pcAux = l_treeActor.GetPszProperty("name",0);

    if(l_pcAux != 0)
    {
      SetName(string(l_pcAux));
    } else {
      LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load No s'ha trobat la propietat \"name\"");
      return IsOk();
    }

    m_pCalCoreModel = new CalCoreModel(GetName().c_str());

    int l_iNumChildren = l_treeActor.GetNumChildren();

    for(int i = 0; i < l_iNumChildren; i++)
    {
      CXMLTreeNode l_treeChild = l_treeActor(i);
      if(strcmp(l_treeChild.GetName(),"skeleton") == 0)
      {
        if(m_szSkeletonFilename != "")
        {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load Esquelet repetit!");
          Release();
          return IsOk();
        }

        const char* l_pcFileName = l_treeChild.GetPszProperty("filename",0);

        if(l_pcFileName != 0)
        {
          m_szSkeletonFilename = l_pcFileName;
        } else {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load No s'ha trobat la propietat \"filename\" a l'esquelet.");
          Release();
          return IsOk();
        }

        if(!LoadSkeleton())
        {
          Release();
          return IsOk();
        }

      } else if(strcmp(l_treeChild.GetName(),"mesh") == 0)
      {
        if(m_szMeshFilename != "")
        {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load Mesh repetit!");
          Release();
          return IsOk();
        }

        const char* l_pcFileName = l_treeChild.GetPszProperty("filename",0);

        if(l_pcFileName != 0)
        {
          m_szMeshFilename = l_pcFileName;
        } else {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load No s'ha trobat la propietat \"filename\" al mesh.");
          Release();
          return IsOk();
        }

        if(!LoadMesh())
        {
          Release();
          return IsOk();
        }

      } else if(strcmp(l_treeChild.GetName(),"material") == 0)
      {
        CTextureManager* l_pTextureManager = CORE->GetTextureManager();
        CTexture* l_pTexture = 0;

        CMaterial* l_pMaterial = new CAnimatedMeshEmptyMaterial();

        int l_iNumChildren = l_treeChild.GetNumChildren();

        for(int i = 0; i < l_iNumChildren; i++)
        {
          if(strcmp(l_treeChild(i).GetName(),"texture") == 0)
          {
            CXMLTreeNode l_xmlTexture = l_treeChild(i);

            string l_szTexType = l_xmlTexture.GetPszISOProperty("type","");
            string l_szTexFilename = l_xmlTexture.GetPszISOProperty("filename", "");

            l_pTexture = l_pTextureManager->GetResource(l_szTexFilename);

            if(l_szTexType == "diffusse")
            {
              l_pMaterial = new CDiffuseTextureDecorator(l_pMaterial,l_pTexture);
            } else if(l_szTexType == "normalmap")
            {
              l_pMaterial = new CNormalTextureDecorator(l_pMaterial,l_pTexture);
            } else if(l_szTexType == "lightmap")
            {
              l_pMaterial = new CLightmapTextureDecorator(l_pMaterial,l_pTexture);
            } else if(l_szTexType == "specular")
            {
              l_pMaterial = new CSpecularTextureDecorator(l_pMaterial,l_pTexture);
            } else if(l_szTexType == "glow")
            {
              l_pMaterial = new CGlowTextureDecorator(l_pMaterial,l_pTexture);
            }
          }

          if(strcmp(l_treeChild(i).GetName(),"property") == 0)
          {
            CXMLTreeNode l_xmlProperty = l_treeChild(i);

            string l_szTexType = l_xmlProperty.GetPszISOProperty("type","");
            float l_fValue = l_xmlProperty.GetFloatProperty("value", 0.0f);

            if(l_szTexType == "parallax")
            {
              l_pMaterial = new CParallaxPropertyDecorator(l_pMaterial,l_fValue);
            } else if(l_szTexType == "bump")
            {
              l_pMaterial = new CBumpPropertyDecorator(l_pMaterial,l_fValue);
            } else if(l_szTexType == "glowIntensity")
            {
              l_pMaterial = new CGlowPropertyDecorator(l_pMaterial,l_fValue);
            }
          }

          
        }

        m_vMaterials.push_back(l_pMaterial);

      } else if(strcmp(l_treeChild.GetName(),"animation") == 0)
      {
        const char* l_pcFileName = l_treeChild.GetPszProperty("filename",0);
        const char* l_pcName = l_treeChild.GetPszProperty("name",0);

        if(l_pcFileName == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load No s'ha trobat la propietat \"filename\" a una animació.");
        } else if(l_pcName == 0)
        {
          LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load No s'ha trobat la propietat \"name\" a una animació.");
        } else {
          LoadAnimation(l_pcName, l_pcFileName);
        }

      } else if (!l_treeChild.IsComment())
      {
        LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load S'ha trobat un element desconegut \"%s\"", l_treeChild.GetName());
      }
    }

    if(LoadVertexBuffer())
      SetOk(true);

  } else {
    LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::Load No s'ha trobat l'arxiu");
  }

  ComputeBoundings();

  return IsOk();
}


bool CAnimatedCoreModel::LoadMesh()
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::LoadMesh Carregant el Mesh \"%s\"", m_szMeshFilename.c_str());

  if(m_pCalCoreModel->loadCoreMesh(m_szMeshFilename.c_str()) < 0)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadMesh Error al Cal3D \"%s\"", CalError::getLastErrorText().c_str());
    return false;
  } else {
    return true;
  }
}

bool CAnimatedCoreModel::LoadSkeleton()
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::LoadSkeleton Carregant l'Esquelet \"%s\"", m_szSkeletonFilename.c_str());

  if(!m_pCalCoreModel->loadCoreSkeleton(m_szSkeletonFilename.c_str()))
  {
    LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadSkeleton Error al Cal3D \"%s\"", CalError::getLastErrorText().c_str());
    return false;
  } else {
    return true;
  }
}

bool CAnimatedCoreModel::LoadAnimation(const string& _szName, const std::string& _szFilename)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::LoadAnimation Carregant l'animació \"%s\"", _szFilename.c_str());

  if(m_pCalCoreModel->loadCoreAnimation(_szFilename,_szName) < 0)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CAnimatedCoreModel::LoadAnimation Error al Cal3D \"%s\"", CalError::getLastErrorText().c_str());
    return false;
  } else {
    return true;
  }
}

//TODO: MOLT DEBUG
bool CAnimatedCoreModel::LoadVertexBuffer()
{
  /*
  LPDIRECT3DDEVICE9 l_pDevice = _pRM->GetDevice();
  if(FAILED(l_pDevice->CreateVertexBuffer(sizeof(SNORMALTEXTUREDVERTEX)*m_iNumVtxs,D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC,SNORMALTEXTUREDVERTEX::GetVertexType(),D3DPOOL_DEFAULT,&m_pVB,NULL)))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CAnimatedInstanceModel::LoadVertexBuffer CreateVertexBuffer ha fallat");
    isOk = false;
  }

  _D3DFORMAT l_IndexFormat = (sizeof(CalIndex) == 2)? D3DFMT_INDEX16 : D3DFMT_INDEX32;

  if(FAILED(l_pDevice->CreateIndexBuffer(sizeof(CalIndex)*m_iNumFaces*3,0,l_IndexFormat,D3DPOOL_DEFAULT,&m_pIB,NULL)))
  {
    LOGGER->AddNewLog(ELL_WARNING,"CAnimatedInstanceModel::LoadVertexBuffer CreateIndexBuffer ha fallat");
    isOk = false;
  }
  */

  bool l_bIsOk = false;

  LOGGER->AddNewLog(ELL_INFORMATION,"CAnimatedCoreModel::LoadVertexBuffer  Loading \"%s\".", GetName().c_str());

  int l_iMeshCount = m_pCalCoreModel->getCoreMeshCount();
  for(int l_iMeshId = 0; l_iMeshId < l_iMeshCount; l_iMeshId++)
  {
    LOGGER->AddNewLog(ELL_INFORMATION,"CAnimatedCoreModel::LoadVertexBuffer  Comptant vèrtexs i índexs de la mesh %d.", l_iMeshId);
    CalCoreMesh* l_pCoreMesh = m_pCalCoreModel->getCoreMesh(l_iMeshId);

    int l_iSubmeshCount = l_pCoreMesh->getCoreSubmeshCount();
    for(int l_iSubMeshId = 0; l_iSubMeshId < l_iSubmeshCount; l_iSubMeshId++)
    {
      m_iNumVtxs += l_pCoreMesh->getCoreSubmesh(l_iSubMeshId)->getVertexCount();
      m_iNumFaces += l_pCoreMesh->getCoreSubmesh(l_iSubMeshId)->getFaceCount();
    }
  }

  //TCAL3D_HW_VERTEX* l_pVertex;
  //if(m_pCalHardwareModel)
    CHECKED_DELETE(m_pCalHardwareModel);

  m_pCalHardwareModel = new CalHardwareModel(m_pCalCoreModel);

  TCAL3D_HW_VERTEX* l_pVtxs = new TCAL3D_HW_VERTEX[m_iNumVtxs*2];
  unsigned short* l_pIdxs = new unsigned short[m_iNumFaces*3];

  //donde se encuentra el vertex buffer
  m_pCalHardwareModel->setVertexBuffer((char*) l_pVtxs, sizeof(TCAL3D_HW_VERTEX));
  //donde se encuentra el weight buffer
  m_pCalHardwareModel->setWeightBuffer(((char*)l_pVtxs) + 12, sizeof(TCAL3D_HW_VERTEX));
  //donde se encuentra el índice de las matrices
  m_pCalHardwareModel->setMatrixIndexBuffer(((char*)l_pVtxs) + 28, sizeof(TCAL3D_HW_VERTEX));
  //donde se encuentra la estructura de normales
  m_pCalHardwareModel->setNormalBuffer(((char*)l_pVtxs) + 44, sizeof(TCAL3D_HW_VERTEX));
  //numero de texturas del vértice
  m_pCalHardwareModel->setTextureCoordNum(1);
  //donde se encuentran las coordenadas de textura
  m_pCalHardwareModel->setTextureCoordBuffer(0,((char*)l_pVtxs) + 56,sizeof(TCAL3D_HW_VERTEX));
  //donde copiar los índices    
  m_pCalHardwareModel->setIndexBuffer((CalIndex *)&l_pIdxs[0]);
  m_pCalHardwareModel->load( 0, 0, MAXBONES);

  m_iNumVtxs=m_pCalHardwareModel->getTotalVertexCount();
  
  CalcTangentsAndBinormalsBis( l_pVtxs, l_pIdxs,             //punters a vertex/index
                                m_iNumVtxs, m_iNumFaces*3,   //num vertex/index
                                sizeof(TCAL3D_HW_VERTEX),    //vertex stride
                                0,                           //geometria
                                sizeof(float)*11,            //normals
                                sizeof(float)*16,            //tangents
                                sizeof(float)*19,            //bitangents
                                sizeof(float)*14);           //coordenades de textura

  //CalcTangentsAndBinormals(l_pVtxs, l_pIdxs, m_iNumVtxs, m_iNumFaces*3, sizeof(TCAL3D_HW_VERTEX),0, 44, 60, 76, 92);

  if(m_pRenderableVertexs)
    CHECKED_DELETE(m_pRenderableVertexs);

  m_pRenderableVertexs = new CIndexedVertexs<TCAL3D_HW_VERTEX>(RENDER_MANAGER, l_pVtxs, l_pIdxs, m_iNumVtxs, m_iNumFaces*3);

  delete []l_pVtxs;
  delete []l_pIdxs;

  if(m_pCalHardwareModel && m_pRenderableVertexs)
    l_bIsOk = true;

  return l_bIsOk;
}

int CAnimatedCoreModel::GetAnimationCount()
{
  return m_pCalCoreModel->getCoreAnimationCount();
}


void CAnimatedCoreModel::ComputeBoundings()
{
  Vect3f l_vMin, l_vMax;
  bool l_bInit = false;

  for(int i = 0; i < m_pCalCoreModel->getCoreMeshCount(); ++i)
  {
    CalCoreMesh* l_pMesh = m_pCalCoreModel->getCoreMesh(i);
    for(int j = 0; j < l_pMesh->getCoreSubmeshCount(); ++j)
    {
      CalCoreSubmesh* l_pSubMesh = l_pMesh->getCoreSubmesh(j);

      const vector<CalCoreSubmesh::Vertex> l_VertexVector  = l_pSubMesh->getVectorVertex();
      vector<CalCoreSubmesh::Vertex>::const_iterator l_it  = l_VertexVector.cbegin();
      vector<CalCoreSubmesh::Vertex>::const_iterator l_end = l_VertexVector.cend();
      for(; l_it != l_end; ++l_it)
      {
        const CalCoreSubmesh::Vertex v = *l_it;
        if(l_bInit)
        {
          l_vMin.x = (l_vMin.x < v.position.x) ? l_vMin.x : v.position.x;
          l_vMin.y = (l_vMin.y < v.position.y) ? l_vMin.y : v.position.y;
          l_vMin.z = (l_vMin.z < v.position.z) ? l_vMin.z : v.position.z;
          
          l_vMax.x = (l_vMax.x > v.position.x) ? l_vMax.x : v.position.x;
          l_vMax.y = (l_vMax.y > v.position.y) ? l_vMax.y : v.position.y;
          l_vMax.z = (l_vMax.z > v.position.z) ? l_vMax.z : v.position.z;
        }
        else
        {
          l_vMin.x = v.position.x;
          l_vMin.y = v.position.y;
          l_vMin.z = v.position.z;
          l_vMax = l_vMin;
          l_bInit = true;
        }
      }
    }
  }

  l_vMin.x = -l_vMin.x;
  l_vMax.x = -l_vMax.x;

  Mat44f l_Transform;
  l_Transform.SetIdentity();
  l_Transform.RotByAngleX(-FLOAT_PI_VALUE*0.5f);

  m_BoundingBox.Init(l_Transform*l_vMin, l_Transform*l_vMax);
  m_BoundingSphere.Init(l_Transform*l_vMin, l_Transform*l_vMax);
}