#include "Precompiled.h"
#include "VulkanDevice.h"
#include <cassert>
#include "VLog.h"

using namespace vulkan;

class VulkanDevice::impl
{
public:
	impl(std::shared_ptr<VulkanPhysicalDevice> device) : PhysicalDevice(device)
	{
		auto queue_props = PhysicalDevice->getQueueFamilyProperties();

		auto found = false;
		for (unsigned int i = 0; i < queue_props.size(); i++)
		{
			if (queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				queue_info.queueFamilyIndex = i;
				found = true;
				log() << "Found gpu " << i << " with queue with VK_QUEUE_GRAPHICS_BIT\n";
				break;
			}
		}
		assert(found);

		float queue_priorities[1] = { 0.0 };
		queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info.pNext = nullptr;
		queue_info.queueCount = 1;
		queue_info.pQueuePriorities = queue_priorities;

		device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_info.pNext = nullptr;
		device_info.queueCreateInfoCount = 1;
		device_info.pQueueCreateInfos = &queue_info;
		device_info.enabledExtensionCount = 0;
		device_info.ppEnabledExtensionNames = nullptr;
		device_info.enabledLayerCount = 0;
		device_info.ppEnabledLayerNames = nullptr;
		device_info.pEnabledFeatures = nullptr;

		auto res = vkCreateDevice(PhysicalDevice->getPhysicalDevice(), &device_info, NULL, &LogicalDevice);
		assert(res == VK_SUCCESS);

		log() << "Created a logical device\n";
	}

	~impl()
	{
		vkDestroyDevice(LogicalDevice, nullptr);
	}

	VkDevice LogicalDevice;
	std::shared_ptr<VulkanPhysicalDevice> PhysicalDevice;

	VkDeviceQueueCreateInfo queue_info = {};
	VkDeviceCreateInfo device_info = {};
};

VulkanDevice::VulkanDevice(std::shared_ptr<VulkanPhysicalDevice> device)
{
	pimpl = std::make_unique<impl>(device);
}

VulkanDevice::~VulkanDevice()
{
	// empty
}
