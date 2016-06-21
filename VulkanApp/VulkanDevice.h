#pragma once
#include "VulkanPhysicalDevice.h"

namespace vulkan
{
	class VulkanDevice
	{
	public:
		explicit VulkanDevice(std::shared_ptr<VulkanPhysicalDevice> device);
		~VulkanDevice();

	private:
		class impl; std::unique_ptr<impl> pimpl;
	};	
}
