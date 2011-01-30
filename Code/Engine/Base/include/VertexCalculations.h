#pragma once
#ifndef __VERTEX_CALCULATIONS_H__

#include "base.h"

void CalcTangentsAndBinormals(void *VtxsData, uint16 *IdxsData, size_t VtxCount,
                              size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride,
                              size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride);

void CalcTangentsAndBinormalsBis( void *VtxsData, uint16 *IdxsData, size_t VtxCount,
                                  size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride,
                                  size_t TangentStride, size_t BitangentStride, size_t TextureCoordsStride);

#endif