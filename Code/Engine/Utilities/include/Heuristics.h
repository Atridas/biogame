#pragma once
#ifndef __HEURISTICS_H__
#define __HEURISTICS_H__

#include "GraphDefines.h"


class CHeuristic
{
public:
  virtual float Calculate(const CSparseGraph& G, int nd1, int nd2) const = 0;
};


//-----------------------------------------------------------------------------
//the euclidian heuristic (straight-line distance)
//-----------------------------------------------------------------------------
class CHeuristicEuclid:
  public CHeuristic
{
public:

  static CHeuristicEuclid instance;

  //calculate the straight line distance from node nd1 to node nd2
  virtual float Calculate(const CSparseGraph& G, int nd1, int nd2) const
  {
    return G.GetNode(nd1).GetPosition().Distance( G.GetNode(nd2).GetPosition());
  }
private:
  CHeuristicEuclid(){};
};

//-----------------------------------------------------------------------------
//this uses the euclidian distance but adds in an amount of noise to the 
//result. You can use this heuristic to provide imperfect paths. This can
//be handy if you find that you frequently have lots of agents all following
//each other in single file to get from one place to another
//-----------------------------------------------------------------------------
class CHeuristicNoisyEuclidian:
  public CHeuristic
{
public:

  static CHeuristicNoisyEuclidian instance;

  //calculate the straight line distance from node nd1 to node nd2
  virtual float Calculate(const CSparseGraph& G, int nd1, int nd2) const
  {
    return G.GetNode(nd1).GetPosition().Distance( G.GetNode(nd2).GetPosition()) * RandomNumber(0.9f, 1.1f);
  }
private:
  CHeuristicNoisyEuclidian(){};
};

//-----------------------------------------------------------------------------
//you can use this class to turn the A* algorithm into Dijkstra's search.
//this is because Dijkstra's is equivalent to an A* search using a heuristic
//value that is always equal to zero.
//-----------------------------------------------------------------------------
class CHeuristicDijkstra:
  public CHeuristic
{
public:

  static CHeuristicDijkstra instance;

  virtual float Calculate(const CSparseGraph& G, int nd1, int nd2) const
  {
    return 0;
  }
private:
  CHeuristicDijkstra(){};
};




CHeuristicEuclid& GetHeuristicEuclid();
CHeuristicNoisyEuclidian& GetHeuristicNoisyEuclid();
CHeuristicDijkstra& GetHeuristicDijkstra();

#endif