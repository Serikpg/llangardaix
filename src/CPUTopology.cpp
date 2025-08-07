
#include <iostream>
#include <fstream>

#include "sparta/utils/SpartaException.hpp"

#include "CPUTopology.hpp"

llangardaix::CPUTopology::CPUTopology(const YAML::Node& topology_node)
{
    parseYamlNode(topology_node);
}

void llangardaix::CPUTopology::parseYamlNodeRecursive(const YAML::Node& topology_node, UnitInfo& unit_info_out)
{
    // parse the basic unit properties from the current YAML node
    unit_info_out.name               = topology_node["name"].as<std::string>();
    // on the top level the parent can be empty
    unit_info_out.parent_name        = topology_node["parent_name"] ? topology_node["parent_name"].as<std::string>() : "";
    unit_info_out.human_name         = topology_node["human_name"].as<std::string>();
    unit_info_out.group_name         = topology_node["group_name"].as<std::string>();
    unit_info_out.group_id           = topology_node["group_id"].as<uint32_t>();
    unit_info_out.is_private_subtree = topology_node["is_private_subtree"].as<bool>();

    // get the factory from the factories object based on the name
    const std::string factory_name = topology_node["factory_name"].as<std::string>();
    unit_info_out.factory = CPUFactories::getGlobalInstance()->getFactory(factory_name);
    if (!unit_info_out.factory) {
        throw sparta::SpartaException("Unknown factory name in YAML for unit '") << unit_info_out.name << "': " << factory_name;
    }

    // parse the parameters
    if (topology_node["parameters"]) {
        unit_info_out.params = topology_node["parameters"];
    }

    // recursively parse the children units
    // if 'children_units' exist, iterate through them and call this function again
    if (topology_node["children_units"]) {
        for (const auto& child_node : topology_node["children_units"]) {
            UnitInfo child_unit;
            parseYamlNodeRecursive(child_node, child_unit);
            unit_info_out.children_units.push_back(child_unit);
        }
    }

    // parse internal connections specific to this unit's subtree
    if (topology_node["connections"]) {
        for (const auto& connection_node : topology_node["connections"]) {
            PortConnectionInfo connection;
            connection.output_port_name = connection_node["output_port_name"].as<std::string>();
            connection.input_port_name  = connection_node["input_port_name"].as<std::string>();
            connections.push_back(connection);
        }
    }
}

void llangardaix::CPUTopology::parseYamlNode(const YAML::Node& topology_node)
{
    try {
        // parse units
        if(topology_node["units"]) {
            for (const auto &unit_node : topology_node["units"]) {
                UnitInfo unit;
                parseYamlNodeRecursive(unit_node, unit);
                top_level_units.push_back(unit);
            }
        }

        // parse port connections
        if(topology_node["connections"]) {
            for (const auto& connection_node : topology_node["connections"]) {
                PortConnectionInfo connection;
                connection.output_port_name = connection_node["output_port_name"].as<std::string>();
                connection.input_port_name  = connection_node["input_port_name"].as<std::string>();
                connections.push_back(connection);
            }
        }
    } catch(const YAML::ParserException& e) {
        throw sparta::SpartaException(std::string("YAML parsing error in topology section: ") + e.what());
    } catch(const std::exception& e) {
        throw sparta::SpartaException(std::string("Error during YAML parsing: ") + e.what());
    }
}

std::unique_ptr<llangardaix::CPUTopology>
llangardaix::CPUTopology::allocateTopology(const YAML::Node& yaml_node)
{
    return std::make_unique<llangardaix::CPUTopology>(yaml_node);
}

