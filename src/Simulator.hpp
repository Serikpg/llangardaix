
#pragma once

#include "sparta/app/Simulation.hpp"
#include "CPUFactory.hpp"

class Simulator : public sparta::app::Simulation
{
public:
    // Simulator class constructor
    Simulator(sparta::Scheduler & scheduler);
    virtual ~Simulator();
private:
    //////////////////////////////////////////////////
    // Setup

    // build the tree using tree nodes
    void buildTree_() override;
    // configure the tree and apply last minute parameter changes
    void configureTree_() override;
    // tree is configured, built and instanciated
    // bind ports and Units toghether
    void bindTree_() override;


    //////////////////////////////////////////////////
    // Runtime

    // get the factory for topology build
    llangardaix::CPUFactory *getCPUFactory_();
};

