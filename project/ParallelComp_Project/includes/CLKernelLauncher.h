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

namespace CL
{
    class CLKernelLauncher
    {
    public:
        CLKernelLauncher();
        ~CLKernelLauncher();
        
        void LoadProgram(std::string const & filename);
        void RunProgram();
        
    protected:
    private:
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
    };
}

#endif
