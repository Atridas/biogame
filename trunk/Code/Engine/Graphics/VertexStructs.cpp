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

bool SDIFFUSEVERTEX::ActivateTextures(const vector<CTexture*>& _TextureArray)
{
  assert(_TextureArray.size() == 1);
  _TextureArray[0]->Activate(1); // normal
  return true;
}

LPDIRECT3DVERTEXDECLARATION9& SDIFFUSEVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 
        0 , 
        D3DDECLTYPE_FLOAT3,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_POSITION,  //ús de les dades
        0 
      },
      { 0, 
        12 ,                    //desplaçament 
        D3DDECLTYPE_D3DCOLOR,   //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_COLOR,     //ús de les dades
        0 
      },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
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

bool SDIFFUSESCREENVERTEX::ActivateTextures(const vector<CTexture*>& _TextureArray)
{
  return true;
}

LPDIRECT3DVERTEXDECLARATION9& SDIFFUSESCREENVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 
        0 , 
        D3DDECLTYPE_FLOAT4,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_POSITIONT,  //ús de les dades
        0 
      },
      { 0, 
        12 ,                    //desplaçament 
        D3DDECLTYPE_D3DCOLOR,   //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_COLOR,     //ús de les dades
        0 
      },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
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

bool STEXTUREDSCREENVERTEX::ActivateTextures(const vector<CTexture*>& _TextureArray)
{
  assert(_TextureArray.size() == 1);
  _TextureArray[0]->Activate(1); // normal
  return true;
}

LPDIRECT3DVERTEXDECLARATION9& STEXTUREDSCREENVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 
        0 , 
        D3DDECLTYPE_FLOAT4,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_POSITIONT,  //ús de les dades
        0 
      },
      { 0, 
        12 ,                    //desplaçament 
        D3DDECLTYPE_FLOAT2,   //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TEXCOORD,     //ús de les dades
        0 
      },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
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

bool SDIFFUSETEXTUREDSCREENVERTEX::ActivateTextures(const vector<CTexture*>& _TextureArray)
{
  assert(_TextureArray.size() == 1);
  _TextureArray[0]->Activate(1); // normal
  return true;
}

LPDIRECT3DVERTEXDECLARATION9& SDIFFUSETEXTUREDSCREENVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 
        0 , 
        D3DDECLTYPE_FLOAT4,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_POSITIONT,  //ús de les dades
        0 
      },
      { 0, 
        12 ,                    //desplaçament 
        D3DDECLTYPE_D3DCOLOR,   //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_COLOR,     //ús de les dades
        0 
      },
      { 0, 
        12 + sizeof(uint32) ,                    //desplaçament 
        D3DDECLTYPE_FLOAT2,   //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TEXCOORD,     //ús de les dades
        0 
      },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
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

bool SNORMALDIFFUSEVERTEX::ActivateTextures(const vector<CTexture*>& _TextureArray)
{
  assert(_TextureArray.size() == 0);
  return true;
}

LPDIRECT3DVERTEXDECLARATION9& SNORMALDIFFUSEVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 
        0 , 
        D3DDECLTYPE_FLOAT3,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_POSITION,  //ús de les dades
        0 
      },
      { 0, 
        12 ,                    //desplaçament 
        D3DDECLTYPE_FLOAT3,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_NORMAL,    //ús de les dades
        0 
      },
      { 0, 
        24 ,                    //desplaçament 
        D3DDECLTYPE_D3DCOLOR,   //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_COLOR,     //ús de les dades
        0 
      },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
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

bool TNORMALTANGENTBINORMALTEXTUREDVERTEX::ActivateTextures(const vector<CTexture*>& _TextureArray)
{
  assert(_TextureArray.size() == 2);
  _TextureArray[0]->Activate(0); // diffuse
  _TextureArray[1]->Activate(1); // normal
  //_TextureArray[2]->Activate(2); // lightmap
  //_TextureArray[3]->Activate(3); // enviroment
  return true;
}

LPDIRECT3DVERTEXDECLARATION9& TNORMALTANGENTBINORMALTEXTUREDVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 
        0 , 
        D3DDECLTYPE_FLOAT3,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_POSITION,  //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*3 ,       //desplaçament 
        D3DDECLTYPE_FLOAT4,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_NORMAL,    //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*7 ,       //desplaçament 
        D3DDECLTYPE_FLOAT4,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TANGENT,   //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*11 ,      //desplaçament 
        D3DDECLTYPE_FLOAT4,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_BINORMAL,  //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*15 ,      //desplaçament 
        D3DDECLTYPE_FLOAT2,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TEXCOORD,  //ús de les dades
        0 
      },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
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

bool TNORMALTANGENTBINORMALTEXTURED2VERTEX::ActivateTextures(const vector<CTexture*>& _TextureArray)
{
  assert(_TextureArray.size() == 3);
  _TextureArray[0]->Activate(0); // diffuse
  _TextureArray[1]->Activate(1); // normal
  _TextureArray[2]->Activate(2); // lightmap
  //_TextureArray[3]->Activate(3); // enviroment
  return true;
}

