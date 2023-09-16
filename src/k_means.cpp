#include "k_means.hpp"

KMeans(std::size_t clusters, std::vector<int>& data){
  centroids.reserve(clusters);
  Initialise(data);
}


