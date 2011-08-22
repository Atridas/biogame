#include "VertexsStructs.h"

#include "Texture.h"

LPDIRECT3DVERTEXDECLARATION9 SDIFFUSEVERTEX::s_VertexDeclaration = 0;

LPDIRECT3DVERTEXDECLARATION9 SDIFFUSESCREENVERTEX::s_VertexDeclaration = 0;
LPDIRECT3DVERTEXDECLARATION9 STEXTUREDSCREENVERTEX::s_VertexDeclaration = 0;
LPDIRECT3DVERTEXDECLARATION9 SDIFFUSETEXTUREDSCREENVERTEX::s_VertexDeclaration = 0;

LPDIRECT3DVERTEXDECLARATION9 STEXTUREDVERTEX::s_VertexDeclaration = 0;
LPDIRECT3DVERTEXDECLARATION9 STEXTURED2VERTEX::s_VertexDeclaration = 0;

LPDIRECT3DVERTEXDECLARATION9 SNORMALDIFFUSEVERTEX::s_VertexDeclaration = 0;

LPDIRECT3DVERTEXDECLARATION9 SNORMALTEXTUREDVERTEX::s_VertexDeclaration = 0;
LPDIRECT3DVERTEXDECLARATION9 SNORMALTEXTURED2VERTEX::s_VertexDeclaration = 0;
LPDIRECT3DVERTEXDECLARATION9 TNORMALTANGENTBINORMALTEXTUREDVERTEX::s_VertexDeclaration=0;
LPDIRECT3DVERTEXDECLARATION9 TNORMALTANGENTBINORMALTEXTURED2VERTEX::s_VertexDeclaration=0;

LPDIRECT3DVERTEXDECLARATION9 TCAL3D_HW_VERTEX::s_VertexDeclaration=0;

LPDIRECT3DVERTEXDECLARATION9 SPARTICLE_VERTEX::s_VertexDeclaration=0;

// Instanced declarations ------------------------------------------------------

LPDIRECT3DVERTEXDECLARATION9 SDIFFUSEVERTEX::s_VertexInstancedDeclaration = 0;

LPDIRECT3DVERTEXDECLARATION9 SDIFFUSESCREENVERTEX::s_VertexInstancedDeclaration = 0;
LPDIRECT3DVERTEXDECLARATION9 STEXTUREDSCREENVERTEX::s_VertexInstancedDeclaration = 0;
LPDIRECT3DVERTEXDECLARATION9 SDIFFUSETEXTUREDSCREENVERTEX::s_VertexInstancedDeclaration = 0;

LPDIRECT3DVERTEXDECLARATION9 STEXTUREDVERTEX::s_VertexInstancedDeclaration = 0;
LPDIRECT3DVERTEXDECLARATION9 STEXTURED2VERTEX::s_VertexInstancedDeclaration = 0;

LPDIRECT3DVERTEXDECLARATION9 SNORMALDIFFUSEVERTEX::s_VertexInstancedDeclaration = 0;

LPDIRECT3DVERTEXDECLARATION9 SNORMALTEXTUREDVERTEX::s_VertexInstancedDeclaration = 0;
LPDIRECT3DVERTEXDECLARATION9 SNORMALTEXTURED2VERTEX::s_VertexInstancedDeclaration = 0;
LPDIRECT3DVERTEXDECLARATION9 TNORMALTANGENTBINORMALTEXTUREDVERTEX::s_VertexInstancedDeclaration=0;
LPDIRECT3DVERTEXDECLARATION9 TNORMALTANGENTBINORMALTEXTURED2VERTEX::s_VertexInstancedDeclaration=0;

LPDIRECT3DVERTEXDECLARATION9 TCAL3D_HW_VERTEX::s_VertexInstancedDeclaration=0;

LPDIRECT3DVERTEXDECLARATION9 SPARTICLE_VERTEX::s_VertexInstancedDeclaration=0;

//---------------------------------------------------------------------------------------------


