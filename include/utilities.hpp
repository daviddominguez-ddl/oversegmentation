//
//  utilities.hpp
//  utilities
//
//  Created by David on 10/6/19.
//  Copyright (c) 2019 David Dominguez. All rights reserved.
//

#if !defined(_UTILITIES_HPP_)
#define _UTILITIES_HPP_

template <class T>
T *Vector(int size){
  T *p = 0;
  try{
    p = new T[size];
  }
  catch (std::bad_alloc){ exit(-1); }
  return p;
}

template <class T>
T **Mat(int rows,int cols){
  T **m = 0;
  try{
    m = new T*[rows];
    for(int i = 0; i < rows; i++)
      m[i] = new T[cols];
  }

  catch (std::bad_alloc){ exit(-1); }
  
  return m;
}

#endif
