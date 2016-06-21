#pragma once
#include "Precompiled.h"
#include <vector>

namespace vulkan
{
	class VulkanPhysicalDevice
	{
		friend class VulkanDevice;
	public:
		explicit VulkanPhysicalDevice(const VkPhysicalDevice& device);
		~VulkanPhysicalDevice();

		std::vector<VkQueueFamilyProperties> getQueueFamilyProperties();

	private:
		VkPhysicalDevice getPhysicalDevice() const;

		class impl; std::unique_ptr<impl> pimpl;
	};	
}
