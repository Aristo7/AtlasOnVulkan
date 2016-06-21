#include "Precompiled.h"
#include "VulkanApplication.h"
#include "AutoModule.h"
#include <cassert>
#include <vector>

#include "VLog.h"
#include "VException.h"

using namespace vulkan;

class VulkanApplication::impl
{
public:
	impl()
	{
		init();
	}

	~impl()
	{
		destroy_device();
		shutdown();
	}

private:
	void init();
	void shutdown();

	void create_instance();
	void enumerate_devices();
	void create_device();
	void destroy_device();

	AutoModule VulkanLibrary;
	std::string AppName{ "Atlas On Vulkan" };

	VkApplicationInfo app_info = {};
	VkInstanceCreateInfo inst_info = {};

	VkInstance inst;
	std::vector<VkPhysicalDevice> Devices;

	VkDeviceQueueCreateInfo queue_info = {};
	std::vector<VkQueueFamilyProperties> QueueProperties;

	VkDeviceCreateInfo device_info = {};

	VkDevice device;
};

void VulkanApplication::impl::init()
{
	create_instance();
	enumerate_devices();
	create_device();
}

void VulkanApplication::impl::shutdown()
{
	vkDestroyInstance(inst, nullptr);
}

void VulkanApplication::impl::create_instance()
{
	// initialize the VkApplicationInfo structure
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = nullptr;
	app_info.pApplicationName = AppName.c_str();
	app_info.applicationVersion = 1;
	app_info.pEngineName = AppName.c_str();
	app_info.engineVersion = 1;
	app_info.apiVersion = VK_API_VERSION_1_0;

	// initialize the VkInstanceCreateInfo structure	
	inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_info.pNext = nullptr;
	inst_info.flags = 0;
	inst_info.pApplicationInfo = &app_info;
	inst_info.enabledExtensionCount = 0;
	inst_info.ppEnabledExtensionNames = nullptr;
	inst_info.enabledLayerCount = 0;
	inst_info.ppEnabledLayerNames = nullptr;

	auto res = vkCreateInstance(&inst_info, nullptr, &inst);
	switch (res)
	{
	case VK_SUCCESS:
	{
		break;
	}
	case VK_ERROR_INCOMPATIBLE_DRIVER:
	{
		throw VException("cannot find a compatible Vulkan ICD");
	}
	default:
	{
		throw VException("unknown error");
	}
	}
}

void VulkanApplication::impl::enumerate_devices()
{
	/* VULKAN_KEY_START */

	uint32_t gpu_count = 1;
	auto res = vkEnumeratePhysicalDevices(inst, &gpu_count, nullptr);
	assert(!res && gpu_count);
	log() << "GPUs= " << gpu_count << std::endl;

	Devices.resize(gpu_count);

	res = vkEnumeratePhysicalDevices(inst, &gpu_count, Devices.data());
	assert(!res && gpu_count >= 1);

	/* VULKAN_KEY_END */
}

void VulkanApplication::impl::create_device()
{
	/* VULKAN_KEY_START */
	uint32_t count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(Devices[0], &count, nullptr);
	assert(count >= 1);

	QueueProperties.resize(count);
	vkGetPhysicalDeviceQueueFamilyProperties(Devices[0], &count, QueueProperties.data());
	assert(count >= 1);

	auto found = false;
	for (unsigned int i = 0; i < QueueProperties.size(); i++)
	{
		if (QueueProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			queue_info.queueFamilyIndex = i;
			found = true;
			log() << "Found gpu " << i << " with queue with VK_QUEUE_GRAPHICS_BIT\n";
			break;
		}
	}
	assert(found);

	float queue_priorities[1] = { 0.0 };
	queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_info.pNext = nullptr;
	queue_info.queueCount = 1;
	queue_info.pQueuePriorities = queue_priorities;

	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pNext = nullptr;
	device_info.queueCreateInfoCount = 1;
	device_info.pQueueCreateInfos = &queue_info;
	device_info.enabledExtensionCount = 0;
	device_info.ppEnabledExtensionNames = nullptr;
	device_info.enabledLayerCount = 0;
	device_info.ppEnabledLayerNames = nullptr;
	device_info.pEnabledFeatures = nullptr;

	auto res = vkCreateDevice(Devices[0], &device_info, nullptr, &device);
	assert(res == VK_SUCCESS);
}

void VulkanApplication::impl::destroy_device()
{
	vkDestroyDevice(device, nullptr);

	/* VULKAN_KEY_END */
}

VulkanApplication::VulkanApplication()
{
	pimpl = std::make_unique<impl>();
}

VulkanApplication::~VulkanApplication()
{
	// empty
}
