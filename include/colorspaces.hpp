//
//  colorspaces.hpp
//  colorspace
//
//  Created by David on 10/6/19.
//  Copyright (c) 2019 David Dominguez. All rights reserved.
//

#if !defined(_COLORSPACES_HPP_)
#define _COLORSPACES_HPP_

template <class T>
T min(T a, T b){
  return (a < b) ? a : b; 
}

template <class T>
T max(T a, T b){
  return (a > b) ? a : b; 
}

class ColorSpaces{
public:

  ColorSpaces();
  ~ColorSpaces();
  
  void rgb2hsv(double *rin, double *gin, double *bin, int sz, double *hvec, double *svec, double *vvec);
  void rgb2ycbcr(double *rin, double *gin, double *bin, int sz, double *yvec, double *cbvec, double *crvec);
  void rgb2lab(double *rin, double *gin, double *bin, int sz, double *lvec, double *avec, double *bvec);
};

#endif
