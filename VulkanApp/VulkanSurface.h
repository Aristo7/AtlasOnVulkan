#pragma once
#include <memory>

namespace vulkan
{
	class VulkanInstance;
	class VulkanWindow;

	class VulkanSurface
	{
	public:
		VulkanSurface(std::shared_ptr<VulkanInstance> device, std::shared_ptr<VulkanWindow> window);
		~VulkanSurface();

		VkSurfaceKHR getSurface() const;

	private:
		class impl; std::unique_ptr<impl> pimpl;
	};	
}
