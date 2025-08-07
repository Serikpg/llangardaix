
#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <yaml-cpp/yaml.h>

#include "sparta/simulation/ResourceFactory.hpp"
#include "sparta/simulation/RootTreeNode.hpp"
#include "sparta/simulation/ResourceTreeNode.hpp"
#include "sparta/simulation/ParameterSet.hpp"

#include "CPUFactories.hpp"

namespace llangardaix {

class CPUTopology
{
public:
    struct UnitInfo
    {
        std::string                     name;
        std::string                     parent_name;
        std::string                     human_name;
        std::string                     group_name;
        uint32_t                        group_id;
        bool                            is_private_subtree;
        sparta::ResourceFactoryBase     *factory;
        YAML::Node                      params;
        std::vector<UnitInfo>           children_units;
    };

    struct PortConnectionInfo
    {
        std::string output_port_name;
        std::string input_port_name;
    };

    CPUTopology(const YAML::Node& topology_node);
    virtual ~CPUTopology() {}

    static std::unique_ptr<CPUTopology> allocateTopology(const YAML::Node& topology_node);

    virtual void bindTree(sparta::RootTreeNode* root_node) {}

    // public members used by CPUFactory ot build and bind tree
    std::vector<UnitInfo>           top_level_units;
    std::vector<PortConnectionInfo> connections;
private:
    // private method to handle the YAML parsing logic
    void parseYamlNodeRecursive(const YAML::Node& unit_node, UnitInfo& unit_info_out);
    void parseYamlNode(const YAML::Node& topology_node); // wrapper for the recursive traversal
}; // class Topology

} // namespace llangardaix
