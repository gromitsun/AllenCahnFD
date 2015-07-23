#ifndef CL_COMMON_HPP
#define CL_COMMON_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif


/********************************************************************************/

#ifdef _WIN32
#define __PATHSEP__ '\\'
#else 
#define __PATHSEP__ '/'
#endif

// Get basename from a file path
#define __SHORT_FORM_OF_FILE__ \
(std::strrchr(__FILE__, __PATHSEP__) \
? std::strrchr(__FILE__, __PATHSEP__) + 1 \
: __FILE__ \
)



#ifdef __APPLE__	// Define time type
typedef  uint64_t ttime_t;
#else
typedef clock_t ttime_t;
#endif


/********************************************************************************/

/* Get CPU time */
ttime_t getTime();

double subtractTimes( const ttime_t endTime, const ttime_t startTime );


/********************************************************************************/

/* Error checking */
const char *get_error_string(cl_int errNum);

inline void check_error(const cl_int & errNum, const std::string & operation_name)
{
    if (errNum != CL_SUCCESS)
    {
        std::cerr << "ERROR: " << operation_name << std::endl;
        std::cerr << "Error code: " << get_error_string(errNum) << " (" << errNum << ")" << std::endl;
        
        exit(EXIT_FAILURE);
    }
}

inline void check_error(const bool & statement, const std::string & operation_name)
{
    if (!statement)
    {
        std::cerr << "ERROR: " << operation_name << std::endl;
               
        exit(EXIT_FAILURE);
    }
}

/* Error checking macros */
// func returning err; return err on error.
#define CHECK_ERROR(func)\
{\
int err=(func);\
if(err!=CL_SUCCESS)\
{\
std::cerr<<__SHORT_FORM_OF_FILE__<<"("<<__LINE__<<"): "<<"Error ("<<err<<"): "<<get_error_string(err)<<std::endl;\
return err;\
}\
}
// func returning object; check if ret == NULL; return NULL on error.
#define CHECK_RETURN_N(ret,func,err)\
(ret)=(func);\
if((ret)==NULL||(err)){\
std::cerr<<__SHORT_FORM_OF_FILE__<<"("<<__LINE__<<"): "<<"Error ("<<err<<"): "<<get_error_string(err)<<std::endl;\
return NULL;}
// func returning err code; return NULL on error.
#define CHECK_ERROR_N(func)\
{\
int err=(func);\
if(err)\
{\
std::cerr<<__SHORT_FORM_OF_FILE__<<"("<<__LINE__<<"): "<<"Error ("<<err<<"): "<<get_error_string(err)<<std::endl;\
return NULL;\
}\
}

// func returning err; return err on error.
#define CHECK_ERROR_EXIT(func)\
{\
int err=(func);\
if(err!=CL_SUCCESS)\
{\
std::cerr<<__SHORT_FORM_OF_FILE__<<"("<<__LINE__<<"): "<<"Error ("<<err<<"): "<<get_error_string(err)<<std::endl;\
exit(EXIT_FAILURE);\
}\
}



/********************************************************************************/

/* Get device info */

const char * device_info_to_str(const cl_device_info param_name);

const char * device_type_string(const cl_device_type type);


template <typename T>
T * getDeviceInfo(const cl_device_id device, const cl_device_info param_name, cl_int & errNum)
{
	std::size_t paramValueSize;

	errNum = clGetDeviceInfo(device, param_name, 0, NULL, &paramValueSize);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Failed to get OpenCL device info " << device_info_to_str(param_name) << "." << std::endl;
		return NULL;
	}

	// T * info = (T *)alloca(sizeof(T) * paramValueSize);
	T * info = new T[paramValueSize];
	errNum = clGetDeviceInfo(device, param_name, paramValueSize, info, NULL);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Failed to get OpenCL device info " << device_info_to_str(param_name) << "." << std::endl;
		return NULL;
	}

	return info;
}


template <typename T>
cl_int printDeviceInfo(const cl_device_id device, const cl_device_info param_name)
{
	cl_int errNum;
	T * info = getDeviceInfo<T>(device, param_name, errNum);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Failed to get OpenCL device info " << device_info_to_str(param_name) << "." << std::endl;
		return CL_INVALID_DEVICE;
	}

	std::cout << device_info_to_str(param_name) << ": " << info << std::endl;
	delete [] info;

	return CL_SUCCESS;
}

cl_int printDevices(const cl_device_type device_type, const bool long_info=true);

/********************************************************************************/

/* Initial setups */
cl_context CreateContext(cl_int & err);

cl_context CreateContext(cl_device_id & device,
                         const cl_device_type & device_type, 
                         const unsigned int & device_num,
                         cl_int & err);

cl_command_queue CreateCommandQueue(const cl_context context, cl_device_id *device, cl_int & err);

cl_program CreateProgram(const cl_context context, const cl_device_id device, const char* fileName, cl_int & err);

template <typename T>
bool CreateMemObject(const cl_context context, cl_mem & memObject,
                     T * arr, const size_t array_size);

template <typename T>
bool CreateMemObject(const cl_context context, cl_mem & memObject,
                     const size_t array_size);

void Cleanup(cl_context context, cl_command_queue commandQueue,
             cl_program program, cl_kernel kernel, cl_mem * memObjects, size_t num_memObjects);



/********************************************************************************/



#endif