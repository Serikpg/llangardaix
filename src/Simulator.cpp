
#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>

#include "Simulator.hpp"

#include "sparta/simulation/Clock.hpp"
#include "sparta/utils/TimeManager.hpp"
#include "sparta/simulation/TreeNode.hpp"
#include "sparta/utils/StringUtils.hpp"
#include "sparta/app/Simulation.hpp" // needed for getCustomConfig()

Simulator::Simulator(sparta::Scheduler & scheduler) :
    sparta::app::Simulation("llangardaix", &scheduler)
{
    getResourceSet()->addResourceFactory<llangardaix::CPUFactory>();
}

Simulator::~Simulator()
{
    getRoot()->enterTeardown(); // allow deletion of nodes without error
}

//////////////////////////////////////////////////

// helper method to get the CPUFactory instance
llangardaix::CPUFactory* Simulator::getCPUFactory_()
{
    // retrieve he registered resource factory for "cpu"
    auto sparta_res_factory = getResourceSet()->getResourceFactory("cpu");
    // safely cast it to your CPUFactory type
    auto cpu_factory = dynamic_cast<llangardaix::CPUFactory*>(sparta_res_factory);
    return cpu_factory;
}

//////////////////////////////////////////////////

// build the resource tree by using the CPUFactory and CPUTopology methods
void Simulator::buildTree_()
{
    auto cpu_factory = getCPUFactory_();

    // get the YAML configuration file path
    sparta::app::SimulationConfiguration* config = getSimulationConfiguration();
    const std::string& config_file_path = config->getFinalConfigFile();
    if (config_file_path.empty()) {
        throw sparta::SpartaException("No final configuration file was provided.");
    }

    YAML::Node full_config_yaml = YAML::LoadFile(config_file_path);

    if(!full_config_yaml["topology"]) {
        throw sparta::SpartaException("Main configuration YAML is missing the 'topology' block.");
    }
    const YAML::Node& topology_node = full_config_yaml["topology"];

    cpu_factory->setTopology(topology_node);
    cpu_factory->buildTree(getRoot());
}

void Simulator::configureTree_()
{
    // configuration from command line has already been applied
}

void Simulator::bindTree_()
{
    auto cpu_factory = getCPUFactory_();
    cpu_factory->bindTree(getRoot());
}

