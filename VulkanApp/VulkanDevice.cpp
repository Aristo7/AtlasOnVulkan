#include "Precompiled.h"
#include "VulkanDevice.h"
#include <cassert>
#include "VLog.h"
#include "VulkanSurface.h"

using namespace vulkan;

class VulkanDevice::impl
{
public:
	explicit impl(std::shared_ptr<VulkanPhysicalDevice> device) : PhysicalDevice(device)
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

		auto res = vkCreateDevice(PhysicalDevice->getPhysicalDevice(), &device_info, nullptr, &LogicalDevice);
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

	unsigned int graphics_queue_family_index = UINT32_MAX;

	void connectWithSurface(std::shared_ptr<VulkanSurface> surface)
	{
		// Iterate over each queue to learn whether it supports presenting:
		std::vector<VkBool32> supportsPresent(queue_info.queueCount);

		for (uint32_t i = 0; i < queue_info.queueCount; i++) 
		{
			vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevice->getPhysicalDevice(), i, surface->getSurface(), &supportsPresent[i]);
		}

		// Search for a graphics queue and a present queue in the array of queue
		// families, try to find one that supports both
		auto graphicsQueueNodeIndex = UINT32_MAX;
		auto queue_props = PhysicalDevice->getQueueFamilyProperties();
		for (uint32_t i = 0; i < queue_info.queueCount; i++) {
			if ((queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
				if (supportsPresent[i] == VK_TRUE) {
					graphicsQueueNodeIndex = i;
					break;
				}
			}
		}

		// Generate error if could not find a queue that supports both a graphics
		// and present
		if (graphicsQueueNodeIndex == UINT32_MAX) {
			vulkan::log() << "Could not find a queue that supports both graphics and "
				"present\n";
			exit(-1);
		}

		graphics_queue_family_index = graphicsQueueNodeIndex;
	}
};

VulkanDevice::VulkanDevice(std::shared_ptr<VulkanPhysicalDevice> device)
{
	pimpl = std::make_unique<impl>(device);
}

VulkanDevice::~VulkanDevice()
{
	// empty
}

void VulkanDevice::connectWithSurface(std::shared_ptr<VulkanSurface> surface)
{
	pimpl->connectWithSurface(surface);
}
