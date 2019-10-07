//
//  slic.hpp
//  slic
//
//  Created by David on 10/6/19.
//  Copyright (c) 2019 David Dominguez. All rights reserved.
//

#if !defined(_SLIC_HPP_)
#define _SLIC_HPP_

#include <png++/png.hpp>


class SLIC{
private:
  int MAX_ITER;
  int width;
  int height;
  int size;
  int step;
  int numseeds;
  int finalNumberOfLabels;

  /* structs */

  double *rin;
  double *gin;
  double *bin;
  
  double *lvec;
  double *avec;
  double *bvec;
  
  int *seedIndices;
  int *klabels;
  int *clabels;

  int **outlabels;
  
  /* image feature */
  double *kseedsx;
  double *kseedsy;
  double *kseedsl;
  double *kseedsa;
  double *kseedsb;

  void getSeeds();
  void performSuperpixel();
  void enforceSuperpixelConnectivity();
  
public:
  int numsp; /* num superpixels */
  double m; /* compactness */
  png::image< png::rgb_pixel > input;
  
  SLIC(png::image< png::rgb_pixel > image,int superpixels,double compactness);
  void displayContours(std::string filename);
  void computeSuperpixeles();
  ~SLIC();
  
};

#endif
