//
//  colorspace.cpp
//  colorspace
//
//  Created by David on 10/6/19.
//  Copyright (c) 2019 David Dominguez. All rights reserved.
//

#include <cmath>
#include "../include/colorspaces.hpp"

ColorSpaces::ColorSpaces(){}
ColorSpaces::~ColorSpaces(){}

void ColorSpaces::rgb2hsv(double *rin, double *gin, double *bin, int sz, double *hvec, double *svec, double *vvec){

  int i = 0;
  double fCMax = 0.0;
  double fCMin = 0.0;
  double fDelta = 0.0;
  
  for(i = 0; i < sz; i++){
    fCMax = max(max(rin[i], gin[i]), bin[i]);
    fCMin = min(min(rin[i], gin[i]), bin[i]);
    fDelta = fCMax - fCMin;
    
    if(fDelta > 0) {
      
      if(fCMax == rin[i])
	hvec[i] = 60.0 * (fmod(((gin[i] - bin[i]) / fDelta), 6.0));
      
      else if(fCMax == gin[i])
	hvec[i] = 60.0 * (((bin[i] - rin[i]) / fDelta) + 2.0);
	  
      else if(fCMax == bin[i]) 
	hvec[i] = 60.0 * (((rin[i] - gin[i]) / fDelta) + 4.0);
	  
      if(fCMax > 0)
	svec[i] = fDelta / fCMax;
      else 
	svec[i] = 0.0;
      
      vvec[i] = fCMax;
    } else {
      hvec[i] = 0.0;
      svec[i] = 0.0;
      vvec[i]= fCMax;
    }
    
    if(hvec[i] < 0) {
      hvec[i] = 360.0 + hvec[i];
    }
  }
}

void ColorSpaces::rgb2ycbcr(double *rin, double *gin, double *bin, int sz, double *yvec, double *cbvec, double *crvec){

  double delta = 128.0;
  int i = 0;
  
  for(i = 0; i < sz; i++){
        yvec[i]  = 0.299 * rin[i] + 0.587 * gin[i] + 0.114 * bin[i];
        cbvec[i] = (rin[i] - yvec[i]) * 0.713 + delta;              
        crvec[i] = (bin[i] - yvec[i]) * 0.564 + delta;
    }
    
}

void ColorSpaces::rgb2lab(double *rin, double *gin, double *bin, int sz, double *lvec, double *avec, double *bvec) {

  double R,G,B;
  double X,Y,Z;
  double r, g, b;
  const double epsilon = 0.008856;  /* actual CIE standard */
  const double kappa   = 903.3;	    /* actual CIE standard */
  
  const double Xr = 0.950456;	    /* reference white */
  const double Yr = 1.0;	    /* reference white */
  const double Zr = 1.088754;	    /* reference white */
  double xr,yr,zr;
  double fx, fy, fz;
  double lval,aval,bval;
  int i;

  for(i = 0; i < sz; i++) {

    R = rin[i]/255.0;
    G = gin[i]/255.0;
    B = bin[i]/255.0;

    if(R <= 0.04045)
      r = R/12.92;
    else
      r = pow((R+0.055)/1.055,2.4);
    
    if(G <= 0.04045)
      g = G/12.92;
    else
      g = pow((G+0.055)/1.055,2.4);
    
    if(B <= 0.04045)
      b = B/12.92;
    else
      b = pow((B+0.055)/1.055,2.4);
    
    X = r * 0.4124564 + g * 0.3575761 + b * 0.1804375;
    Y = r * 0.2126729 + g * 0.7151522 + b * 0.0721750;
    Z = r * 0.0193339 + g * 0.1191920 + b * 0.9503041;
    
    xr = X/Xr;
    yr = Y/Yr;
    zr = Z/Zr;
    
    if(xr > epsilon)
      fx = pow(xr, 1.0/3.0);
    else
      fx = (kappa * xr + 16.0)/116.0;
    
    if(yr > epsilon)
      fy = pow(yr, 1.0/3.0);
    else
      fy = (kappa * yr + 16.0)/116.0;
    
    if(zr > epsilon)
      fz = pow(zr, 1.0/3.0);
    else
      fz = (kappa * zr + 16.0)/116.0;
    
    lval = 116.0 * fy-16.0;
    aval = 500.0 * (fx-fy);
    bval = 200.0 * (fy-fz);
    
    lvec[i] = lval; avec[i] = aval; bvec[i] = bval;
  }
}
