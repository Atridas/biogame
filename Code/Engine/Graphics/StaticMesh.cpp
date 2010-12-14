#include "Core.h"
#include "RenderManager.h"
#include "StaticMesh.h"
#include "VertexsStructs.h"
#include "Texture.h"
#include "TextureManager.h"
#include <IndexedVertexs.h>
#include <base.h>
#include <fstream>

#define HEADER 0xAAAA
#define FOOTER 0xFFFF

bool CStaticMesh::LoadSergi(const string &_szFileName)
{
  unsigned short l_header = 0;
  unsigned short l_footer = 0;
  unsigned short l_materialCount = 0;
  unsigned short l_propertiesCount = 0;
  unsigned short l_pathCount = 0;
  char* l_pathString = 0;
  unsigned short* l_pVertexType;
  unsigned short l_vertexCount = 0;
  unsigned short l_indexCount = 0;

  char* l_vertex = 0;
  uint16* l_index = 0;

  ifstream  l_file;
  l_file.open(_szFileName,ios::binary);
  if(l_file.is_open())
  {
    m_szFileName=_szFileName;

    l_file.read((char*)&l_header,sizeof(unsigned short));
	  l_file.read((char*)&l_materialCount,sizeof(unsigned short));

	  l_pVertexType = new unsigned short[l_materialCount];

    vector<CTexture *> l_vTextures;

	  for(int i = 0; i < l_materialCount; i++)
	  {
      l_vTextures.clear();

		  l_file.read((char*)&(l_pVertexType[i]),sizeof(unsigned short));

		  l_file.read((char*)&l_propertiesCount,sizeof(unsigned short));

		  for(int j = 0; j < l_propertiesCount;j++)
		  {
			  l_file.read((char*)&l_pathCount,sizeof(unsigned short));
			  l_pathCount++;
			  l_pathString = new char[l_pathCount];
			  l_file.read(l_pathString,sizeof(char)*l_pathCount);

        l_vTextures.push_back(RENDER_MANAGER->GetTextureManager()->GetResource(string("D:/Max/").append(string(l_pathString))));

			  CHECKED_DELETE(l_pathString);
		  }
		
      m_Textures.push_back(l_vTextures);
	  }

	  for(int i = 0; i < l_materialCount; i++)
	  {

		  l_file.read((char*)&l_vertexCount,sizeof(unsigned short));

      int l_iSize = GetVertexSize(l_pVertexType[i]);
      l_vertex = new char[l_iSize*l_vertexCount];
      l_file.read(l_vertex,l_iSize*l_vertexCount);

      l_file.read((char*)&l_indexCount,sizeof(unsigned short));
		  l_index = new uint16[l_indexCount];
		  l_file.read((char*)l_index,sizeof(uint16)*l_indexCount);

      if(l_pVertexType[i] == STEXTUREDVERTEX::GetVertexType())
      {
        m_RVs.push_back(new CIndexedVertexs<STEXTUREDVERTEX>(RENDER_MANAGER, (STEXTUREDVERTEX*) l_vertex, l_index, l_vertexCount, l_indexCount));
      }

      if(l_pVertexType[i] == SDIFFUSEVERTEX::GetVertexType())
      {
        m_RVs.push_back(new CIndexedVertexs<SDIFFUSEVERTEX>(RENDER_MANAGER, (SDIFFUSEVERTEX*) l_vertex, l_index, l_vertexCount, l_indexCount));
      }

      if(l_pVertexType[i] == SNORMALDIFSSUSEVERTEX::GetVertexType())
      {
        m_RVs.push_back(new CIndexedVertexs<SNORMALDIFSSUSEVERTEX>(RENDER_MANAGER, (SNORMALDIFSSUSEVERTEX*) l_vertex, l_index, l_vertexCount, l_indexCount));
      }

      if(l_pVertexType[i] == SNORMALDIFFUSETEXTUREDVERTEX::GetVertexType())
      {
        m_RVs.push_back(new CIndexedVertexs<SNORMALDIFFUSETEXTUREDVERTEX>(RENDER_MANAGER, (SNORMALDIFFUSETEXTUREDVERTEX*) l_vertex, l_index, l_vertexCount, l_indexCount));
      }

      if(l_pVertexType[i] == SNORMALDIFFUSEDTEXTURED2VERTEX::GetVertexType())
      {
        m_RVs.push_back(new CIndexedVertexs<SNORMALDIFFUSEDTEXTURED2VERTEX>(RENDER_MANAGER, (SNORMALDIFFUSEDTEXTURED2VERTEX*) l_vertex, l_index, l_vertexCount, l_indexCount));
      }

      if(l_pVertexType[i] == STEXTUREDVERTEX::GetVertexType())
      {
        m_RVs.push_back(new CIndexedVertexs<STEXTUREDVERTEX>(RENDER_MANAGER, (STEXTUREDVERTEX*) l_vertex, l_index, l_vertexCount, l_indexCount));
      }

      if(l_pVertexType[i] == STEXTURED2VERTEX::GetVertexType())
      {
        m_RVs.push_back(new CIndexedVertexs<STEXTURED2VERTEX>(RENDER_MANAGER, (STEXTURED2VERTEX*) l_vertex, l_index, l_vertexCount, l_indexCount));
      }

      if(l_pVertexType[i] == SNORMALTEXTUREDVERTEX::GetVertexType())
      {
        m_RVs.push_back(new CIndexedVertexs<SNORMALTEXTUREDVERTEX>(RENDER_MANAGER, (SNORMALTEXTUREDVERTEX*) l_vertex, l_index, l_vertexCount, l_indexCount));
      }

      if(l_pVertexType[i] == SNORMALTEXTURED2VERTEX::GetVertexType())
      {
        m_RVs.push_back(new CIndexedVertexs<SNORMALTEXTURED2VERTEX>(RENDER_MANAGER, (SNORMALTEXTURED2VERTEX*) l_vertex, l_index, l_vertexCount, l_indexCount));
      }

	  }

    l_file.read((char*)&l_footer,sizeof(unsigned short));

    l_file.close();

    CHECKED_DELETE(l_vertex);
    CHECKED_DELETE(l_index);
	  CHECKED_DELETE(l_pVertexType);
  
    return l_footer==0xffff;
  }
  return false;
}

