#include <iostream>
#include <string>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#ifdef __APPLE__
#include <mach/mach_time.h>
#else
#include <ctime>
#endif


#include "cl_common.hpp"



/********************************************************************************/

/* Get CPU time */
ttime_t getTime()
{
#ifdef __APPLE__
    return mach_absolute_time();
#else
    return std::clock();
#endif
}

double subtractTimes(const ttime_t endTime, const ttime_t startTime )
{
#ifdef __APPLE__
    ttime_t difference = endTime - startTime;
    static double conversion = 0.0;
    
    if( conversion == 0.0 )
    {
        mach_timebase_info_data_t info;
        kern_return_t err = mach_timebase_info( &info );
        
        //Convert the timebase into seconds
        if( err == 0  )
            conversion = 1e-9 * (double) info.numer / (double) info.denom;
    }
    
    return conversion * (double) difference;
#else
    return (endTime-startTime)/(double)CLOCKS_PER_SEC;
#endif
}

/********************************************************************************/

/* Error checking */
const char *get_error_string(const cl_int errNum)
{
    switch (errNum)
    {
        // run-time and JIT compiler errors
        case 0: return "CL_SUCCESS";
        case -1: return "CL_DEVICE_NOT_FOUND";
        case -2: return "CL_DEVICE_NOT_AVAILABLE";
        case -3: return "CL_COMPILER_NOT_AVAILABLE";
        case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
        case -5: return "CL_OUT_OF_RESOURCES";
        case -6: return "CL_OUT_OF_HOST_MEMORY";
        case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
        case -8: return "CL_MEM_COPY_OVERLAP";
        case -9: return "CL_IMAGE_FORMAT_MISMATCH";
        case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
        case -11: return "CL_BUILD_PROGRAM_FAILURE";
        case -12: return "CL_MAP_FAILURE";
        case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
        case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
        case -15: return "CL_COMPILE_PROGRAM_FAILURE";
        case -16: return "CL_LINKER_NOT_AVAILABLE";
        case -17: return "CL_LINK_PROGRAM_FAILURE";
        case -18: return "CL_DEVICE_PARTITION_FAILED";
        case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
            
        // compile-time errors
        case -30: return "CL_INVALID_VALUE";
        case -31: return "CL_INVALID_DEVICE_TYPE";
        case -32: return "CL_INVALID_PLATFORM";
        case -33: return "CL_INVALID_DEVICE";
        case -34: return "CL_INVALID_CONTEXT";
        case -35: return "CL_INVALID_QUEUE_PROPERTIES";
        case -36: return "CL_INVALID_COMMAND_QUEUE";
        case -37: return "CL_INVALID_HOST_PTR";
        case -38: return "CL_INVALID_MEM_OBJECT";
        case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
        case -40: return "CL_INVALID_IMAGE_SIZE";
        case -41: return "CL_INVALID_SAMPLER";
        case -42: return "CL_INVALID_BINARY";
        case -43: return "CL_INVALID_BUILD_OPTIONS";
        case -44: return "CL_INVALID_PROGRAM";
        case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
        case -46: return "CL_INVALID_KERNEL_NAME";
        case -47: return "CL_INVALID_KERNEL_DEFINITION";
        case -48: return "CL_INVALID_KERNEL";
        case -49: return "CL_INVALID_ARG_INDEX";
        case -50: return "CL_INVALID_ARG_VALUE";
        case -51: return "CL_INVALID_ARG_SIZE";
        case -52: return "CL_INVALID_KERNEL_ARGS";
        case -53: return "CL_INVALID_WORK_DIMENSION";
        case -54: return "CL_INVALID_WORK_GROUP_SIZE";
        case -55: return "CL_INVALID_WORK_ITEM_SIZE";
        case -56: return "CL_INVALID_GLOBAL_OFFSET";
        case -57: return "CL_INVALID_EVENT_WAIT_LIST";
        case -58: return "CL_INVALID_EVENT";
        case -59: return "CL_INVALID_OPERATION";
        case -60: return "CL_INVALID_GL_OBJECT";
        case -61: return "CL_INVALID_BUFFER_SIZE";
        case -62: return "CL_INVALID_MIP_LEVEL";
        case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
        case -64: return "CL_INVALID_PROPERTY";
        case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
        case -66: return "CL_INVALID_COMPILER_OPTIONS";
        case -67: return "CL_INVALID_LINKER_OPTIONS";
        case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";
            
        // extension errors
        case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
        case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
        case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
        case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
        case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
        case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
        default: return "Unknown OpenCL error";
    }
}

