//
//  CLKernelLauncher.h
//  LandySzalayEstimator
//
//  Created by Cédric Famin on 16/02/13.
//
//

#ifndef LandySzalayEstimator_CLKernelLauncher_h
# define LandySzalayEstimator_CLKernelLauncher_h

# if defined __APPLE__ || defined(MACOSX)
#  include <OpenCL/opencl.h>
# else
#  include <CL/opencl.h>
# endif

#include <string>
#include <vector>
#include <iostream>

namespace CL
{
    namespace {
        // Helper function to get error string
        // *********************************************************************
        const char* oclErrorString(cl_int error)
        {
            static const char* errorString[] = {
                "CL_SUCCESS",
                "CL_DEVICE_NOT_FOUND",
                "CL_DEVICE_NOT_AVAILABLE",
                "CL_COMPILER_NOT_AVAILABLE",
                "CL_MEM_OBJECT_ALLOCATION_FAILURE",
                "CL_OUT_OF_RESOURCES",
                "CL_OUT_OF_HOST_MEMORY",
                "CL_PROFILING_INFO_NOT_AVAILABLE",
                "CL_MEM_COPY_OVERLAP",
                "CL_IMAGE_FORMAT_MISMATCH",
                "CL_IMAGE_FORMAT_NOT_SUPPORTED",
                "CL_BUILD_PROGRAM_FAILURE",
                "CL_MAP_FAILURE",
                "",
                "",
                "",
                "",
                "",
                "",
                "",
                "",
                "",
                "",
                "",
                "",
                "",
                "",
                "",
                "",
                "",
                "CL_INVALID_VALUE",
                "CL_INVALID_DEVICE_TYPE",
                "CL_INVALID_PLATFORM",
                "CL_INVALID_DEVICE",
                "CL_INVALID_CONTEXT",
                "CL_INVALID_QUEUE_PROPERTIES",
                "CL_INVALID_COMMAND_QUEUE",
                "CL_INVALID_HOST_PTR",
                "CL_INVALID_MEM_OBJECT",
                "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
                "CL_INVALID_IMAGE_SIZE",
                "CL_INVALID_SAMPLER",
                "CL_INVALID_BINARY",
                "CL_INVALID_BUILD_OPTIONS",
                "CL_INVALID_PROGRAM",
                "CL_INVALID_PROGRAM_EXECUTABLE",
                "CL_INVALID_KERNEL_NAME",
                "CL_INVALID_KERNEL_DEFINITION",
                "CL_INVALID_KERNEL",
                "CL_INVALID_ARG_INDEX",
                "CL_INVALID_ARG_VALUE",
                "CL_INVALID_ARG_SIZE",
                "CL_INVALID_KERNEL_ARGS",
                "CL_INVALID_WORK_DIMENSION",
                "CL_INVALID_WORK_GROUP_SIZE",
                "CL_INVALID_WORK_ITEM_SIZE",
                "CL_INVALID_GLOBAL_OFFSET",
                "CL_INVALID_EVENT_WAIT_LIST",
                "CL_INVALID_EVENT",
                "CL_INVALID_OPERATION",
                "CL_INVALID_GL_OBJECT",
                "CL_INVALID_BUFFER_SIZE",
                "CL_INVALID_MIP_LEVEL",
                "CL_INVALID_GLOBAL_WORK_SIZE",
            };
            
            const int errorCount = sizeof(errorString) / sizeof(errorString[0]);
            
            const int index = -error;
            
            return (index >= 0 && index < errorCount) ? errorString[index] : "";
            
        }
    }
    
    class CLKernelLauncher
    {
    public:
        CLKernelLauncher();
        ~CLKernelLauncher();
        
        void LoadProgram(std::string const & filename);
        void RunProgram();
        void InitKernel(std::string const & kernelName, unsigned int nbExec);
        void CleanKernel()
        {
            //clFinish(_command_queue);
        }
        
        template<typename T>
        void AddArg(T* arg, unsigned int pos, cl_mem_flags memFlag)
        {
            cl_mem cl_buffer;
            
            if (memFlag & CL_MEM_WRITE_ONLY)
                cl_buffer = clCreateBuffer(_context, memFlag, sizeof(T) * _num, NULL, &_err);
            else
                cl_buffer = clCreateBuffer(_context, memFlag, sizeof(T) * _num, arg, &_err);
            std::cout << oclErrorString(_err) << std::endl;
            
            
            if (!(memFlag & CL_MEM_COPY_HOST_PTR) && !(memFlag & CL_MEM_WRITE_ONLY))
            {
                _err = clEnqueueWriteBuffer(_command_queue, cl_buffer, CL_TRUE, 0, sizeof(T) * _num, arg, 0, NULL, &_event);
                clReleaseEvent(_event);
            }
            _err  = clSetKernelArg(_kernel, pos, sizeof(cl_mem), (void *) &cl_buffer);
            std::cout << oclErrorString(_err) << std::endl;
            
            if (_argList.size() < pos + 1)
            {
                _argList.resize(pos+1);
            }
            _argList[pos] = cl_buffer;
        }
        
        template<typename T>
        void ReadArgs(T* readArgs, unsigned int pos)
        {
            _err = clEnqueueReadBuffer(_command_queue, _argList[pos], CL_TRUE, 0, sizeof(T) * _num, readArgs, 0, NULL, &_event);
            std::cout << oclErrorString(_err) << std::endl;
            
            clReleaseEvent(_event);
        }
        
    protected:
    private:
        void BuildExecutable();
        
        //handles for creating an opencl context
        cl_platform_id _platform;
        
        //device variables
        cl_device_id*    _devices;
        cl_uint          _numDevices;
        unsigned int     _deviceUsed;
        
        cl_context       _context;
        
        cl_command_queue _command_queue;
        cl_program       _program;
        cl_kernel        _kernel;
        
        
        //debugging variables
        cl_int   _err;
        cl_event _event;
        
        // Run Kernel variables
        int    _num;
        size_t _workGroupSize[1];
        
        // Args
        std::vector<cl_mem> _argList;
    };
}

#endif
