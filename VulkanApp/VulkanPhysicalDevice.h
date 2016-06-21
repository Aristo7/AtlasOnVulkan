#pragma once
#include "Precompiled.h"
#include <vector>

namespace vulkan
{
	class VulkanPhysicalDevice
	{
	public:
		explicit VulkanPhysicalDevice(const VkPhysicalDevice& device);
		~VulkanPhysicalDevice();

		std::vector<VkQueueFamilyProperties> getQueueFamilyProperties();

	private:
		class impl; std::unique_ptr<impl> pimpl;
	};	
}
