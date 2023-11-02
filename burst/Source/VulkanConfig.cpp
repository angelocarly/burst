#include "burst/VulkanConfig.h"

// ============================================= VulkanConfigBuilder ===================================================

burst::VulkanConfig
burst::VulkanConfigBuilder::Build() const
{
    return mConfig;
}

burst::VulkanConfigBuilder &
burst::VulkanConfigBuilder::SetInstanceExtensions( std::vector< const char * > inInstanceExtensions )
{
    mConfig.mInstanceExtensions = inInstanceExtensions;
    return * this;
}
