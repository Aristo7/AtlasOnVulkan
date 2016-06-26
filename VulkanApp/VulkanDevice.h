#pragma once
#include "VulkanPhysicalDevice.h"

namespace vulkan
{
	class VulkanDevice
	{
	public:
		explicit VulkanDevice(std::shared_ptr<VulkanPhysicalDevice> device);
		~VulkanDevice();

		void connectWithSurface(std::shared_ptr<VulkanSurface> surface);

	private:
		class impl; std::unique_ptr<impl> pimpl;
	};	
}
