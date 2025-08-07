
#pragma once

#include <string>
#include <map>
#include <memory>

#include "sparta/simulation/ResourceFactory.hpp"
#include "sparta/utils/SpartaException.hpp"

namespace llangardaix {

class CPUFactories
{
public:
    // constructor - registers all known factories
    CPUFactories();
    // desctructor
    ~CPUFactories();

    // retrieves a ResourceFactoryBase pointer by its string name
    // - factory_name : the name of the factory to retrieve
    sparta::ResourceFactoryBase* getFactory(const std::string& factory_name) const;

    // get the global unary instance of CPUFactories
    // this makes it easy to access the factories from anywhere in the code
    static CPUFactories* getGlobalInstance();

private:
    // map to store registered factories by name
    // using unique_ptr for automatic memory management of factory instances
    std::map<std::string, std::unique_ptr<sparta::ResourceFactoryBase>> factories_;

    // static member to hold the single global instance
    static std::unique_ptr<CPUFactories> global_instance_;
};


} // namespace llangardaix
