//
//  randn.cpp
//  AllenCahnFD
//
//  Created by Yue Sun on 7/21/15.
//  Copyright (c) 2015 Yue Sun. All rights reserved.
//

#include <random>

#include "randn.hpp"

template<typename T>
void gauss(T * arr, unsigned int n, T mean, T sigma)
{
    //initialize random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    std::normal_distribution<T> normdist(mean,sigma);

    for (unsigned int i=0; i < n; i++)
        arr[i] = normdist(gen);
}