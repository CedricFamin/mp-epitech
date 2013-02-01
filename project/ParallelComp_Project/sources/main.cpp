#include "../includes/main.hpp"
#include <unistd.h>

#include "../includes/Program.h"

using namespace ICoDF;
using namespace ICoDF_HTM;

int main(int ac, char **av)
{
    // SETTING UP LOG
	LogService::GetInstance()->SetConfiguration(LogService::LS_PRINT_ON_COUT);
	LS_ADDMSG(LogService::NOTICE, "main", "BLINK::HTM test main");
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
		LS_ADDMSG(LogService::FATAL, "main", "Usage : ./LandySzalayEstimator <catalog_file> <radius> <delta>");
	
	LS_ADDMSG(LogService::NOTICE, "main", "Exiting...");
	LogService::Delete();
	return 0;
}
