//
//  slic.cpp
//  slic
//
//  Created on 10/6/19.
//  Implementation of SLIC algorithm, proposed by  Radhakrishna Achanta et al. (2010)
//

#include <iostream>
#include <cmath>
#include <cfloat>
#include <png++/png.hpp>
#include "../include/colorspaces.hpp"
#include "../include/utilities.hpp"
#include "../include/slic.hpp"

SLIC::SLIC(png::image< png::rgb_pixel > image, int superpixels,double compactness){
  numsp = superpixels;
  m = compactness;
  input = image;
  MAX_ITER = 10;
  int i = 0;
  png::uint_32 x = 0,y = 0;
  
  width = input.get_width();
  height = input.get_height();
  size = width * height;

  step = sqrt((double)(size)/(double)(superpixels))+0.5;
  
  rin = Vector <double>(size);
  gin = Vector <double>(size);
  bin = Vector <double>(size);
  
  lvec = Vector <double>(size);
  avec = Vector <double>(size);
  bvec = Vector <double>(size);

  seedIndices = Vector <int>(size);
  klabels = Vector <int>(size);
  clabels = Vector <int>(size);
  
  outlabels = Mat <int>(height,width);
  
  for (y = 0; y < height; ++y){
    for (x = 0; x < width; ++x){
      rin[i] = (int)image[y][x].red;
      gin[i] = (int)image[y][x].green;
      bin[i] = (int)image[y][x].blue;
      i++;
    }
  }
  
}

SLIC::~SLIC(){

  delete rin;
  delete gin;
  delete bin;
  
  delete lvec;
  delete avec;
  delete bvec;
  
  delete seedIndices;
  delete klabels;
  delete clabels;

  for (int i = 0; i < height; i++)
    delete outlabels[i]; 
  delete outlabels;

  delete kseedsx;
  delete kseedsy;
  delete kseedsl;
  delete kseedsa;
  delete kseedsb;
 
}
void SLIC::computeSuperpixeles(){
  
  /* RGB to LAB */
  ColorSpaces cs;
  int i,j;
  /* compute superpixels using CIELAB color space */
   cs.rgb2lab(rin,gin,bin,size,lvec,avec,bvec);

  /* compute superpixels using HSV color space */
  //cs.rgb2hsv(rin,gin,bin,size,lvec,avec,bvec);
  
  /* compute superpixels using YCbCr color space */
  // cs.rgb2ycbcr(rin,gin,bin,size,lvec,avec,bvec);
  
  getSeeds();
  performSuperpixel();
  enforceSuperpixelConnectivity();
  
  for(i = 0; i < height; i++)
    for(j = 0; j < width ; j++)
      outlabels[i][j] = clabels[j + (i * width)];
}

void SLIC::getSeeds(){
  bool hexgrid = false;
  int xstrips, ystrips;
  int xerr, yerr;
  double xerrperstrip,yerrperstrip;
  int xoff,yoff;
  int x,y;
  int xe,ye;
  int seedx,seedy;
  int i,n;
  
  xstrips = (0.5+(double)(width)/(double)(step));
  ystrips = (0.5+(double)(height)/(double)(step));
  
  xerr = width  - step*xstrips;if(xerr < 0){xstrips--;xerr = width - step*xstrips;}
  yerr = height - step*ystrips;if(yerr < 0){ystrips--;yerr = height- step*ystrips;}
  
  xerrperstrip = (double)(xerr)/(double)(xstrips);
  yerrperstrip = (double)(yerr)/(double)(ystrips);
  
  xoff = step/2;
  yoff = step/2;
  
  n = 0;
  for( y = 0; y < ystrips; y++ )
    {
      ye = y*yerrperstrip;
      for( x = 0; x < xstrips; x++ )
	{
	  xe = x*xerrperstrip;
	  seedx = (x*step+xoff+xe);
	  if(hexgrid){ seedx = x*step+(xoff<<(y&0x1))+xe; if(seedx >= width)seedx = width-1; }
	  seedy = (y*step+yoff+ye);
	  i = seedy*width + seedx;
	  seedIndices[n] = i;
	  n++;
	}
    }
  numseeds = n;
  
  kseedsx    = Vector <double>(numseeds);
  kseedsy    = Vector <double>(numseeds);
  kseedsl    = Vector <double>(numseeds);
  kseedsa    = Vector <double>(numseeds);
  kseedsb    = Vector <double>(numseeds);

  for(i = 0; i < numseeds; i++)
    {
      kseedsx[i] = seedIndices[i]%width;
      kseedsy[i] = seedIndices[i]/width;
      kseedsl[i] = lvec[seedIndices[i]];
      kseedsa[i] = avec[seedIndices[i]];
      kseedsb[i] = bvec[seedIndices[i]];
    }
  
}