/********************************************************************************/

/* Get device information */
// Covert cl_device_info to string
const char * device_info_to_str(const cl_device_info param_name)
{
    switch(param_name)
    {
        case CL_DEVICE_NAME: return "CL_DEVICE_NAME";
        default: return "";
    }
}


const char * device_type_string(const cl_device_type type)
{
    if(type==CL_DEVICE_TYPE_CPU)
        return "CL_DEVICE_TYPE_CPU";
    else if(type==CL_DEVICE_TYPE_GPU)
        return "CL_DEVICE_TYPE_GPU";
    else if(type==CL_DEVICE_TYPE_ACCELERATOR)
        return "CL_DEVICE_TYPE_ACCELERATOR";
    return "CL_DEVICE_TYPE_ALL";
}


cl_int printDevices(const cl_device_type device_type, const bool long_info)
{
    cl_uint uiDevCount;
    cl_device_id* devices;
    cl_platform_id firstPlatformId;
    cl_uint numPlatforms;
    cl_int errNum;
    errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
    if (errNum != CL_SUCCESS || numPlatforms <= 0)
    {
        std::cerr << "Failed to find any OpenCL platforms." << std::endl;
        return errNum;
    }
    // Get the number of GPU devices available to the platform
    errNum = clGetDeviceIDs(firstPlatformId, device_type, 0, NULL, &uiDevCount);
    if(uiDevCount == 0)
    {
        std::cerr << "Failed to find any OpenCL Devices of type: "<<device_type << std::endl;
        return errNum;
    }
    devices = new cl_device_id [uiDevCount];
    errNum = clGetDeviceIDs(firstPlatformId, device_type, uiDevCount, devices, NULL);
    for(int i=0;i<uiDevCount;i++)
    {
        char buffer[20240];
        cl_uint buf_uint;
        cl_ulong buf_ulong;
        printf("  -- %d --\n", i);
        CHECK_ERROR(clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL));
        printf("  DEVICE_NAME = %s\n", buffer);
        CHECK_ERROR(clGetDeviceInfo(devices[i], CL_DEVICE_VENDOR, sizeof(buffer), buffer, NULL));
        printf("  DEVICE_VENDOR = %s\n", buffer);
        CHECK_ERROR(clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, sizeof(buffer), buffer, NULL));
        printf("  DEVICE_VERSION = %s\n", buffer);
        CHECK_ERROR(clGetDeviceInfo(devices[i], CL_DRIVER_VERSION, sizeof(buffer), buffer, NULL));
        printf("  DRIVER_VERSION = %s\n", buffer);
        CHECK_ERROR(clGetDeviceInfo(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buf_uint), &buf_uint, NULL));
        if (long_info) {
            printf("  DEVICE_MAX_COMPUTE_UNITS = %u\n", (unsigned int)buf_uint);
            CHECK_ERROR(clGetDeviceInfo(devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buf_uint), &buf_uint, NULL));
            printf("  DEVICE_MAX_CLOCK_FREQUENCY = %u\n", (unsigned int)buf_uint);
            CHECK_ERROR(clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, NULL));
            printf("  DEVICE_GLOBAL_MEM_SIZE = %llu (%llu MB)\n", (unsigned long long)buf_ulong,buf_ulong/(1024*1024));
            CHECK_ERROR(clGetDeviceInfo(devices[i], CL_DEVICE_TYPE, sizeof(buf_ulong), &buf_ulong, NULL));
            printf("  DEVICE_TYPE = %s\n", device_type_string(buf_ulong));
            
            CHECK_ERROR(clGetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS, sizeof(buffer), buffer, NULL));
            printf("  CL_DEVICE_EXTENSIONS:\n");
            char * pch;
            pch = strtok (buffer," ");
            while (pch != NULL)
            {
                printf ("        %s\n",pch);
                pch = strtok (NULL, " ");
            }
        }

        
    }
    return CL_SUCCESS;
}


