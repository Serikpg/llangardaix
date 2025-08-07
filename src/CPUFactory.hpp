
#pragma once

#include "sparta/simulation/ResourceFactory.hpp"
#include "sparta/simulation/RootTreeNode.hpp"
#include "sparta/simulation/ResourceTreeNode.hpp"

#include "CPU.hpp"
#include "CPUTopology.hpp"
#include "CPUFactories.hpp"

namespace llangardaix {

class CPUFactory : public sparta::ResourceFactory<CPU, CPU::CPUParameterSet>
{
public:
    CPUFactory();
    ~CPUFactory();

    // set the user defined topology for this microarchitecture
    void setTopology(const YAML::Node& topology_node);

    // build the device tree by instancitating resource nodes
    void buildTree(sparta::RootTreeNode *root_node);

    // bind all the paorts between differnet units and set TLBs and preload
    void bindTree(sparta::RootTreeNode *root_node);

    // get a list of resources instantiated in this topology
    const std::vector<std::string>& getResourceNames() const;
private:
    void deleteSubtree(sparta::ResourceTreeNode *) override;

    void buildUnitRecursive(
        sparta::TreeNode* root_node,
        const CPUTopology::UnitInfo& unit_info
    );

    // private implementation wrapper for buildTree interface
    void buildTree_(
        sparta::RootTreeNode *root,
        const std::vector<CPUTopology::UnitInfo> &infovector
    );

    // private implementation wrapper for bindTree interface
    void bindTree_(
        sparta::RootTreeNode *root,
        const std::vector<CPUTopology::PortConnectionInfo> &connectionsvector
    );

    // the user-defined topology
    std::unique_ptr<CPUTopology> topology_;
    // vector maintaining the instantiated resource names
    std::vector<std::string> resource_names_;
    // vector of private tree nodes
    std::vector<sparta::TreeNode*> private_nodes_;
    // list for deleting nodes that this class created
    std::vector<std::unique_ptr<sparta::ResourceTreeNode>> to_delete_;

}; // class CPUFactory

} // namespace llangardaix
