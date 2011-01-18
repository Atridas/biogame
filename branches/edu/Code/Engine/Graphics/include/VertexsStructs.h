#pragma once
#ifndef __VERTEX_STRUCTS_H__
#define __VERTEX_STRUCTS_H__

#include <d3d9.h>

#include <d3dx9.h>


#define VERTEX_TYPE_GEOMETRY 0x0001
#define VERTEX_TYPE_NORMAL   0x0002
#define VERTEX_TYPE_TANGENT  0x0004
#define VERTEX_TYPE_BINORMAL 0x0008
#define VERTEX_TYPE_TEXTURE1 0x0010
#define VERTEX_TYPE_TEXTURE2 0x0020
#define VERTEX_TYPE_DIFFUSE  0x0040


/**
 * Getter del tamany del v�rtex.
 * @param _usVertexType Tipus de v�rtex.
 * @return Tamany en bytes del v�rtex.
**/
inline uint16 GetVertexSize(uint16 _usVertexType)
{
  uint16 size = 0;
  if(_usVertexType & VERTEX_TYPE_GEOMETRY)
  {
    size += 3 * sizeof(float);
  }
  if(_usVertexType & VERTEX_TYPE_NORMAL)
  {
    size += 3 * sizeof(float);
  }
  if(_usVertexType & VERTEX_TYPE_TEXTURE1)
  {
    size += 2 * sizeof(float);
  } else if(_usVertexType & VERTEX_TYPE_TEXTURE2)
  {
    size += 4 * sizeof(float);
  }
  if(_usVertexType & VERTEX_TYPE_DIFFUSE)
  {
    size += sizeof(unsigned long);
  }
  return size;
}

/**
 * Getter del nombre de textures.
 * @param _usVertexType Tipus de v�rtex.
 * @return Nombre de textures que cont� el v�rtex.
**/
inline uint16 GetTextureNum(uint16 _usVertexType)
{
  uint16 num = 0;
  if(_usVertexType & VERTEX_TYPE_TEXTURE1)
  {
    num += 1;
  } else if(_usVertexType & VERTEX_TYPE_TEXTURE2)
  {
    num += 2;
  }
  if(_usVertexType & (VERTEX_TYPE_TANGENT | VERTEX_TYPE_BINORMAL) )
  {
    num += 2;
  }
  return num;
}

/**
 * Estructura Vertex Difus.
 * Representa a un vertex amb coordenades geom�triques i color dif�s.
**/
struct SDIFFUSEVERTEX
{
	float x, y, z;
  uint32 color;

  /**
   * Getter del tipus de v�rtex.
   * Els tipus de v�rtex �s una m�scara donada pel seg�ent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del v�rtex.
  **/
  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_DIFFUSE;
  }

  /**
   * Getter del tipus de v�rtex.
   * @return El tipus del v�rtex segons la codificaci� de DirectX9.
  **/
	static unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_DIFFUSE;
	}
};

/**
 * Estructura Vertex Difus amb Normals.
 * Representa a un vertex amb coordenades geom�triques, normals i color dif�s.
**/
struct SNORMALDIFSSUSEVERTEX
{
	float x, y, z;
  float nx, ny, nz;
  uint32 color;

  /**
   * Getter del tipus de v�rtex.
   * Els tipus de v�rtex �s una m�scara donada pel seg�ent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del v�rtex.
  **/
  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_DIFFUSE;
  }

	/**
   * Getter del tipus de v�rtex.
   * @return El tipus del v�rtex segons la codificaci� de DirectX9.
  **/
  static unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE;
	}
};

/*
struct SNORMALDIFFUSETEXTUREDVERTEX
{
	float x, y, z;
  float nx, ny, nz;
  uint32 color;
  float u1, v1;

  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_DIFFUSE|VERTEX_TYPE_TEXTURE1;
  }

	static unsigned int GetFVF()
	{
    return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX2;
	}
};

struct SNORMALDIFFUSEDTEXTURED2VERTEX
{
	float x, y, z;
  float nx, ny, nz;
  uint32 color;
  float u1, v1;
  float u2, v2;

  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_DIFFUSE|VERTEX_TYPE_TEXTURE2;
  }

	static unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX4;
	}
};
*/

/**
 * Estructura Vertex amb Textura.
 * Representa a un vertex amb coordenades geom�triques i de textura.
**/
struct STEXTUREDVERTEX
{
	float x, y, z;
  float u1, v1;

  /**
   * Getter del tipus de v�rtex.
   * Els tipus de v�rtex �s una m�scara donada pel seg�ent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del v�rtex.
  **/
  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_TEXTURE1;
  }

	/**
   * Getter del tipus de v�rtex.
   * @return El tipus del v�rtex segons la codificaci� de DirectX9.
  **/
  static unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_TEX2;
	}
};

/**
 * Estructura Vertex amb dues Textures.
 * Representa a un vertex amb coordenades geom�triques de dues textures.
**/
struct STEXTURED2VERTEX
{
	float x, y, z;
  float u1, v1;
  float u2, v2;

  /**
   * Getter del tipus de v�rtex.
   * Els tipus de v�rtex �s una m�scara donada pel seg�ent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del v�rtex.
  **/
  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_TEXTURE2;
  }

	/**
   * Getter del tipus de v�rtex.
   * @return El tipus del v�rtex segons la codificaci� de DirectX9.
  **/
  static unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_TEX4;
	}
};

/**
 * Estructura Vertex amb Normals i Textura.
 * Representa a un vertex amb coordenades geom�triques, normals i de textura.
**/
struct SNORMALTEXTUREDVERTEX
{
	float x, y, z;
  float nx, ny, nz;
  float u1, v1;

  /**
   * Getter del tipus de v�rtex.
   * Els tipus de v�rtex �s una m�scara donada pel seg�ent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del v�rtex.
  **/
  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE1;
  }

	/**
   * Getter del tipus de v�rtex.
   * @return El tipus del v�rtex segons la codificaci� de DirectX9.
  **/
  static unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2;
	}
};

/**
 * Estructura Vertex amb Normals i dues Textura.
 * Representa a un vertex amb coordenades geom�triques, normals i de dues textures.
**/
struct SNORMALTEXTURED2VERTEX
{
	float x, y, z;
  float nx, ny, nz;
  float u1, v1;
  float u2, v2;

  /**
   * Getter del tipus de v�rtex.
   * Els tipus de v�rtex �s una m�scara donada pel seg�ent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del v�rtex.
  **/
  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE2;
  }

	/**
   * Getter del tipus de v�rtex.
   * @return El tipus del v�rtex segons la codificaci� de DirectX9.
  **/
  static unsigned int GetFVF()
	{
		return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX4;
	}
};

#endif