uint16 GetVertexSize(uint16 _usVertexType)
{
  if(_usVertexType == SDIFFUSEVERTEX::GetVertexType())
  {
    return sizeof(SDIFFUSEVERTEX);
  } else if(_usVertexType == STEXTUREDVERTEX::GetVertexType())
  {
    return sizeof(STEXTUREDVERTEX);
  } else if(_usVertexType == STEXTURED2VERTEX::GetVertexType())
  {
    return sizeof(STEXTURED2VERTEX);
  } else if(_usVertexType == STEXTUREDSCREENVERTEX::GetVertexType())
  {
    return sizeof(STEXTUREDSCREENVERTEX);
  } else if(_usVertexType == SDIFFUSETEXTUREDSCREENVERTEX::GetVertexType())
  {
    return sizeof(SDIFFUSETEXTUREDSCREENVERTEX);
  } else if(_usVertexType == SDIFFUSESCREENVERTEX::GetVertexType())
  {
    return sizeof(SDIFFUSESCREENVERTEX);
  } else if(_usVertexType == SNORMALDIFFUSEVERTEX::GetVertexType())
  {
    return sizeof(SNORMALDIFFUSEVERTEX);
  } else if(_usVertexType == SNORMALTEXTUREDVERTEX::GetVertexType())
  {
    return sizeof(SNORMALTEXTUREDVERTEX);
  } else if(_usVertexType == SNORMALTEXTURED2VERTEX::GetVertexType())
  {
    return sizeof(SNORMALTEXTURED2VERTEX);
  } else if(_usVertexType == TNORMALTANGENTBINORMALTEXTUREDVERTEX::GetVertexType())
  {
    return sizeof(TNORMALTANGENTBINORMALTEXTUREDVERTEX);
  } else if(_usVertexType == TNORMALTANGENTBINORMALTEXTURED2VERTEX::GetVertexType())
  {
    return sizeof(TNORMALTANGENTBINORMALTEXTURED2VERTEX);
  } else if(_usVertexType == SPARTICLE_VERTEX::GetVertexType())
  {
    return sizeof(SPARTICLE_VERTEX);
  } else {
    return 0;
  }
}


uint16 GetTextureNum(uint16 _usVertexType)
{
  /*uint16 num = 0;
  if(_usVertexType & VERTEX_TYPE_TEXTURE1)
  {
    num += 1;
  } else if(_usVertexType & VERTEX_TYPE_TEXTURE2)
  {
    num += 2;
  }
  if(_usVertexType & VERTEX_TYPE_TANGENT & VERTEX_TYPE_BINORMAL )
  {
    num += 2;
  }
  return num;*/
  
  if(_usVertexType == SDIFFUSEVERTEX::GetVertexType())
  {
    return 1;
  } else if(_usVertexType == STEXTUREDVERTEX::GetVertexType())
  {
    return 2;
  } else if(_usVertexType == STEXTUREDSCREENVERTEX::GetVertexType())
  {
    return 1;
  } else if(_usVertexType == SDIFFUSETEXTUREDSCREENVERTEX::GetVertexType())
  {
    return 1;
  } else if(_usVertexType == SDIFFUSESCREENVERTEX::GetVertexType())
  {
    return 0;
  } else if(_usVertexType == STEXTURED2VERTEX::GetVertexType())
  {
    return 3;
  } else if(_usVertexType == SNORMALDIFFUSEVERTEX::GetVertexType())
  {
    return 0;
  } else if(_usVertexType == SNORMALTEXTUREDVERTEX::GetVertexType())
  {
    return 1;
  } else if(_usVertexType == SNORMALTEXTURED2VERTEX::GetVertexType())
  {
    return 2;
  } else if(_usVertexType == TNORMALTANGENTBINORMALTEXTUREDVERTEX::GetVertexType())
  {
    return 2;
  } else if(_usVertexType == TNORMALTANGENTBINORMALTEXTURED2VERTEX::GetVertexType())
  {
    return 3;
  } else {
    return 0;
  }
}


// Diffuse Vertex -----------------------------------------------------------------------------------------------------------

unsigned short SDIFFUSEVERTEX::GetVertexType()
{
  return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_DIFFUSE;
}

unsigned int SDIFFUSEVERTEX::GetFVF()
{
	return D3DFVF_XYZ|D3DFVF_DIFFUSE;
}