void SLIC::performSuperpixel(){
  int x1, y1, x2, y2;
  double l, a, b;
  double dist;
  double distxy;
  int itr;
  int n;
  int x,y;
  int i;
  int ind;
  int r,c;
  int k;
  const int numk = numseeds;
  int offset = step;
  
  double *clustersize = Vector <double>(numk); 
  double *inv         = Vector <double>(numk);
  double *sigmal      = Vector <double>(numk);
  double *sigmaa      = Vector <double>(numk);
  double *sigmab      = Vector <double>(numk);
  double *sigmax      = Vector <double>(numk);
  double *sigmay      = Vector <double>(numk);
  double *distvec     = Vector <double>(size);
  
  double invwt = 1.0/((step/m)*(step/m));
  
  
  for( itr = 0; itr < MAX_ITER; itr++ )
    {
      for(i = 0; i < size; i++){distvec[i] = DBL_MAX;} // set distances_i <- double_max_value
      
      for( n = 0; n < numk; n++ )
        {
	  x1 = kseedsx[n]-offset; if(x1 < 0) x1 = 0;
	  y1 = kseedsy[n]-offset; if(y1 < 0) y1 = 0;
	  x2 = kseedsx[n]+offset; if(x2 > width)  x2 = width;
	  y2 = kseedsy[n]+offset; if(y2 > height) y2 = height;
          
	  for( y = y1; y < y2; y++ )
            {
	      for( x = x1; x < x2; x++ )
                {
		  i = y*width + x;
                  
		  l = lvec[i];
		  a = avec[i];
		  b = bvec[i];
                  
		  dist = (l - kseedsl[n])*(l - kseedsl[n]) +
                    (a - kseedsa[n])*(a - kseedsa[n]) +
                    (b - kseedsb[n])*(b - kseedsb[n]) ;
		  
		  distxy = (x - kseedsx[n])*(x - kseedsx[n]) +
                    (y - kseedsy[n])*(y - kseedsy[n]) ;
		  
		  dist += distxy*invwt;
                  
		  if(dist < distvec[i])
                    {
		      distvec[i] = dist;
		      klabels[i]  = n;
                    }
                }
            }
        }
      //
      // Recalculate the centroid and store in the seed values
      //
      for(k = 0; k < numk; k++)
        {
	  sigmal[k] = 0;
	  sigmaa[k] = 0;
	  sigmab[k] = 0;
	  sigmax[k] = 0;
	  sigmay[k] = 0;
	  clustersize[k] = 0;
        }
      
      ind = 0;
      for( r = 0; r < height; r++ )
        {
	  for( c = 0; c < width; c++ )
            {
	      if(klabels[ind] >= 0)
                {
		  sigmal[klabels[ind]] += lvec[ind];
		  sigmaa[klabels[ind]] += avec[ind];
		  sigmab[klabels[ind]] += bvec[ind];
		  sigmax[klabels[ind]] += c;
		  sigmay[klabels[ind]] += r;
		  clustersize[klabels[ind]] += 1.0;
                }
	      ind++;
            }
        }
      
      {for( k = 0; k < numk; k++ )
	  {
            if( clustersize[k] <= 0 ) clustersize[k] = 1;
            inv[k] = 1.0/clustersize[k];
	  }}
        
      {for( k = 0; k < numk; k++ )
	  {
            kseedsl[k] = sigmal[k]*inv[k];
            kseedsa[k] = sigmaa[k]*inv[k];
            kseedsb[k] = sigmab[k]*inv[k];
            kseedsx[k] = sigmax[k]*inv[k];
            kseedsy[k] = sigmay[k]*inv[k];
	  }}
      
    }
  delete sigmal;
  delete sigmaa;
  delete sigmab;
  delete sigmax;
  delete sigmay;
  delete clustersize;
  delete inv;
  delete distvec;
}