/********************************************************************************/

/* Initial setups */
///
//  Create an OpenCL context on the first available platform using
//  either a GPU or CPU depending on what is available.
//
cl_context CreateContext(cl_int & errNum)
{
    cl_uint numPlatforms;
    cl_platform_id firstPlatformId;
    cl_context context = NULL;

    // First, select an OpenCL platform to run on.  For this example, we
    // simply choose the first available platform.  Normally, you would
    // query for all available platforms and select the most appropriate one.
    errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
    if (errNum != CL_SUCCESS || numPlatforms <= 0)
    {
        std::cerr << "Failed to find any OpenCL platforms." << std::endl;
        return NULL;
    }

    // Next, create an OpenCL context on the platform.  Attempt to
    // create a GPU-based context, and if that fails, try to create
    // a CPU-based context.
    cl_context_properties contextProperties[] =
    {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)firstPlatformId,
        0
    };
    context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU,
                                      NULL, NULL, &errNum);
    if (errNum != CL_SUCCESS)
    {
        std::cout << "Could not create GPU context, trying CPU..." << std::endl;
        context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU,
                                          NULL, NULL, &errNum);
        if (errNum != CL_SUCCESS)
        {
            std::cerr << "Failed to create an OpenCL GPU or CPU context." << std::endl;
            return NULL;
        }
    }

    return context;
}


///
//  Create an OpenCL context on the device specified by device_type and device_num (0-base ordinal）.
//
cl_context CreateContext(cl_device_id & device,
                         const cl_device_type & device_type, 
                         const unsigned int & device_num,
                         cl_int & errNum)
{
    cl_uint numPlatforms;
    cl_platform_id firstPlatformId;
    cl_context context = NULL;

    // First, select an OpenCL platform to run on.  For this example, we
    // simply choose the first available platform.  Normally, you would
    // query for all available platforms and select the most appropriate one.
    errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
    if (errNum != CL_SUCCESS || numPlatforms <= 0)
    {
        std::cerr << "Failed to find any OpenCL platforms." << std::endl;
        return NULL;
    }

    cl_uint numDevices;
    cl_device_id* deviceList;
    
    CHECK_ERROR_N(clGetDeviceIDs(firstPlatformId, device_type, 0, NULL, &numDevices));
    if (numDevices<=0) {
        printf("NO DEVICES FOUND OF SPECIFIED DEVICE TYPE (%s)\n", device_type_string(device_type));
        return NULL;
    }
    if (device_num >= numDevices)
    {
        printf("Specified device number %i is higher than the number of devices %i (0 based)\n", device_num, numDevices);
        return NULL;
    }


    deviceList = new cl_device_id [numDevices];
    CHECK_ERROR_N(clGetDeviceIDs(firstPlatformId, device_type, numDevices, deviceList, NULL));
    device = deviceList[device_num];
    delete [] deviceList;

    CHECK_ERROR_N(printDeviceInfo<char>(device, CL_DEVICE_NAME));

    // Next, create an OpenCL context on the platform.  Attempt to
    // create a GPU-based context, and if that fails, try to create
    // a CPU-based context.
    cl_context_properties contextProperties[] =
    {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)firstPlatformId,
        0
    };
    
    CHECK_RETURN_N(context, clCreateContext(contextProperties, 1, &device, NULL, NULL, &errNum), errNum);

    return context;
}


