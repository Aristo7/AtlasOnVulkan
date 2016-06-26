#include "Precompiled.h"
#include "VulkanSurface.h"
#include "VulkanWindow.h"
#include "VulkanInstance.h"

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
