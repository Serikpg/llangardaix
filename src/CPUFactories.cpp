
#include "CPUFactories.hpp"

#include "Core.hpp"

namespace llangardaix {

std::unique_ptr<CPUFactories> CPUFactories::global_instance_ = nullptr;

// Constructor
CPUFactories::CPUFactories()
{
    factories_["core"] = std::make_unique<sparta::ResourceFactory<Core, Core::CoreParameterSet>>();
}

// Destructor
CPUFactories::~CPUFactories() = default;

/* Retrieves a ResourceFactoryBase pointer by name
 * - factory_name : string representing the name of the factory desired
 * -> returns a raw pointer to the reqquested ResourceFactoryBase or nullptr if not found
 */
sparta::ResourceFactoryBase* CPUFactories::getFactory(const std::string& factory_name) const
{
    auto it = factories_.find(factory_name);
    if (it != factories_.end()) {
        return it->second.get();
    }
    return nullptr;
}

/* Provides access to the global singleton instance of CPUFactory
 * ensures than only one intance of CPUFactory exists
 * -> returns a raw pointer to the global CPUFactories instance
 */
CPUFactories* CPUFactories::getGlobalInstance()
{
    if(!global_instance_) {
        global_instance_ = std::make_unique<CPUFactories>();
    }
    return global_instance_.get();
}

} // namespace llangardaix
