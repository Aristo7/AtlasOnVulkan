#pragma once
#include <string>
#include <memory>
#include <vector>
#include "VulkanPhysicalDevice.h"

namespace vulkan
{
	class VulkanInstance
	{
	public:
		explicit VulkanInstance(const std::string& app_name);
		~VulkanInstance();

		std::vector<std::shared_ptr<VulkanPhysicalDevice>> enumeratePhysicalDevices();
	private:
		class impl; std::unique_ptr<impl> pimpl;
	};	
}
