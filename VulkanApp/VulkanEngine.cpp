#include "Precompiled.h"
#include "VulkanEngine.h"
#include "AutoModule.h"
#include <cassert>
#include <string>
#include <iostream>
#include <vector>

#include "VLog.h"
#include "VException.h"

class VulkanEngine::impl
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

	void create_instance();

	AutoModule VulkanLibrary;
	std::string AppName{ "Atlas On Vulkan" };

	VkApplicationInfo app_info = {};
	VkInstanceCreateInfo inst_info = {};

	VkInstance inst;
	std::vector<VkPhysicalDevice> Devices;
};

void VulkanEngine::impl::init()
{
	create_instance();

	/* VULKAN_KEY_START */

	uint32_t gpu_count = 1;
	auto res = vkEnumeratePhysicalDevices(inst, &gpu_count, nullptr);
	assert(gpu_count);
	log() << "GPUs= " << gpu_count << std::endl;

	Devices.resize(gpu_count);

	res = vkEnumeratePhysicalDevices(inst, &gpu_count, Devices.data());
	assert(!res && gpu_count >= 1);

	/* VULKAN_KEY_END */

}

void VulkanEngine::impl::shutdown()
{
	vkDestroyInstance(inst, nullptr);
}

void VulkanEngine::impl::create_instance()
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

VulkanEngine::VulkanEngine()
{
	pimpl = std::make_unique<impl>();
}

VulkanEngine::~VulkanEngine()
{
	// empty
}
