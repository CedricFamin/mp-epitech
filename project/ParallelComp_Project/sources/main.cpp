#include "../includes/main.hpp"
#include <unistd.h>

#include "../includes/Program.h"
#include "../includes/CLKernelLauncher.h"

#include "../includes/Vector.hpp"

using namespace ICoDF;
using namespace ICoDF_HTM;

int main(int ac, char **av)
{
    // SETTING UP LOG
    //GenericMemoryPool<trixel_t>::CreateInstance();
    
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
