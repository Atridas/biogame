#include "Core.h"
#include "RenderManager.h"
#include "StaticMesh.h"
#include "VertexsStructs.h"
#include <IndexedVertexs.h>
#include <base.h>
#include <fstream>

#define HEADER 0xAAAA
#define FOOTER 0xFFFF

bool CStaticMesh::LoadSergi(const string &_szFileName)
{
  unsigned short l_header = 0;
  unsigned short l_footer = 0;
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
    l_file.read((char*)&l_vertexCount,sizeof(unsigned short));

    l_vertex = new SNORMALTEXTUREDVERTEX[l_vertexCount];
  
    l_file.read((char*)l_vertex,sizeof(SNORMALTEXTUREDVERTEX)*l_vertexCount);

    l_file.read((char*)&l_indexCount,sizeof(unsigned short));

    l_index = new uint16[l_indexCount];

    l_file.read((char*)l_index,sizeof(uint16)*l_indexCount);

    l_file.read((char*)&l_footer,sizeof(unsigned short));

    l_file.close();

    m_RVs.push_back(new CIndexedVertexs<SNORMALTEXTUREDVERTEX>(RENDER_MANAGER, l_vertex, l_index, l_vertexCount, l_indexCount));

    CHECKED_DELETE(l_vertex);
    CHECKED_DELETE(l_index);
  
    return l_footer==0xffff;
  }
  return false;
}


bool CStaticMesh::Load(const string &_szFileName)
{
  
  fstream l_File;
  uint16 l_usHelper = 0;
  uint16 l_usVertexType = 0;
  uint16 l_usTextureNum = 0;
  
  l_File.open(_szFileName, fstream::in | fstream::binary );
  if(!l_File.is_open())
  {
    LOGGER->AddNewLog(ELL_WARNING, "CStaticMesh::Load no s'ha pogut obrir el fitxer.");
    return false;
  }


  l_File.read((char*)&l_usHelper, sizeof(uint16));

  if(l_usHelper != HEADER)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CStaticMesh::Load reading file with incorrect header");
    l_File.close();
    return false;
  }

  //Vertex Type
  l_File.read((char*)&l_usVertexType, sizeof(uint16));
  l_File.read((char*)&l_usTextureNum, sizeof(uint16));

  for(int i = 0; i < l_usTextureNum; i++)
  {
    uint16 l_usTextLen = 0;
    l_File.read((char*)&l_usTextLen, sizeof(uint16));
    char* l_pcTexture = new char[++l_usTextLen];
    
    l_File.read(l_pcTexture, sizeof(char) * l_usTextLen);
    
    //TODO algo
    delete l_pcTexture;
  }

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


  l_File.read((char*)&l_usHelper, sizeof(uint16));

  if(l_usHelper != FOOTER)
  {
    LOGGER->AddNewLog(ELL_WARNING, "CStaticMesh::Load reading file with incorrect header");
    l_File.close();
    return false;
  }
  
  l_File.close();
  m_szFileName = _szFileName;

  SetOk(true);
  return IsOk();
}

void CStaticMesh::Render(CRenderManager *_pRM) const
{
    vector<CRenderableVertexs*>::const_iterator l_It = m_RVs.begin();
    vector<CRenderableVertexs*>::const_iterator l_End = m_RVs.end();
    while(l_It != l_End) 
    {
      (*l_It)->Render(_pRM);
      ++l_It;
    }
}

void CStaticMesh::Release()
{
    vector<CRenderableVertexs*>::iterator l_It = m_RVs.begin();
    vector<CRenderableVertexs*>::iterator l_End = m_RVs.end();

    while(l_It != l_End) 
    {
      CHECKED_DELETE(*l_It);
      ++l_It;
    }

    m_RVs.clear();
}
