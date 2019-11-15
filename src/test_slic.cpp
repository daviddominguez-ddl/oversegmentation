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

#include <iostream>
#include <png++/png.hpp>
#include "../include/utilities.hpp"
#include "../include/slic.hpp"

int main(int argc,char *argv[]){

  std::string input,result;
  int numsp;
  
  if( argc == 1 ){
    std::cout<<"No input files.\n";
  }else{
    int i=0;
    bool arg_in = false, arg_out = false, arg_sp = false;
    for(;i < argc; i++){
      if(strcmp(argv[i],"-i") == 0) {arg_in  = true;input  = argv[i+1];}
      if(strcmp(argv[i],"-o") == 0) {arg_out = true;result = argv[i+1];}
      if(strcmp(argv[i],"-s") == 0) {arg_sp  = true;numsp  = atoi(argv[i+1]);}
    }
    if(arg_in && arg_out && arg_sp){

      std::cout<<"Image:  "<<input<<"\n";
      std::cout<<"Result: "<<result<<"\n";
      std::cout<<"Superpixels: "<<numsp<<"\n";
      
      png::image< png::rgb_pixel > image(input);
  
      SLIC slic(image,numsp,40.0); /* input image , num superpixels and compactness*/
      slic.computeSuperpixeles();
      slic.displayContours(result);
    }
  }
  return 0;
}
