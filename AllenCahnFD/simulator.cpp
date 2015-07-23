//
//  simulator.cpp
//  AllenCahnFD
//
//  Created by Yue Sun on 7/20/15.
//  Copyright (c) 2015 Yue Sun. All rights reserved.
//

#include "simulator.hpp"
#include "util.hpp"


template <typename T>
Simulator<T>::Simulator(const unsigned int nx,
                        const unsigned int ny,
                        const unsigned int nz,
                        const unsigned int ndim) : context(_context), device(_device), queue(_queue),
                                                   current_step(_current_step), time(_time), _data(NULL)

{
    _dim = {nx, ny, nz};
    _ndim = ndim;
    _size = nx*ny*nz;
    _current_step = 0;
    _time = 0;
}


template <typename T>
Simulator<T>::Simulator(const Dim3 dim,
                        const unsigned int ndim) : context(_context), device(_device), queue(_queue),
                                                   current_step(_current_step), time(_time), _data(NULL)
{
    _dim = dim;
    _ndim = ndim;
    _size = dim.x*dim.y*dim.z;
    _current_step = 0;
    _time = 0;
}


template <typename T>
Simulator<T>::~Simulator()
{
    if (_cl_initialized)
    {
        if (queue != 0)
            clReleaseCommandQueue(queue);
        
        if (device != 0)
            clReleaseDevice(device);
        
        if (context != 0)
            clReleaseContext(context);
    }
}



template <typename T>
cl_int Simulator<T>::init_cl(const cl_context & context_, const cl_device_id & device_, const cl_command_queue & queue_)
{
    if (device_==NULL) return CL_INVALID_DEVICE;
    _context=context_;
    if (context_==NULL)return CL_INVALID_CONTEXT;
    _device=device_;
    if (queue_==NULL)
    {
        cl_int err;
        CHECK_RETURN_N(_queue, clCreateCommandQueue(context_, device_, 0, &err), err);
    }
    else {_queue=queue_;}
    return CL_SUCCESS;
}

template <typename T>
cl_int Simulator<T>::init_cl()
{
    cl_int err;
    CHECK_RETURN_N(_context, CreateContext(err), err);
    CHECK_RETURN_N(_queue, CreateCommandQueue(_context, &_device, err), err)
    return CL_SUCCESS;
}


template <typename T>
cl_int Simulator<T>::init_cl(const cl_device_type & device_type, const unsigned int & device_num)
{
    cl_int err;
    CHECK_RETURN_N(_context, CreateContext(_device, device_type, device_num, err), err);
    CHECK_RETURN_N(_queue, CreateCommandQueue(_context, &_device, err), err);
    return CL_SUCCESS;
}


template <typename T>
void Simulator<T>::step(T dt)
{
    _current_step++;
    _time+=dt;
}


template <typename T>
void Simulator<T>::steps(const T dt, const unsigned int nsteps, const bool finish, const bool cputime)
{
    _current_step+=nsteps;
    _time+=dt*nsteps;
}


template <typename T>
void Simulator<T>:: writefile(const std::string & filename)
{
    if (filename=="")
        write2bin(time2fname("out_", current_step), _data, _size);
    else
        write2bin(filename, _data, _size);
}



template class Simulator<float>;
template class Simulator<double>;