LPDIRECT3DVERTEXDECLARATION9& TNORMALTANGENTBINORMALTEXTURED2VERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 
        0 , 
        D3DDECLTYPE_FLOAT3,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_POSITION,  //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*3 ,       //desplaçament 
        D3DDECLTYPE_FLOAT4,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_NORMAL,    //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*7 ,       //desplaçament 
        D3DDECLTYPE_FLOAT4,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TANGENT,   //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*11 ,      //desplaçament 
        D3DDECLTYPE_FLOAT4,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_BINORMAL,  //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*15 ,      //desplaçament 
        D3DDECLTYPE_FLOAT2,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TEXCOORD,  //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*17 ,      //desplaçament 
        D3DDECLTYPE_FLOAT2,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TEXCOORD,  //ús de les dades
        1 
      },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
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

bool STEXTUREDVERTEX::ActivateTextures(const vector<CTexture*>& _TextureArray)
{
  assert(_TextureArray.size() == 2);
  _TextureArray[0]->Activate(0); // diffuse
  _TextureArray[1]->Activate(1); // normal
  return true;
}

LPDIRECT3DVERTEXDECLARATION9& STEXTUREDVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 
        0 , 
        D3DDECLTYPE_FLOAT3,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_POSITION,  //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*3 ,                    //desplaçament 
        D3DDECLTYPE_FLOAT2,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TEXCOORD,  //ús de les dades
        0 
      },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
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

bool STEXTURED2VERTEX::ActivateTextures(const vector<CTexture*>& _TextureArray)
{
  assert(_TextureArray.size() == 3);
  _TextureArray[0]->Activate(0); // diffuse
  _TextureArray[1]->Activate(1); // normal
  _TextureArray[2]->Activate(2); // lightmap
  return true;
}

LPDIRECT3DVERTEXDECLARATION9& STEXTURED2VERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 
        0 , 
        D3DDECLTYPE_FLOAT3,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_POSITION,  //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*3 ,       //desplaçament
        D3DDECLTYPE_FLOAT2,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TEXCOORD,  //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*5 ,       //desplaçament
        D3DDECLTYPE_FLOAT2,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TEXCOORD,  //ús de les dades
        1 
      },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
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

bool SNORMALTEXTUREDVERTEX::ActivateTextures(const vector<CTexture*>& _TextureArray)
{
  assert(_TextureArray.size() == 1);
  _TextureArray[0]->Activate(0); // diffuse
  return true;
}

LPDIRECT3DVERTEXDECLARATION9& SNORMALTEXTUREDVERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 
        0 , 
        D3DDECLTYPE_FLOAT3,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_POSITION,  //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*3 ,       //desplaçament
        D3DDECLTYPE_FLOAT3,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_NORMAL,    //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*6 ,       //desplaçament
        D3DDECLTYPE_FLOAT2,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TEXCOORD,  //ús de les dades
        0 
      },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
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

bool SNORMALTEXTURED2VERTEX::ActivateTextures(const vector<CTexture*>& _TextureArray)
{
  assert(_TextureArray.size() == 2);
  _TextureArray[0]->Activate(0); // diffuse
  _TextureArray[1]->Activate(2); // lightmap
  return true;
}

LPDIRECT3DVERTEXDECLARATION9& SNORMALTEXTURED2VERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 
        0 , 
        D3DDECLTYPE_FLOAT3,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_POSITION,  //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*3 ,       //desplaçament
        D3DDECLTYPE_FLOAT3,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_NORMAL,    //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*6 ,       //desplaçament
        D3DDECLTYPE_FLOAT2,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TEXCOORD,  //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*8 ,       //desplaçament
        D3DDECLTYPE_FLOAT2,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TEXCOORD,  //ús de les dades
        1 
      },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}


// Cal 3D Vertex ---------------------------------------------------------------------------------------------------

bool TCAL3D_HW_VERTEX::ActivateTextures(const vector<CTexture*>& _TextureArray)
{
  //assert(_TextureArray.size() == 2);
  //_TextureArray[0]->Activate(0); // diffuse
  //_TextureArray[1]->Activate(2); // lightmap
  return true; // TODO
}

LPDIRECT3DVERTEXDECLARATION9& TCAL3D_HW_VERTEX::GetVertexDeclaration()
{
  if(s_VertexDeclaration==NULL)
  {
    D3DVERTEXELEMENT9 l_VertexDeclaration[] =
    {
      { 0, 
        0 , 
        D3DDECLTYPE_FLOAT3,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_POSITION,  //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*3 ,        //desplaçament
        D3DDECLTYPE_FLOAT4,      //type
        D3DDECLMETHOD_DEFAULT,   //---- sempre default (per meshes)
        D3DDECLUSAGE_BLENDWEIGHT,//ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*7 ,         //desplaçament
        D3DDECLTYPE_FLOAT4,       //type
        D3DDECLMETHOD_DEFAULT,    //---- sempre default (per meshes)
        D3DDECLUSAGE_BLENDINDICES,//ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*11 ,      //desplaçament
        D3DDECLTYPE_FLOAT3,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_NORMAL,    //ús de les dades
        0 
      },
      { 0, 
        sizeof(float)*14 ,      //desplaçament
        D3DDECLTYPE_FLOAT2,     //type
        D3DDECLMETHOD_DEFAULT,  //---- sempre default (per meshes)
        D3DDECLUSAGE_TEXCOORD,  //ús de les dades
        0 
      },
      D3DDECL_END()
    };
    HRESULT result = RENDER_MANAGER->GetDevice()->CreateVertexDeclaration(l_VertexDeclaration, &s_VertexDeclaration);
    assert(result == D3D_OK);
  }
  return s_VertexDeclaration;
}
