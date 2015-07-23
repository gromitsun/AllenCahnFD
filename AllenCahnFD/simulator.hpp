//
//  simulator.h
//  AllenCahnFD
//
//  Created by Yue Sun on 7/20/15.
//  Copyright (c) 2015 Yue Sun. All rights reserved.
//

#ifndef __AllenCahnFD__simulator__
#define __AllenCahnFD__simulator__

#include <iostream>
#include "cl_common.hpp"

typedef struct
{
    unsigned int x;
    unsigned int y;
    unsigned int z;
}Dim3;

template <typename T>
class Simulator
{
protected:
    cl_context          _context;
    cl_device_id        _device;    
    cl_command_queue    _queue;
    
    Dim3 _dim;
    unsigned int _ndim;
    unsigned int _size;
    
    T * _data;
    unsigned int _current_step;
    unsigned int _time;
    
    bool _cl_initialized;
    bool _sim_initialized;
    
public:
    /* Variables */
    // OpenCL
    const cl_context          & context;
    const cl_device_id        & device;
    const cl_command_queue    & queue;
    // Simulation
//    const T * const data;
    const unsigned int & current_step;
    const unsigned int & time;
    
    /* Constructors & destructors */
    Simulator(const unsigned int nx,
              const unsigned int ny,
              const unsigned int nz,
              const unsigned int ndim);
    Simulator(const Dim3 dim,
              const unsigned int ndim);
    virtual ~Simulator();
    
    /* Functions */
    // OpenCL
    virtual cl_int  init_cl(const cl_context & context_, const cl_device_id & device_, const cl_command_queue & queue_=NULL);
    virtual cl_int  init_cl();
    virtual cl_int  init_cl(const cl_device_type & device_type, const unsigned int & device_num);
    virtual cl_int  build_kernel(const char * filename) {return CL_SUCCESS;};
    virtual void    finishQueue() {if(_queue)clFinish(_queue);}
    
    // write array from host to device
    cl_int WriteArray(const cl_mem & image, const T* orig, size_t x, size_t y, size_t z=1)
    {
        size_t offset[3]={0,0,0};
        size_t size[3]={x,y,z};
        clEnqueueWriteImage(_queue, image, true, offset, size, 0, 0, orig, 0, NULL, NULL);
        return CL_SUCCESS;
    }
    
    // read array from device to host
    cl_int ReadArray(const cl_mem & image, T* dest, size_t x, size_t y, size_t z=1)
    {
        size_t offset[3]={0,0,0};
        size_t size[3]={x,y,z};
        clEnqueueReadImage(_queue, image, true, offset, size, 0, 0, dest, 0, NULL, NULL);
        return CL_SUCCESS;
    };
    
    // simulation
    virtual void init_sim() {};
    virtual void step(const T dt);
    virtual void steps(const T dt,const unsigned int nsteps,const bool finish=true, const bool cputime=true);
    
    // output
    virtual void writefile(const std::string & filename="");
    
};

#endif /* defined(__AllenCahnFD__simulator__) */
