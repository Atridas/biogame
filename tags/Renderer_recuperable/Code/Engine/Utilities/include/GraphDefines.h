#pragma once
#ifndef __GRAPH_DEFINES_H__
#define __GRAPH_DEFINES_H__

#define INVALID_GRAPH_NODE_INDEX -1


#include "GraphNode.h"
#include "GraphEdge.h"
#include "SparseGraph.h"

bool ValidNeighbour(int x, int y, int NumCellsX, int NumCellsY);

void GraphHelper_AddAllNeighboursToGridNode(CSparseGraph& graph,
                                            int           row,
                                            int           col,
                                            int           NumCellsX,
                                            int           NumCellsZ);

void GraphHelper_CreateGrid(CSparseGraph& graph,
                             int czSize,
                             int cxSize,
                             int NumCellsZ,
                             int NumCellsX);
#endif