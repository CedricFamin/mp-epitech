#include "../includes/main.hpp"
#include <unistd.h>

#include "../includes/Program.h"
#include "../includes/CLKernelLauncher.h"

#include "../includes/Vector.hpp"

using namespace ICoDF;
using namespace ICoDF_HTM;

void testOpenCL()
{
    float args1[1024] = {884274.0f};
    float args2[1024] = {1.0f};
    float argsResult[1024] = {0.0f};
    
    for (int i = 0; i < 1024; ++i)
    {
        args2[i] = i;
        args1[i] = i;
    }
    
    CL::CLKernelLauncher cl;
    
    cl.LoadProgram("kernel.cl");
    cl.InitKernel("test_program", 1024);
    cl.AddArg<float>(args1, 0, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR);
    cl.AddArg<float>(args2, 1, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR);
    cl.AddArg<float>(argsResult, 2, CL_MEM_WRITE_ONLY);
    cl.RunProgram();
    cl.ReadArgs<float>(argsResult, 2);
    cl.CleanKernel();

    for (int i = 0; i < 1024; ++i)
    {
        std::cout << "Result[" << i << "] = " << argsResult[i] << std::endl;
    }
}

int main(int ac, char **av)
{
    //testOpenCL();
    
    LogService::GetInstance()->SetConfiguration(LogService::LS_PRINT_ON_COUT);
    LogQueue* logQueue = LogService::GetInstance()->CreateNewLogQueue();
    logQueue->AddLogMessage(LogService::NOTICE, "main", "BLINK::HTM test main");
	if (ac == 4)
    {
        ProgramConfig config;
        
        config.loop = 100;
        config.filename = av[1];
        std::istringstream stm, stm2;
		stm.str(av[2]);
		stm >> config.radius;
		stm2.str(av[3]);
		stm2 >> config.delta;
        
        Program prgm;
        prgm.Init(config);
        prgm.Launch();
        prgm.Clean();
    }
	else
        logQueue->AddLogMessage(LogService::FATAL, "main", "Usage : ./LandySzalayEstimator <catalog_file> <radius> <delta>");	
    
    logQueue->AddLogMessage(LogService::NOTICE, "main", "Exiting...");
	LogService::Delete();
    //GenericMemoryPool<trixel_t>::DeleteInstance();
	return 0;
}
