
#include "CPUFactory.hpp"

#include <string>
#include <algorithm>
#include "sparta/simulation/ParameterSet.hpp"
#include "sparta/utils/SpartaException.hpp"

namespace llangardaix {

CPUFactory::CPUFactory() :
    sparta::ResourceFactory<llangardaix::CPU, llangardaix::CPU::CPUParameterSet>() {}

CPUFactory::~CPUFactory() = default;

void CPUFactory::setTopology(const YAML::Node& topology_node)
{
    // The topology string is now treated as a file path.
    // The CPUTopology object is created and populated here.
    sparta_assert(!topology_);
    topology_ = llangardaix::CPUTopology::allocateTopology(topology_node);
}

void CPUFactory::buildUnitRecursive(
    sparta::TreeNode* parent_node,
    const CPUTopology::UnitInfo& unit_info
) {
    // create the ResourceTreeNode for the current unit
    // the 'parent_node' argument correctly places this new node
    // within the SPARTA tree hierarchically
    sparta::ResourceTreeNode* rtn = new sparta::ResourceTreeNode(
        parent_node,
        unit_info.name,
        unit_info.group_name,
        unit_info.group_id,
        unit_info.human_name,
        unit_info.factory
    );

    // iterate over the parameters parsed from the YAML
    // and apply them to the Node's ParameterSet
    sparta::ParameterSet *node_params = rtn->getChildAs<sparta::ParameterSet>("params");

    for (const auto& kv_pair : unit_info.params) {
        const std::string param_name  = kv_pair.first.as<std::string>();
        const std::string param_value = kv_pair.second.as<std::string>();

        sparta::ParameterBase *target_param = node_params->getParameter(param_name, true);
        target_param->setValueFromString(param_value);
    }

    // handle private subtrees
    // if specified from the YAML, the node won't be visible through the public tree
    if (unit_info.is_private_subtree) {
        rtn->makeSubtreePrivate();
        private_nodes_.emplace_back(rtn);
    }

    // store the created ResourceTreeNode for memory management and future reference.
    // unique_ptr ensures proper deletion when 'to_delete_' goes out of scope
    to_delete_.emplace_back(rtn);
    resource_names_.emplace_back(rtn->getName()); // store the resource's names for getResourceNames()

    // keep recursing if there are children nodes
    for (const auto& child_unit : unit_info.children_units) {
        buildUnitRecursive(rtn, child_unit);
    }
}

// This method builds the SPARTA tree based on the UnitInfo vector
void CPUFactory::buildTree_(
    sparta::RootTreeNode* root_node,
    const std::vector<llangardaix::CPUTopology::UnitInfo>& units)
{
    // Iterate through all the units defined in the topology
    for(const auto& unit : units) {
        buildUnitRecursive(root_node, unit);
    }
}

// This method binds all the ports based on the PortConnectionInfo vector
void CPUFactory::bindTree_(
    sparta::RootTreeNode* root_node,
    const std::vector<llangardaix::CPUTopology::PortConnectionInfo>& ports)
{
    sparta_assert(topology_); // ensure 'topology_' has been set before calling this function

    // bind all the ports specified in the YAML file
    for(const auto& port : ports) {
        sparta::bind(
            root_node->getChildAs<sparta::Port>(port.output_port_name),
            root_node->getChildAs<sparta::Port>(port.input_port_name)
        );
    }
}

// Public interface to build the tree
void CPUFactory::buildTree(sparta::RootTreeNode* root_node)
{
    sparta_assert(topology_);
    buildTree_(root_node, topology_->top_level_units);
}

// Public interface to bind the tree
void CPUFactory::bindTree(sparta::RootTreeNode* root_node)
{
    sparta_assert(topology_);
    bindTree_(root_node, topology_->connections);
}

// return a list of resource names instantiated in this topology
const std::vector<std::string>& CPUFactory::getResourceNames() const
{
    return resource_names_;
}

// Override deleteSubtree
void CPUFactory::deleteSubtree(sparta::ResourceTreeNode *node)
{
    to_delete_.clear();
}

} // namespace llangardaix

