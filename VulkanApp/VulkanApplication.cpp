#include "Precompiled.h"
#include "VulkanApplication.h"
#include "AutoModule.h"
#include <cassert>
#include <vector>

#include "VLog.h"
#include "VulkanInstance.h"
#include "VulkanLogicalDevice.h"
#include "VulkanWindow.h"
#include "VulkanSurface.h"

using namespace vulkan;
using std::make_shared;
using std::vector;
using std::string;
using std::unique_ptr;
using std::shared_ptr;

class VulkanApplication::impl
{
public:
	impl()
	{
		init();
	}

	~impl()
	{
		shutdown();
	}

private:
	void init();
	void shutdown();

	AutoModule VulkanLibrary;
	std::string AppName{ "Atlas On Vulkan" };

	shared_ptr<VulkanInstance> Instance;
	vector<std::shared_ptr<VulkanPhysicalDevice>> PhysicalDevices;
	shared_ptr<VulkanLogicalDevice> LogicalDevice;

	shared_ptr<VulkanWindow> Window;
	shared_ptr<VulkanSurface> Surface;
};

void VulkanApplication::impl::init()
{
	Instance = make_shared<VulkanInstance>(AppName);

	PhysicalDevices = Instance->enumeratePhysicalDevices();
	assert(PhysicalDevices.size() > 0);
	log() << "GPUs= " << PhysicalDevices.size() << std::endl;

	LogicalDevice = make_shared<VulkanLogicalDevice>(PhysicalDevices[0]);

	Window = make_shared<VulkanWindow>(AppName);

	/* VULKAN_KEY_START */
	// Construct the surface description:
	Surface = make_shared<VulkanSurface>(Instance, Window);

	LogicalDevice->connectWithSurface(Surface);

	//init_device(info);

	// Get the list of VkFormats that are supported:

	auto supported_format = LogicalDevice->findSupportedImageFormat(Surface);

	VkSurfaceCapabilitiesKHR surfCapabilities;
	surfCapabilities = Surface->getSurfaceCapabilitiesKHR(LogicalDevice);

	auto modesKHR = Surface->getPresentModesKHR(LogicalDevice);

	VkExtent2D swapChainExtent;
	// width and height are either both -1, or both not -1.
	if (surfCapabilities.currentExtent.width == static_cast<uint32_t>(-1)) 
	{
		// If the surface size is undefined, the size is set to
		// the size of the images requested.
		swapChainExtent.width = Window->getWidth();
		swapChainExtent.height = Window->getHeight();
	}
	else 
	{
		// If the surface size is defined, the swap chain size must match
		swapChainExtent = surfCapabilities.currentExtent;
	}

	// If mailbox mode is available, use it, as is the lowest-latency non-
	// tearing mode.  If not, try IMMEDIATE which will usually be available,
	// and is fastest (though it tears).  If not, fall back to FIFO which is
	// always available.
	auto swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
	for(auto mode : modesKHR)
	{
		if (mode == VK_PRESENT_MODE_MAILBOX_KHR) 
		{
			swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
		if ((swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR) &&
			(mode == VK_PRESENT_MODE_IMMEDIATE_KHR)) 
		{
			swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		}
	}

	// Determine the number of VkImage's to use in the swap chain (we desire to
	// own only 1 image at a time, besides the images being displayed and
	// queued for display):
	uint32_t desiredNumberOfSwapChainImages =
		surfCapabilities.minImageCount + 1;
	if ((surfCapabilities.maxImageCount > 0) &&
		(desiredNumberOfSwapChainImages > surfCapabilities.maxImageCount)) {
		// Application must settle for fewer images than desired:
		desiredNumberOfSwapChainImages = surfCapabilities.maxImageCount;
	}

	VkSurfaceTransformFlagBitsKHR preTransform;
	if (surfCapabilities.supportedTransforms &
		VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
		preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else {
		preTransform = surfCapabilities.currentTransform;
	}

	VkSwapchainCreateInfoKHR swap_chain = {};
	swap_chain.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swap_chain.pNext = nullptr;
	swap_chain.surface = Surface->getSurface();
	swap_chain.minImageCount = desiredNumberOfSwapChainImages;
	swap_chain.imageFormat = supported_format;
	swap_chain.imageExtent.width = swapChainExtent.width;
	swap_chain.imageExtent.height = swapChainExtent.height;
	swap_chain.preTransform = preTransform;
	swap_chain.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swap_chain.imageArrayLayers = 1;
	swap_chain.presentMode = swapchainPresentMode;
	swap_chain.oldSwapchain = VK_NULL_HANDLE;
	swap_chain.clipped = true;
	swap_chain.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	swap_chain.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swap_chain.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swap_chain.queueFamilyIndexCount = 0;
	swap_chain.pQueueFamilyIndices = nullptr;

	auto res = vkCreateSwapchainKHR(info.device, &swap_chain, nullptr, &info.swap_chain);
	assert(res == VK_SUCCESS);

	res = vkGetSwapchainImagesKHR(info.device, info.swap_chain,
		&info.swapchainImageCount, nullptr);
	assert(res == VK_SUCCESS);
	VkImage *swapchainImages =
		(VkImage *)malloc(info.swapchainImageCount * sizeof(VkImage));
	assert(swapchainImages);
	res = vkGetSwapchainImagesKHR(info.device, info.swap_chain,
		&info.swapchainImageCount, swapchainImages);
	assert(res == VK_SUCCESS);

	info.buffers.resize(info.swapchainImageCount);

	// Going to need a command buffer to send the memory barriers in
	// set_image_layout but we couldn't have created one before we knew
	// what our graphics_queue_family_index is, but now that we have it,
	// create the command buffer

	// TODO

	/* VULKAN_KEY_END */
}

void VulkanApplication::impl::shutdown()
{
	/* Clean Up */
	VkCommandBuffer cmd_bufs[1] = { info.cmd };
	vkFreeCommandBuffers(info.device, info.cmd_pool, 1, cmd_bufs);
	vkDestroyCommandPool(info.device, info.cmd_pool, nullptr);
	for (uint32_t i = 0; i < info.swapchainImageCount; i++) {
		vkDestroyImageView(info.device, info.buffers[i].view, nullptr);
	}
	vkDestroySwapchainKHR(info.device, info.swap_chain, nullptr);

	Window.reset();
	LogicalDevice.reset();
	PhysicalDevices.clear();
	Instance.reset();
}

VulkanApplication::VulkanApplication()
{
	pimpl = std::make_unique<impl>();
}

VulkanApplication::~VulkanApplication()
{
	// empty
}
