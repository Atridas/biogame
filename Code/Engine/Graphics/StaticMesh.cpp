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

  SNORMALTEXTUREDVERTEX* l_vertex = 0;
  uint16* l_index = 0;

  ifstream  l_file;
  l_file.open(_szFileName,ios::binary);
  if(l_file.is_open())
  {
    m_szFileName=_szFileName;

    l_file.read((char*)&l_header,sizeof(unsigned short));
	l_file.read((char*)&l_materialCount,sizeof(unsigned short));

	l_pVertexType = new unsigned short[l_materialCount];

	for(int i = 0; i < l_materialCount; i++)
	{
		l_file.read((char*)&(l_pVertexType[i]),sizeof(unsigned short));

		l_file.read((char*)&l_propertiesCount,sizeof(unsigned short));

		for(int j = 0; j < l_propertiesCount;j++)
		{
			l_file.read((char*)&l_pathCount,sizeof(unsigned short));
			
			l_pathCount++;
			
			l_pathString = new char[l_pathCount];

			l_file.read(l_pathString,sizeof(char)*l_pathCount);

			CHECKED_DELETE(l_pathString);
		}
		

	}

	for(int i = 0; i < l_materialCount; i++)
	{

		l_file.read((char*)&l_vertexCount,sizeof(unsigned short));

		l_vertex = new SNORMALTEXTUREDVERTEX[l_vertexCount];
  
		l_file.read((char*)l_vertex,sizeof(SNORMALTEXTUREDVERTEX)*l_vertexCount);

		l_file.read((char*)&l_indexCount,sizeof(unsigned short));

		l_index = new uint16[l_indexCount];

		l_file.read((char*)l_index,sizeof(uint16)*l_indexCount);

		m_RVs.push_back(new CIndexedVertexs<SNORMALTEXTUREDVERTEX>(RENDER_MANAGER, l_vertex, l_index, l_vertexCount, l_indexCount));
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


bool CStaticMesh::Load(const string &_szFileName)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CStaticMesh::Load \"%s\"", _szFileName.c_str());
  
  fstream l_File;
  uint16 l_usHelper = 0;
  uint16 l_usNumMaterials = 0;
  uint16* l_pusVertexType = 0;
  uint16* l_pusTextureNum = 0;

  CTextureManager* l_pTextureManager = CORE->GetRenderManager()->GetTextureManager();
  
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
    uint32 l_VertexCount;
  
    l_File.read((char*)&l_VertexCount, sizeof(uint32));

    SNORMALTEXTUREDVERTEX* l_pVertexBuffer = new SNORMALTEXTUREDVERTEX[l_VertexCount];
    l_File.read((char *)&l_pVertexBuffer[0], sizeof(SNORMALTEXTUREDVERTEX)*l_VertexCount);
  
  
    uint32 l_IndexCount;
  
    l_File.read((char*)&l_IndexCount, sizeof(uint32));

    uint16 * l_pIndexList = new uint16[l_IndexCount];
    l_File.read((char *)&l_pIndexList[0], sizeof(uint16)*l_IndexCount);

    CIndexedVertexs<SNORMALTEXTUREDVERTEX> *l_IndexedVertexs=new CIndexedVertexs<SNORMALTEXTUREDVERTEX>(RENDER_MANAGER, l_pVertexBuffer, l_pIndexList, l_VertexCount, l_IndexCount);
    m_RVs.push_back(l_IndexedVertexs);
  
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
      vector<CTexture*>::const_iterator l_ItTexture = (*l_ItTextureArray).begin();
      vector<CTexture*>::const_iterator l_EndTexture = (*l_ItTextureArray).end();
      int stage = 0;
      while(l_ItTexture != l_EndTexture)
      {
        (*l_ItTexture)->Activate(stage);
        ++l_ItTexture;
        ++stage;
      }

      (*l_ItRV)->Render(_pRM);
      ++l_ItRV;
      ++l_ItTextureArray;
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
