/**
 * Copyright (c) 2019 David Dominguez. All rights reserved.
 * david.dominguez1603@gmail.com
 * Created on 10/6/19.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