///
//  Create a command queue on the first device available on the
//  context
//
cl_command_queue CreateCommandQueue(const cl_context context, cl_device_id *device, cl_int & errNum)
{
    cl_device_id *devices;
    cl_command_queue commandQueue = NULL;
    size_t deviceBufferSize = -1;

    // First get the size of the devices buffer
    errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
    if (errNum != CL_SUCCESS)
    {
        std::cerr << "Failed call to clGetContextInfo(...,GL_CONTEXT_DEVICES,...)";
        return NULL;
    }

    if (deviceBufferSize <= 0)
    {
        std::cerr << "No devices available.";
        return NULL;
    }

    // Allocate memory for the devices buffer
    devices = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
    errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);
    if (errNum != CL_SUCCESS)
    {
        delete [] devices;
        std::cerr << "Failed to get device IDs";
        return NULL;
    }

    // In this example, we just choose the first available device.  In a
    // real program, you would likely use all available devices or choose
    // the highest performance device based on OpenCL device queries
    commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);
    if (commandQueue == NULL)
    {
        delete [] devices;
        std::cerr << "Failed to create commandQueue for device 0";
        return NULL;
    }

    *device = devices[0];
    delete [] devices;
    return commandQueue;
}

///
//  Create an OpenCL program from the kernel source file
//
cl_program CreateProgram(const cl_context context, const cl_device_id device, const char* fileName, cl_int & errNum)
{
    cl_program program;

    std::ifstream kernelFile(fileName, std::ios::in);
    if (!kernelFile.is_open())
    {
        std::cerr << "Failed to open file for reading: " << fileName << std::endl;
        return NULL;
    }

    std::ostringstream oss;
    oss << kernelFile.rdbuf();

    std::string srcStdStr = oss.str();
    const char *srcStr = srcStdStr.c_str();
    program = clCreateProgramWithSource(context, 1,
                                        (const char**)&srcStr,
                                        NULL, NULL);
    if (program == NULL)
    {
        std::cerr << "Failed to create CL program from source." << std::endl;
        return NULL;
    }

    errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (errNum != CL_SUCCESS)
    {
        // Determine the reason for the error
        char buildLog[16384];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
                              sizeof(buildLog), buildLog, NULL);

        std::cerr << "Error in kernel: " << std::endl;
        std::cerr << buildLog;
        clReleaseProgram(program);
        return NULL;
    }

    return program;
}

///
//  Create memory object for read
//  used as the arguments to the kernel
//
template <typename T>
bool CreateMemObject(cl_context context, cl_mem & memObject,
                     T * arr, size_t array_size)
{
   memObject = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                  sizeof(T) * array_size, arr, NULL);

   if (memObject == NULL)
   {
       std::cerr << "Error creating memory object of size " << array_size;
       std::cerr << " for read." << std::endl;
       return false;
   }

   return true;
}


///
//  Create memory object for read and write 
//  used as the arguments to the kernel
//
template <typename T>
bool CreateMemObject(const cl_context context, cl_mem & memObject,
                     const size_t array_size)
{
   memObject = clCreateBuffer(context, CL_MEM_READ_WRITE,
                                  sizeof(T) * array_size, NULL, NULL);

   if (memObject == NULL)
   {
       std::cerr << "Error creating memory object of size " << array_size;
       std::cerr << " for read & write." << std::endl;
       return false;
   }

   return true;
}


///
//  Cleanup any created OpenCL resources
//
void Cleanup(cl_context context, cl_command_queue commandQueue,
             cl_program program, cl_kernel kernel, cl_mem * memObjects, size_t num_memObjects)
{
    for (size_t i = 0; i < num_memObjects; i++)
    {
        if (memObjects[i] != 0)
            clReleaseMemObject(memObjects[i]);
    }
    if (commandQueue != 0)
        clReleaseCommandQueue(commandQueue);

    if (kernel != 0)
        clReleaseKernel(kernel);

    if (program != 0)
        clReleaseProgram(program);

    if (context != 0)
        clReleaseContext(context);

}



/********************************************************************************/