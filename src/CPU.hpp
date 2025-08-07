#pragma once

#include <string>

#include "sparta/simulation/Unit.hpp"
#include "sparta/simulation/TreeNode.hpp"
#include "sparta/simulation/ParameterSet.hpp"

namespace llangardaix {

class CPU : public sparta::Unit
{
public:
    // define the ParameterSet for the CPU resource
    // will be accessed by the CPUFactory
    class CPUParameterSet : public sparta::ParameterSet
    {
    public:
        CPUParameterSet(sparta::TreeNode *n) : sparta::ParameterSet(n) {}
    };

    // constructor - It takes a TreeNode pointer to its parent and it's parameterset pointer
    CPU(sparta::TreeNode *node, const CPUParameterSet *params);
    // destructor
    ~CPU();

    static constexpr char name[] = "cpu";

private:
    const CPUParameterSet* params_;
};

} // llangardaix
