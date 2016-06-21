#include "Precompiled.h"
#include "VulkanPhysicalDevice.h"
#include <cassert>

using namespace vulkan;

class VulkanPhysicalDevice::impl
{
public:
	explicit impl(const VkPhysicalDevice& device) : Device(device) {}
	~impl() {}

	VkPhysicalDevice Device;
	std::vector<VkQueueFamilyProperties> QueueProperties;

	std::vector<VkQueueFamilyProperties> getQueueFamilyProperties()
	{
		uint32_t count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(Device, &count, nullptr);
		assert(count >= 1);

		QueueProperties.resize(count);
		vkGetPhysicalDeviceQueueFamilyProperties(Device, &count, QueueProperties.data());
		assert(count >= 1);

		return QueueProperties;
	}
};

VulkanPhysicalDevice::VulkanPhysicalDevice(const VkPhysicalDevice& device)
{
	pimpl = std::make_unique<impl>(device);
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
	// empty
}

std::vector<VkQueueFamilyProperties> VulkanPhysicalDevice::getQueueFamilyProperties()
{
	return pimpl->getQueueFamilyProperties();
}

VkPhysicalDevice VulkanPhysicalDevice::getPhysicalDevice() const
{
	return pimpl->Device;
}
