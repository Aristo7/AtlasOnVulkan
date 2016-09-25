#include "Precompiled.h"
#include "VulkanSurface.h"
#include "VulkanWindow.h"
#include "VulkanInstance.h"
#include "VulkanLogicalDevice.h"

using namespace vulkan;

class VulkanSurface::impl
{
public:
	impl(std::shared_ptr<VulkanInstance> instance, std::shared_ptr<VulkanWindow> window) : Instance(instance), Window(window)
	{
		// Construct the surface description:
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.pNext = nullptr;
		createInfo.hinstance = Window->getInstanceHandle();
		createInfo.hwnd = Window->setWindowHandle();;
		auto res = vkCreateWin32SurfaceKHR(Instance->getInstance(), &createInfo, nullptr, &Surface);
		assert(res == VK_SUCCESS);
	}

	~impl() {}

	VkWin32SurfaceCreateInfoKHR createInfo = {};
	VkSurfaceKHR Surface;

	std::shared_ptr<VulkanInstance> Instance;
	std::shared_ptr<VulkanWindow> Window;
};

VulkanSurface::VulkanSurface(std::shared_ptr<VulkanInstance> instance, std::shared_ptr<VulkanWindow> window)
{
	pimpl = std::make_unique<impl>(instance, window);
}

VulkanSurface::~VulkanSurface()
{
}

VkSurfaceKHR VulkanSurface::getSurface() const
{
	return pimpl->Surface;
}

VkSurfaceCapabilitiesKHR VulkanSurface::getSurfaceCapabilitiesKHR(std::shared_ptr<VulkanLogicalDevice> gpu) const
{
	VkSurfaceCapabilitiesKHR catabilitiesKHR;

	auto res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu->getPhysicalDevice(), pimpl->Surface,
		&catabilitiesKHR);
	assert(res == VK_SUCCESS);

	return catabilitiesKHR;
}

std::vector<VkPresentModeKHR> VulkanSurface::getPresentModesKHR(std::shared_ptr<VulkanLogicalDevice> gpu) const
{
	uint32_t presentModeCount;
	auto res = vkGetPhysicalDeviceSurfacePresentModesKHR(gpu->getPhysicalDevice(), pimpl->Surface, &presentModeCount, nullptr);
	assert(res == VK_SUCCESS);

	std::vector<VkPresentModeKHR> modesKHR(presentModeCount);

	res = vkGetPhysicalDeviceSurfacePresentModesKHR(gpu->getPhysicalDevice(), pimpl->Surface, &presentModeCount, modesKHR.data());
	assert(res == VK_SUCCESS);

	return modesKHR;
}
