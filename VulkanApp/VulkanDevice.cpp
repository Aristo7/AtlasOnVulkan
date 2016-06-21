#include "Precompiled.h"
#include "VulkanDevice.h"

using namespace vulkan;

class VulkanDevice::impl
{
public:
	impl() {}
	~impl()
	{
		vkDestroyDevice(device, nullptr);
	}

	VkDevice device;
	std::shared_ptr<VulkanPhysicalDevice> Device;
};

VulkanDevice::VulkanDevice(std::shared_ptr<VulkanPhysicalDevice> device, VkDeviceCreateInfo& info)
{
}

VulkanDevice::~VulkanDevice()
{
	
}
