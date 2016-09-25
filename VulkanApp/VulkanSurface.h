#pragma once
#include <memory>
#include <vector>

namespace vulkan
{
	class VulkanInstance;
	class VulkanWindow;
	class VulkanLogicalDevice;

	class VulkanSurface
	{
	public:
		VulkanSurface(std::shared_ptr<VulkanInstance> device, std::shared_ptr<VulkanWindow> window);
		~VulkanSurface();

		VkSurfaceKHR getSurface() const;

		VkSurfaceCapabilitiesKHR getSurfaceCapabilitiesKHR(std::shared_ptr<VulkanLogicalDevice> gpu) const;

		std::vector<VkPresentModeKHR> getPresentModesKHR(std::shared_ptr<VulkanLogicalDevice> gpu) const;

	private:
		class impl; std::unique_ptr<impl> pimpl;
	};	
}
