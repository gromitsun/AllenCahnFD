//
//  randn.h
//  AllenCahnFD
//
//  Created by Yue Sun on 7/21/15.
//  Copyright (c) 2015 Yue Sun. All rights reserved.
//

#ifndef __AllenCahnFD__randn__
#define __AllenCahnFD__randn__


template<typename T>
void gauss(T * arr, unsigned int n, T mean, T sigma);

template void gauss<float>(float * arr, unsigned int n, float mean, float sigma);
template void gauss<double>(double * arr, unsigned int n, double mean, double sigma);

#endif /* defined(__AllenCahnFD__randn__) */
