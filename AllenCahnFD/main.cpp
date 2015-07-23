//
//  main.cpp
//  AllenCahnFD
//
//  Created by Yue Sun on 7/17/15.
//  Copyright (c) 2015 Yue Sun. All rights reserved.
//

#include <iostream>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include "simulator_2d.hpp"

int main(int argc, const char * argv[])
{
    // insert code here...
    std::cout << "Hello, World!\n";
    
    Simulator_2D<float> sim{};
    
    // Read input parameters
    if (argc > 1)
        sim.read_input(argv[1]);
    else
    {
        std::cerr << "ERROR: No input file specified!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    sim.init_cl(CL_DEVICE_TYPE_GPU, 1);
    
    sim.build_kernel("/Users/yue/Dropbox/Documents/Work/OpenCL/AllenCahnFD/AllenCahnFD/kernel_float.cl");
    
    sim.init_sim(0, 0.001);
    
    sim.run();
    
    return 0;
}
