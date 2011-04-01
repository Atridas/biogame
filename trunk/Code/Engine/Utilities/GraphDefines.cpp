#include "GraphDefines.h"

//--------------------------- ValidNeighbour -----------------------------
//
//  returns true if x,y is a valid position in the map
//------------------------------------------------------------------------
bool ValidNeighbour(int x, int y, int NumCellsX, int NumCellsY)
{
  return !((x < 0) || (x >= NumCellsX) || (y < 0) || (y >= NumCellsY));
}
  
//------------ GraphHelper_AddAllNeighboursToGridNode ------------------
//
//  use to add he eight neighboring edges of a graph node that 
//  is positioned in a grid layout
//------------------------------------------------------------------------
void GraphHelper_AddAllNeighboursToGridNode(CSparseGraph& graph,
                                            int         row,
                                            int         col,
                                            int         NumCellsX,
                                            int         NumCellsY)
{   
  for (int i=-1; i<2; ++i)
  {
    for (int j=-1; j<2; ++j)
    {
      int nodeX = col+j;
      int nodeY = row+i;

      //skip if equal to this node
      if ( (i == 0) && (j==0) ) continue;

      //check to see if this is a valid neighbour
      if (ValidNeighbour(nodeX, nodeY, NumCellsX, NumCellsY))
      {
        //calculate the distance to this node
        Vect3f PosNode      = graph.GetNode(row*NumCellsX+col).GetPosition();
        Vect3f PosNeighbour = graph.GetNode(nodeY*NumCellsX+nodeX).GetPosition();

        float dist = PosNode.Distance(PosNeighbour);

        //this neighbour is okay so it can be added
        CGraphEdge NewEdge(row*NumCellsX+col,
                           nodeY*NumCellsX+nodeX,
                           dist);
        graph.AddEdge(NewEdge);

        //if graph is not a diagraph then an edge needs to be added going
        //in the other direction
        if (!graph.isDigraph())
        {
          CGraphEdge NewEdge(nodeY*NumCellsX+nodeX,
                             row*NumCellsX+col,
                             dist);
          graph.AddEdge(NewEdge);
        }
      }
    }
  }
}


//--------------------------- GraphHelper_CreateGrid --------------------------
//
//  creates a graph based on a grid layout. This function requires the 
//  dimensions of the environment and the number of cells required horizontally
//  and vertically 
//-----------------------------------------------------------------------------
void GraphHelper_CreateGrid(CSparseGraph& graph,
                             int cySize,
                             int cxSize,
                             int NumCellsY,
                             int NumCellsX)
{ 
  //need some temporaries to help calculate each node center
  float CellWidth  = (float)cySize / (float)NumCellsX;
  float CellHeight = (float)cxSize / (float)NumCellsY;

  float midX = CellWidth/2;
  float midY = CellHeight/2;

  
  //first create all the nodes
  for (int row=0; row<NumCellsY; ++row)
  {
    for (int col=0; col<NumCellsX; ++col)
    {
      graph.AddNode(CGraphNode(graph.GetNextFreeNodeIndex(),
                               Vect3f(midX + (col*CellWidth),
                               0.f,
                               midY + (row*CellHeight))));

    }
  }
  //now to calculate the edges. (A position in a 2d array [x][y] is the
  //same as [y*NumCellsX + x] in a 1d array). Each cell has up to eight
  //neighbours.
  for (int row=0; row<NumCellsY; ++row)
  {
    for (int col=0; col<NumCellsX; ++col)
    {
      GraphHelper_AddAllNeighboursToGridNode(graph, row, col, NumCellsX, NumCellsY);
    }
  }
}  