int CStaticMesh::GetVertexSize(unsigned short _uiVertexType)
{
  if(_uiVertexType == SDIFFUSEVERTEX::GetVertexType())
  {
    return sizeof(SDIFFUSEVERTEX);
  }

  if(_uiVertexType == SNORMALDIFSSUSEVERTEX::GetVertexType())
  {
    return sizeof(SNORMALDIFSSUSEVERTEX);
  }

  if(_uiVertexType == SNORMALDIFFUSETEXTUREDVERTEX::GetVertexType())
  {
    return sizeof(SNORMALDIFFUSETEXTUREDVERTEX);
  }

  if(_uiVertexType == SNORMALDIFFUSEDTEXTURED2VERTEX::GetVertexType())
  {
    return sizeof(SNORMALDIFFUSEDTEXTURED2VERTEX);
  }

  if(_uiVertexType == STEXTUREDVERTEX::GetVertexType())
  {
    return sizeof(STEXTUREDVERTEX);
  }

  if(_uiVertexType == STEXTURED2VERTEX::GetVertexType())
  {
    return sizeof(STEXTURED2VERTEX);
  }

  if(_uiVertexType == SNORMALTEXTUREDVERTEX::GetVertexType())
  {
    return sizeof(SNORMALTEXTUREDVERTEX);
  }

  if(_uiVertexType == SNORMALTEXTURED2VERTEX::GetVertexType())
  {
    return sizeof(SNORMALTEXTURED2VERTEX);
  }

  return 0;
}

bool CStaticMesh::Load(const string &_szFileName)
{
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
    l_File.read((char*)&(l_pusTextureNum[i]), sizeof(uint16));

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
  
  
    uint16 l_IndexCount;
  
    l_File.read((char*)&l_IndexCount, sizeof(uint16));

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
    }
    
    m_RVs.push_back(l_RenderableVertexs);
  
    delete l_pVertexBuffer;
    delete l_pIndexList;
  }


  l_File.read((char*)&l_usHelper, sizeof(uint16));

  if(l_usHelper != FOOTER)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CStaticMesh::Load reading file with incorrect header");
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
      if(l_ItTextureArray != m_Textures.end())
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
      }

      (*l_ItRV)->Render(_pRM);
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
