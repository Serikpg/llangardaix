
#pragma once

#include <string>

#include "sparta/simulation/Unit.hpp"
#include "sparta/simulation/TreeNode.hpp"
#include "sparta/simulation/ParameterSet.hpp"

namespace llangardaix {

class Core : public sparta::Unit
{
public:
    class CoreParameterSet : public sparta::ParameterSet
    {
    public:
        CoreParameterSet(sparta::TreeNode *n) : sparta::ParameterSet(n) {}
    };

    // constructor - It takes a TreeNode pointer to its parent and it's parameterset pointer
    Core(sparta::TreeNode *node, const CoreParameterSet *params);
    // destructor
    ~Core();

    static constexpr char name[] = "corTg";

private:
    const CoreParameterSet* params_;
};

} // llangardaix

