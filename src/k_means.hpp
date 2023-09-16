#ifndef KMEANS_HPP
#define KMEANS_HPP

#include <vector>
// class for K means classifier
//
// In scikit learn, K means has the following parameters:
//
//  n_clusters - the numbers of clusters to form
//
//  init -  the method to choose initial cluster point
//          Common are forgy, random partition, k-means++, Maximin. 
//
//  n_init - number of times the algorithm is ran with different centroid seeds
//  
//  max_iter - Maximum iterations in a single run.
//
//  tol - relative tolerance with regards to the frobenius norm of the difference in cluster centres of two 
//        consequctive iterations to declare convergence
//
//  verbose - verbosity mode
//
//  algorithm - There are different k means algorithms.
//              The common ones are lloyd and elkan. Lloyd is the most common one.


// For starting, I will just do support for n_clusters, forgy and classical lloyd K Means
class KMeans{
  std::vector<int> centroids;

  public:
  KMeans(std::size_t clusters, std::vector<int>& data, std::size_t iterations);
  void Initalise(std::vector<int>& data);
  void iterate(std::vector<int>& data);

  private:

};

#endif
