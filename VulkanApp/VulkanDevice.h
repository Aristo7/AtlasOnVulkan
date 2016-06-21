#pragma once
#include "VulkanPhysicalDevice.h"

namespace vulkan
{
	class VulkanDevice
	{
	public:
		VulkanDevice(std::shared_ptr<VulkanPhysicalDevice> device, VkDeviceCreateInfo& info);
		~VulkanDevice();

	private:
		class impl; std::unique_ptr<impl> pimpl;
	};	
}
