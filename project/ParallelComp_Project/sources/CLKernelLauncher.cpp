//
//  CLKernelLauncher.cpp
//  LandySzalayEstimator
//
//  Created by Cédric Famin on 16/02/13.
//
//

#include "../includes/CLKernelLauncher.h"

namespace CL
{
    namespace
    {
        char *file_contents(const char *filename, int *length)
        {
            FILE *f = fopen(filename, "r");
            void *buffer;
            
            if (!f) {
                return NULL;
            }
            
            fseek(f, 0, SEEK_END);
            *length = ftell(f);
            fseek(f, 0, SEEK_SET);
            
            buffer = malloc(*length+1);
            *length = fread(buffer, 1, *length, f);
            fclose(f);
            ((char*)buffer)[*length] = '\0';
            
            return (char*)buffer;
        }
        
        cl_int oclGetPlatformID(cl_platform_id* clSelectedPlatformID)
        {
            char chBuffer[1024];
            cl_uint num_platforms;
            cl_platform_id* clPlatformIDs;
            cl_int ciErrNum;
            *clSelectedPlatformID = NULL;
            cl_uint i = 0;
            
            // Get OpenCL platform count
            ciErrNum = clGetPlatformIDs (0, NULL, &num_platforms);
            if (ciErrNum != CL_SUCCESS)
            {
                return -1000;
            }
            else
            {
                if(num_platforms == 0)
                {
                    return -2000;
                }
                else
                {
                    // if there's a platform or more, make space for ID's
                    if ((clPlatformIDs = (cl_platform_id*)malloc(num_platforms * sizeof(cl_platform_id))) == NULL)
                        return -3000;
                    
                    // get platform info for each platform and trap the NVIDIA platform if found
                    ciErrNum = clGetPlatformIDs (num_platforms, clPlatformIDs, NULL);
                    printf("Available platforms:\n");
                    for(i = 0; i < num_platforms; ++i)
                    {
                        ciErrNum = clGetPlatformInfo (clPlatformIDs[i], CL_PLATFORM_NAME, 1024, &chBuffer, NULL);
                        if(ciErrNum == CL_SUCCESS)
                        {
                            if(strstr(chBuffer, "NVIDIA") != NULL)
                            {
                                *clSelectedPlatformID = clPlatformIDs[i];
                                break;
                            }
                        }
                    }
                    
                    // default to zeroeth platform if NVIDIA not found
                    if(*clSelectedPlatformID == NULL)
                        *clSelectedPlatformID = clPlatformIDs[0];
                    
                    free(clPlatformIDs);
                }
            }
            
            return CL_SUCCESS;
        }
    }
    
    CLKernelLauncher::CLKernelLauncher()
    : _platform(0)
    , _devices(0)
    , _numDevices(0)
    , _deviceUsed(0)
    , _context(0)
    , _command_queue(0)
    , _program(0)
    , _kernel(0)
    , _err(0)
    , _event(0)
    , _num(0)
    {
        _err = oclGetPlatformID(&_platform);
        _err = clGetDeviceIDs(_platform, CL_DEVICE_TYPE_GPU, 0, NULL, &_numDevices);
        _devices = new cl_device_id [_numDevices];
        _err = clGetDeviceIDs(_platform, CL_DEVICE_TYPE_GPU, _numDevices, _devices, NULL);
        _context = clCreateContext(0, 1, _devices, NULL, NULL, &_err);
        _deviceUsed = 0;
        _command_queue = clCreateCommandQueue(_context, _devices[_deviceUsed], 0, &_err);
    }
    
    CLKernelLauncher::~CLKernelLauncher()
    {
        if(_kernel) clReleaseKernel(_kernel);
        if(_program) clReleaseProgram(_program);
        if(_command_queue) clReleaseCommandQueue(_command_queue);
        
        if(_context) clReleaseContext(_context);
        
        if(_devices) delete(_devices);
    }
    
    void CLKernelLauncher::LoadProgram(const std::string &filename)
    {
        int pl;
        size_t program_length;
        
        std::string path(CL_SOURCE_DIR);
        path += "/" + std::string(filename);
        
        char* cSourceCL = file_contents(path.c_str(), &pl);
        program_length = (size_t)pl;
        
        _program = clCreateProgramWithSource(_context, 1, (const char **) &cSourceCL, &program_length, &_err);
        
        //buildExecutable();
        
        free(cSourceCL);
    }
    
    void CLKernelLauncher::RunProgram()
    {
        _err = clEnqueueNDRangeKernel(_command_queue, _kernel, 1, NULL, _workGroupSize, NULL, 0, NULL, &_event);
        clReleaseEvent(_event);
        clFinish(_command_queue);
        
        // Read arg here
        
        // ---====---
        clReleaseEvent(_event);
    }
    
}