void SLIC::enforceSuperpixelConnectivity() {
  
  int i,j,k;
  int n,c,count;
  int x,y;
  int ind;
  int oindex, adjlabel;
  int label;
  const int dx4[4] = {-1,  0,  1,  0};
  const int dy4[4] = { 0, -1,  0,  1};
  const int SUPSZ = size/numsp;

  int *xvec = Vector <int>(SUPSZ*10);
  int *yvec = Vector <int>(SUPSZ*10);

  
  for( i = 0; i < size; i++ ) clabels[i] = -1;
  oindex = 0;
  adjlabel = 0;
  label = 0;
  for( j 	= 0; j < height; j++ ){
    for( k = 0; k < width; k++ ){
      if( 0 > clabels[oindex] ){
	clabels[oindex] = label;
	xvec[0] = k;
	yvec[0] = j;
	{for( n = 0; n < 4; n++ ){
	    int x = xvec[0] + dx4[n];
	    int y = yvec[0] + dy4[n];
	    if( (x >= 0 && x < width) && (y >= 0 && y < height) ){
	      int nindex = y*width + x;
	      if(clabels[nindex] >= 0) adjlabel = clabels[nindex];
	    }
	  }}
	
	count = 1;
	for( c = 0; c < count; c++ ){
	  for( n = 0; n < 4; n++ ){
	    x = xvec[c] + dx4[n];
	    y = yvec[c] + dy4[n];
	    
	    if( (x >= 0 && x < width) && (y >= 0 && y < height) ){
	      int nindex = y*width + x;
	      
	      if( 0 > clabels[nindex] && klabels[oindex] == klabels[nindex] ){
		xvec[count] = x;
		yvec[count] = y;
		clabels[nindex] = label;
		count++;
	      }
	    }
	    
	  }
	}
	      
	if(count <= SUPSZ >> 2){
	  for( c = 0; c < count; c++ ){
	    ind = yvec[c]*width+xvec[c];
	    clabels[ind] = adjlabel;
	  }
	  label--;
	}
	label++;
      }
      oindex++;
    }
  }
  finalNumberOfLabels = label;
  
  delete xvec;
  delete yvec;
}

void SLIC::displayContours(std::string filename){
  
  const int dx8[8] = {-1, -1,  0,  1, 1, 1, 0, -1};
  const int dy8[8] = { 0, -1, -1, -1, 0, 1, 1,  1};
  const int dx4[4] = { 0, -1, 0, 1};
  const int dy4[4] = {-1,  0, 1, 0};
  
  int i = 0, j = 0, cc = 0, rr = 0, index;
  int ROWS = height;
  int COLS = width; 
  int contours_color[3] = {109,210,202};
  png::uint_32 x,y;
  
  bool **istaken = Mat <bool>(ROWS,COLS);
  int  **contour = Mat <int>(size,2);
  
  for(i = 0; i < ROWS; i++){
    for(j = 0; j < COLS; j++){
      istaken[i][j] = false;
    }
  }

  for(i = 0; i < size; i++){
    for(j = 0; j < 2; j++){
      contour[i][j] = 0;
    }
  }

  for (i = 0; i < ROWS; i++){
    for (j = 0; j < COLS; j++){
      int nr_p = 0;
      
      for (int k = 0; k < 8; k++){
	
	int x = j + dx8[k], y = i + dy8[k];
	if ( y >= 0 && y < ROWS && x >= 0 && x < COLS ){
	  if (istaken[y][x] == false && outlabels[i][j] != outlabels[y][x]){
	    nr_p += 1;
	  }
	}
      }

      if (nr_p >= 2){
	contour[index][0] = j;
	contour[index][1] = i;
	istaken[i][j] = true;
	index++;
      }
    }
  }

  // write result
  
  for (i = 0; i < index; i++){

    x = contour[i][0];
    y = contour[i][1];

    input[y][x].red   = contours_color[0];
    input[y][x].green = contours_color[1];
    input[y][x].blue  = contours_color[2];
    
    for(int k = 0; k < 4; k++){
      
      cc = x + dx4[k];
      rr = y + dy4[k];
      
      if ( rr >= 0 && rr < ROWS && cc >= 0 && cc < COLS ){
	input[rr][cc].red   = contours_color[0];
	input[rr][cc].green = contours_color[1];
	input[rr][cc].blue  = contours_color[2];
      }
    }
  }
  
  input.write(filename);
    
  for (i = 0; i < ROWS;i++)
    delete istaken[i];
  delete istaken;
  
  for(i = 0; i < size; i++)
    delete contour[i];
  delete contour;
}
