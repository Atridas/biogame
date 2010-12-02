#include "Core.h"
#include "RenderManager.h"
#include "StaticMesh.h"
#include "VertexsStructs.h"
#include <IndexedVertexs.h>
#include <base.h>
#include <fstream>

bool CStaticMesh::Load (const string &_szFileName)
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