LPDIRECT3DVERTEXDECLARATION9& SDIFFUSEVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {          //type            sempre default (per meshes)    //ús de les dades
      { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
      { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0  },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& SDIFFUSEVERTEX::GetInstancedVertexDeclaration()
{
  if(s_VertexInstancedDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {          //type            sempre default (per meshes)    //ús de les dades
      { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
      //Buffer
      { 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      { 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      //World matrix
      { 1, 16 * 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
      { 1, 16 * 1, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
      { 1, 16 * 2, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
      { 1, 16 * 3, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },

      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexInstancedDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexInstancedDeclaration;
}


// Diffuse Screen Vertex -----------------------------------------------------------------------------------------------------------

unsigned short SDIFFUSESCREENVERTEX::GetVertexType()
{
  return VERTEX_TYPE_SCREENGEO|VERTEX_TYPE_DIFFUSE|VERTEX_TYPE_TEXTURE1;
}

unsigned int SDIFFUSESCREENVERTEX::GetFVF()
{
  return D3DFVF_XYZRHW|D3DFVF_DIFFUSE;
}

LPDIRECT3DVERTEXDECLARATION9& SDIFFUSESCREENVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {          //type            sempre default (per meshes)    //ús de les dades
      { 0, 0,  D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
      { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,     0 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& SDIFFUSESCREENVERTEX::GetInstancedVertexDeclaration()
{
  if(s_VertexInstancedDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {          //type            sempre default (per meshes)    //ús de les dades
      { 0, 0,  D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
      { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,     0 },
      //Buffer
      { 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      { 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      //World matrix
      { 1, 16 * 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
      { 1, 16 * 1, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
      { 1, 16 * 2, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
      { 1, 16 * 3, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexInstancedDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexInstancedDeclaration;
}


// Textured Screen Vertex -----------------------------------------------------------------------------------------------------------

unsigned short STEXTUREDSCREENVERTEX::GetVertexType()
{
  return VERTEX_TYPE_SCREENGEO|VERTEX_TYPE_TEXTURE1;
}

unsigned int STEXTUREDSCREENVERTEX::GetFVF()
{
  return D3DFVF_XYZRHW|D3DFVF_TEX1;
}

LPDIRECT3DVERTEXDECLARATION9& STEXTUREDSCREENVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {          //type            sempre default (per meshes)    //ús de les dades
      { 0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
      { 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& STEXTUREDSCREENVERTEX::GetInstancedVertexDeclaration()
{
  if(s_VertexInstancedDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {          //type            sempre default (per meshes)    //ús de les dades
      { 0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
      { 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
      //Buffer
      { 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      //World matrix
      { 1, 16 * 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
      { 1, 16 * 1, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
      { 1, 16 * 2, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
      { 1, 16 * 3, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexInstancedDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexInstancedDeclaration;
}


// Diffuse Textured Screen Vertex -----------------------------------------------------------------------------------------------------------

unsigned short SDIFFUSETEXTUREDSCREENVERTEX::GetVertexType()
{
  return VERTEX_TYPE_SCREENGEO|VERTEX_TYPE_DIFFUSE|VERTEX_TYPE_TEXTURE1;
}

unsigned int SDIFFUSETEXTUREDSCREENVERTEX::GetFVF()
{
  return D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1;
}

LPDIRECT3DVERTEXDECLARATION9& SDIFFUSETEXTUREDSCREENVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {                           //type            sempre default (per meshes)    //ús de les dades
      { 0, 0,                   D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
      { 0, 12,                  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,     0 },
      { 0, 12 + sizeof(uint32), D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& SDIFFUSETEXTUREDSCREENVERTEX::GetInstancedVertexDeclaration()
{
  if(s_VertexInstancedDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {                           //type            sempre default (per meshes)    //ús de les dades
      { 0, 0,                   D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 },
      { 0, 12,                  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,     0 },
      { 0, 12 + sizeof(uint32), D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
      //Buffer
      { 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      //World matrix
      { 1, 16 * 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
      { 1, 16 * 1, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
      { 1, 16 * 2, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
      { 1, 16 * 3, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexInstancedDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexInstancedDeclaration;
}


// Normal Diffuse Vertex -----------------------------------------------------------------------------------------------------------

unsigned short SNORMALDIFFUSEVERTEX::GetVertexType()
{
  return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_DIFFUSE;
}

unsigned int SNORMALDIFFUSEVERTEX::GetFVF()
{
	return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE;
}

LPDIRECT3DVERTEXDECLARATION9& SNORMALDIFFUSEVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {        //type            sempre default (per meshes)    //ús de les dades
      { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
      { 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& SNORMALDIFFUSEVERTEX::GetInstancedVertexDeclaration()
{
  if(s_VertexInstancedDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {        //type            sempre default (per meshes)    //ús de les dades
      { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
      { 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
      //Buffer
      { 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      { 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      //World matrix
      { 1, 16 * 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
      { 1, 16 * 1, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
      { 1, 16 * 2, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
      { 1, 16 * 3, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexInstancedDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexInstancedDeclaration;
}

// Normal Tangent Binormal Textured --------------------------------------------------------------------------------
unsigned short TNORMALTANGENTBINORMALTEXTUREDVERTEX::GetVertexType()
{
  return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_BITANGENT | VERTEX_TYPE_TEXTURE1;
}

unsigned int TNORMALTANGENTBINORMALTEXTUREDVERTEX::GetFVF()
{
  return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1; //TODO?
}

LPDIRECT3DVERTEXDECLARATION9& TNORMALTANGENTBINORMALTEXTUREDVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {                        //type            sempre default (per meshes)    //ús de les dades
      { 0, 0,                D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, sizeof(float)*3,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
      { 0, sizeof(float)*7,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
      { 0, sizeof(float)*11, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
      { 0, sizeof(float)*15, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& TNORMALTANGENTBINORMALTEXTUREDVERTEX::GetInstancedVertexDeclaration()
{
  if(s_VertexInstancedDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {                        //type            sempre default (per meshes)    //ús de les dades
      { 0, 0,                D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, sizeof(float)*3,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
      { 0, sizeof(float)*7,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
      { 0, sizeof(float)*11, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
      { 0, sizeof(float)*15, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      //Buffer
      { 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      //World matrix
      { 1, 16 * 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
      { 1, 16 * 1, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
      { 1, 16 * 2, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
      { 1, 16 * 3, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexInstancedDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexInstancedDeclaration;
}


// Normal Tangent Binormal Textured 2-------------------------------------------------------------------------------
unsigned short TNORMALTANGENTBINORMALTEXTURED2VERTEX::GetVertexType()
{
  return VERTEX_TYPE_GEOMETRY | VERTEX_TYPE_NORMAL | VERTEX_TYPE_TANGENT | VERTEX_TYPE_BITANGENT | VERTEX_TYPE_TEXTURE2;
}

unsigned int TNORMALTANGENTBINORMALTEXTURED2VERTEX::GetFVF()
{
  return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2; //TODO?
}

LPDIRECT3DVERTEXDECLARATION9& TNORMALTANGENTBINORMALTEXTURED2VERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {                        //type            sempre default (per meshes)    //ús de les dades
      { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, sizeof(float)*3 , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
      { 0, sizeof(float)*7 , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
      { 0, sizeof(float)*11 , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
      { 0, sizeof(float)*15 , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      { 0, sizeof(float)*17 , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& TNORMALTANGENTBINORMALTEXTURED2VERTEX::GetInstancedVertexDeclaration()
{
  if(s_VertexInstancedDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {                        //type            sempre default (per meshes)    //ús de les dades
      { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, sizeof(float)*3 , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
      { 0, sizeof(float)*7 , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
      { 0, sizeof(float)*11 , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
      { 0, sizeof(float)*15 , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      { 0, sizeof(float)*17 , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      //Buffer
      // -----------------------------------------------------------------------------------------
      //World matrix
      { 1, 16 * 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
      { 1, 16 * 1, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
      { 1, 16 * 2, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
      { 1, 16 * 3, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexInstancedDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexInstancedDeclaration;
}

// Textured Vertex -------------------------------------------------------------------------------------------------

unsigned short STEXTUREDVERTEX::GetVertexType()
{
  return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_TEXTURE1;
}

unsigned int STEXTUREDVERTEX::GetFVF()
{
	return D3DFVF_XYZ|D3DFVF_TEX1;
}

LPDIRECT3DVERTEXDECLARATION9& STEXTUREDVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, sizeof(float)*3 , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& STEXTUREDVERTEX::GetInstancedVertexDeclaration()
{
  if(s_VertexInstancedDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, sizeof(float)*3 , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      //Buffer
      { 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      //World matrix
      { 1, 16 * 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
      { 1, 16 * 1, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
      { 1, 16 * 2, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
      { 1, 16 * 3, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexInstancedDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexInstancedDeclaration;
}

// Textured 2 Vertex -----------------------------------------------------------------------------------------------
unsigned short STEXTURED2VERTEX::GetVertexType()
{
  return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_TEXTURE2;
}

unsigned int STEXTURED2VERTEX::GetFVF()
{
	return D3DFVF_XYZ|D3DFVF_TEX4;
}

LPDIRECT3DVERTEXDECLARATION9& STEXTURED2VERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, sizeof(float)*3 , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      { 0, sizeof(float)*5 , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& STEXTURED2VERTEX::GetInstancedVertexDeclaration()
{
  if(s_VertexInstancedDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, sizeof(float)*3 , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      { 0, sizeof(float)*5 , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      //Buffer
      //-------------------------------------------------------------------------------------------------
      //World matrix
      { 1, 16 * 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
      { 1, 16 * 1, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
      { 1, 16 * 2, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
      { 1, 16 * 3, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexInstancedDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexInstancedDeclaration;
}


// Normal Textured Vertex ------------------------------------------------------------------------------------------
unsigned short SNORMALTEXTUREDVERTEX::GetVertexType()
{
  return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE1;
}

unsigned int SNORMALTEXTUREDVERTEX::GetFVF()
{
	return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2;
}

LPDIRECT3DVERTEXDECLARATION9& SNORMALTEXTUREDVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, sizeof(float)*3 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
      { 0, sizeof(float)*6 , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& SNORMALTEXTUREDVERTEX::GetInstancedVertexDeclaration()
{
  if(s_VertexInstancedDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, sizeof(float)*3 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
      { 0, sizeof(float)*6 , D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      //Buffer
      { 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      //World matrix
      { 1, 16 * 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
      { 1, 16 * 1, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
      { 1, 16 * 2, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
      { 1, 16 * 3, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexInstancedDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexInstancedDeclaration;
}

// Normal Textured2 Vertex -----------------------------------------------------------------------------------------
unsigned short SNORMALTEXTURED2VERTEX::GetVertexType()
{
  return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE2;
}

unsigned int SNORMALTEXTURED2VERTEX::GetFVF()
{
	return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX4;
}

LPDIRECT3DVERTEXDECLARATION9& SNORMALTEXTURED2VERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 0 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, sizeof(float)*3 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
      { 0,    sizeof(float)*6 ,     D3DDECLTYPE_FLOAT2,    D3DDECLMETHOD_DEFAULT,    D3DDECLUSAGE_TEXCOORD,    0  },
      { 0,    sizeof(float)*8 ,   D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT,   D3DDECLUSAGE_TEXCOORD, 1  },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}

LPDIRECT3DVERTEXDECLARATION9& SNORMALTEXTURED2VERTEX::GetInstancedVertexDeclaration()
{
  if(s_VertexInstancedDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 0,               D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
      { 0, sizeof(float)*3, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
      { 0, sizeof(float)*6, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      { 0, sizeof(float)*8, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      //World matrix
      { 1, 16 * 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
      { 1, 16 * 1, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
      { 1, 16 * 2, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
      { 1, 16 * 3, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexInstancedDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexInstancedDeclaration;
}


// Cal 3D Vertex ---------------------------------------------------------------------------------------------------


LPDIRECT3DVERTEXDECLARATION9& TCAL3D_HW_VERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 0 ,               D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0 },
      { 0, sizeof(float)*3 , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,  0 },
      { 0, sizeof(float)*7 , D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
      { 0, sizeof(float)*11, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,       0 },
      { 0, sizeof(float)*14, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     0 },
      { 0, sizeof(float)*16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,      0 },
      { 0, sizeof(float)*19, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,     0 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}


LPDIRECT3DVERTEXDECLARATION9& TCAL3D_HW_VERTEX::GetInstancedVertexDeclaration()
{
  if(s_VertexInstancedDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 0,                D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0 },
      { 0, sizeof(float)*3,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,  0 },
      { 0, sizeof(float)*7,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
      { 0, sizeof(float)*11, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,       0 },
      { 0, sizeof(float)*14, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,     0 },
      { 0, sizeof(float)*16,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
      { 0, sizeof(float)*19 , D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
      //Buffer
      { 1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      //World matrix
      { 1, 16 * 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
      { 1, 16 * 1, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
      { 1, 16 * 2, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
      { 1, 16 * 3, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexInstancedDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexInstancedDeclaration;
}



// Particle Texture ---------------------------------------------------------------------------------------------------


LPDIRECT3DVERTEXDECLARATION9& SPARTICLE_VERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      /*
      { 0, 0 ,               D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0 },
      D3DDECL_END()
      */
      { 0, 0,                D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0 },
      //Buffer
      { 1, 0              ,  D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      { 1, sizeof(float)*4,  D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      { 1, sizeof(float)*8,  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,  0 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}


LPDIRECT3DVERTEXDECLARATION9& SPARTICLE_VERTEX::GetInstancedVertexDeclaration()
{
  if(s_VertexInstancedDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      /*
      { 0, 0 ,               D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0 },
      D3DDECL_END()
      */
      { 0, 0,                D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,     0 },
      //Buffer
      { 1, 0              ,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
      { 1, sizeof(float)*3,  D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
      { 1, sizeof(float)*7,  D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
      { 1, sizeof(float)*11, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,  0 },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexInstancedDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexInstancedDeclaration;
}
