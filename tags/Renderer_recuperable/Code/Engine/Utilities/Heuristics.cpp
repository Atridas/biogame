#include "Heuristics.h"

CHeuristicEuclid CHeuristicEuclid::instance;
CHeuristicNoisyEuclidian CHeuristicNoisyEuclidian::instance;
CHeuristicDijkstra CHeuristicDijkstra::instance;

CHeuristicEuclid& GetHeuristicEuclid()
{
  return CHeuristicEuclid::instance;
}
CHeuristicNoisyEuclidian& GetHeuristicNoisyEuclid()
{
  return CHeuristicNoisyEuclidian::instance;
}
CHeuristicDijkstra& GetHeuristicDijkstra()
{
  return CHeuristicDijkstra::instance;
}