#pragma once
#ifndef __VERTEX_STRUCTS_H__
#define __VERTEX_STRUCTS_H__

#include <d3d9.h>

#include <d3dx9.h>

#include <base.h>
#include <Core.h>
#include "RenderManager.h"

// Forward Declarations ---------------------------------------
class CTexture;
// ------------------------------------------------------------

#define VERTEX_TYPE_GEOMETRY 0x0001
#define VERTEX_TYPE_NORMAL   0x0002
#define VERTEX_TYPE_TANGENT  0x0004
#define VERTEX_TYPE_BINORMAL 0x0008
#define VERTEX_TYPE_TEXTURE1 0x0010
#define VERTEX_TYPE_TEXTURE2 0x0020
#define VERTEX_TYPE_DIFFUSE  0x0040


/**
 * Getter del tamany del vèrtex.
 * @param _usVertexType Tipus de vèrtex.
 * @return Tamany en bytes del vèrtex.
**/
uint16 GetVertexSize(uint16 _usVertexType);

/**
 * Getter del nombre de textures.
 * @param _usVertexType Tipus de vèrtex.
 * @return Nombre de textures que conté el vèrtex.
**/
uint16 GetTextureNum(uint16 _usVertexType);

/**
 * Estructura Vertex Difus.
 * Representa a un vertex amb coordenades geomètriques i color difús.
**/
struct SDIFFUSEVERTEX
{
	float x, y, z;
  uint32 color;

  /**
   * Getter del tipus de vèrtex.
   * Els tipus de vèrtex és una màscara donada pel següent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del vèrtex.
  **/
  static unsigned short GetVertexType();

  /**
   * Getter del tipus de vèrtex.
   * @return El tipus del vèrtex segons la codificació de DirectX9.
  **/
	static unsigned int GetFVF();

  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static bool ActivateTextures(const vector<CTexture*>& _TextureArray);

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }

private:
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
};

/**
 * Estructura Vertex Difus amb Normals.
 * Representa a un vertex amb coordenades geomètriques, normals i color difús.
**/
struct SNORMALDIFFUSEVERTEX
{
	float x, y, z;
  float nx, ny, nz;
  uint32 color;

  /**
   * Getter del tipus de vèrtex.
   * Els tipus de vèrtex és una màscara donada pel següent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del vèrtex.
  **/
  static unsigned short GetVertexType();

	/**
   * Getter del tipus de vèrtex.
   * @return El tipus del vèrtex segons la codificació de DirectX9.
  **/
  static unsigned int GetFVF();

  static LPDIRECT3DVERTEXDECLARATION9& GetVertexDeclaration();

  static bool ActivateTextures(const vector<CTexture*>& _TextureArray);

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }

private:
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
};
// codi afegit

struct TNORMALTANGENTBINORMALTEXTUREDVERTEX
{
  float x, y, z;
  float nx, ny, nz;
  float tangentx, tangenty, tangentz;
  float binormalx, binormaly, binormalz;
  float tu,tv;

  /**
   * Getter del tipus de vèrtex.
   * Els tipus de vèrtex és una màscara donada pel següent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del vèrtex.
  **/
  static unsigned short GetVertexType();

  static unsigned int GetFVF();

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }

  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static bool ActivateTextures(const vector<CTexture*>& _TextureArray);

private:
  
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
};

struct TNORMALTANGENTBINORMALTEXTURED2VERTEX
{
  float x, y, z;
  float nx, ny, nz;
  float tangentx, tangenty, tangentz;
  float binormalx, binormaly, binormalz;
  float tu,tv;
  float tu2,tv2;

  /**
   * Getter del tipus de vèrtex.
   * Els tipus de vèrtex és una màscara donada pel següent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del vèrtex.
  **/
  static unsigned short GetVertexType();

  static unsigned int GetFVF();

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }

  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static bool ActivateTextures(const vector<CTexture*>& _TextureArray);

private:
  
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
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
 * Representa a un vertex amb coordenades geomètriques i de textura.
**/
struct STEXTUREDVERTEX
{
	float x, y, z;
  float u1, v1;

  /**
   * Getter del tipus de vèrtex.
   * Els tipus de vèrtex és una màscara donada pel següent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del vèrtex.
  **/
  static unsigned short GetVertexType();

	/**
   * Getter del tipus de vèrtex.
   * @return El tipus del vèrtex segons la codificació de DirectX9.
  **/
  static unsigned int GetFVF();

  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static bool ActivateTextures(const vector<CTexture*>& _TextureArray);

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }

private:
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
};

/**
 * Estructura Vertex amb dues Textures.
 * Representa a un vertex amb coordenades geomètriques de dues textures.
**/
struct STEXTURED2VERTEX
{
	float x, y, z;
  float u1, v1;
  float u2, v2;

  /**
   * Getter del tipus de vèrtex.
   * Els tipus de vèrtex és una màscara donada pel següent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del vèrtex.
  **/
  static unsigned short GetVertexType();

	/**
   * Getter del tipus de vèrtex.
   * @return El tipus del vèrtex segons la codificació de DirectX9.
  **/
  static unsigned int GetFVF();

  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static bool ActivateTextures(const vector<CTexture*>& _TextureArray);

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }

private:
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
};

/**
 * Estructura Vertex amb Normals i Textura.
 * Representa a un vertex amb coordenades geomètriques, normals i de textura.
**/
struct SNORMALTEXTUREDVERTEX
{
	float x, y, z;
  float nx, ny, nz;
  float u1, v1;

  /**
   * Getter del tipus de vèrtex.
   * Els tipus de vèrtex és una màscara donada pel següent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del vèrtex.
  **/
  static unsigned short GetVertexType();

	/**
   * Getter del tipus de vèrtex.
   * @return El tipus del vèrtex segons la codificació de DirectX9.
  **/
  static unsigned int GetFVF();

  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static bool ActivateTextures(const vector<CTexture*>& _TextureArray);

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }

private:
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
};

/**
 * Estructura Vertex amb Normals i dues Textura.
 * Representa a un vertex amb coordenades geomètriques, normals i de dues textures.
**/
struct SNORMALTEXTURED2VERTEX
{
	float x, y, z;
  float nx, ny, nz;
  float u1, v1;
  float u2, v2;

  /**
   * Getter del tipus de vèrtex.
   * Els tipus de vèrtex és una màscara donada pel següent conjunt d'elements:
   *  - VERTEX_TYPE_GEOMETRY 0x0001
   *  - VERTEX_TYPE_NORMAL   0x0002
   *  - VERTEX_TYPE_TANGENT  0x0004
   *  - VERTEX_TYPE_BINORMAL 0x0008
   *  - VERTEX_TYPE_TEXTURE1 0x0010
   *  - VERTEX_TYPE_TEXTURE2 0x0020
   *  - VERTEX_TYPE_DIFFUSE  0x0040
   * @return El tipus del vèrtex.
  **/
  static unsigned short GetVertexType();

	/**
   * Getter del tipus de vèrtex.
   * @return El tipus del vèrtex segons la codificació de DirectX9.
  **/
  static unsigned int GetFVF();

  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static bool ActivateTextures(const vector<CTexture*>& _TextureArray);

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }

private:
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
};

struct TCAL3D_HW_VERTEX
{
  float x, y, z;
  float weights[4];
  float indices[4];
  float nx, ny, nz;
  float tu,tv;

  static unsigned short GetVertexType();

  static inline unsigned int GetFVF()
  {
    return 0;
  }

  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static bool ActivateTextures(const vector<CTexture*>& _TextureArray);

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
private:
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
};

#endif