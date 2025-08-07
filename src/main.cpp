
#include "sparta/app/CommandLineSimulator.hpp"
#include "sparta/app/Simulation.hpp"

#include "Simulator.hpp"
#include "CPUFactory.hpp"

#include <iostream> // for error output

const char USAGE[] =
    "Usage:\n"
    "    [-i insts] [-r RUNTIME] [--show-tree] [--show-dag]\n"
    "    [-p PATTERN VAL] [-c FILENAME]\n"
    "    [--node-config-file PATTERN FILENAME]\n"
    "    [-l PATTERN CATEGORY DEST]\n"
    "    [-h]\n"
    "\n";

constexpr char VERSION_VARNAME[] = "0.1.0";

int main(int argc, char **argv)
{
    sparta::app::DefaultValues DEFAULTS;
    DEFAULTS.auto_summary_default = "on";

    // create Sparta command-line simulator

    sparta::app::CommandLineSimulator cls(USAGE, DEFAULTS);

    // parse the command-line arguments and return error in this case
    int err_code = 0;
    if (!cls.parse(argc, argv, err_code))
        return err_code;

    sparta::Scheduler scheduler;
    Simulator sim(scheduler);

    cls.populateSimulation(&sim);
    cls.runSimulator(&sim);
    cls.postProcess(&sim);

    return 0;
}
