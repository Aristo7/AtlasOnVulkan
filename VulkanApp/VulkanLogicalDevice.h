#pragma once
#include "VulkanPhysicalDevice.h"

namespace vulkan
{
	class VulkanLogicalDevice
	{
	public:
		explicit VulkanLogicalDevice(std::shared_ptr<VulkanPhysicalDevice> device);
		~VulkanLogicalDevice();

		void connectWithSurface(std::shared_ptr<VulkanSurface> surface);
		VkFormat findSupportedImageFormat(std::shared_ptr<VulkanSurface> surface);

		VkDevice getLogicalDevice() const;
		VkPhysicalDevice getPhysicalDevice() const;

	private:
		class impl; std::unique_ptr<impl> pimpl;
	};	
}
