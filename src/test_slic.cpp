//
//  test_slic.cpp
//  slic
//
//  Created by David on 10/6/19.
//  Copyright (c) 2019 David Dominguez. All rights reserved.
//
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

      std::cout<<"image:  "<<input<<"\n";
      std::cout<<"result: "<<result<<"\n";
      std::cout<<"superpixels: "<<numsp<<"\n";
      
      png::image< png::rgb_pixel > image(input);
  
      SLIC slic(image,numsp,40.0);
      slic.computeSuperpixeles();
      slic.displayContours(result);
    }
  }
  return 0;
}
