#include "Precompiled.h"
#include "VulkanEngine.h"
#include "AutoModule.h"
#include <cassert>
#include <string>
#include <iostream>

class VulkanEngine::impl
{
public:
	impl() {}
	~impl() {}

	void init();

	AutoModule VulkanLibrary;
	std::string AppName{ "Atlas On Vulkan" };
};

void VulkanEngine::impl::init()
{
	/* VULKAN_KEY_START */

	// initialize the VkApplicationInfo structure
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = nullptr;
	app_info.pApplicationName = AppName.c_str();
	app_info.applicationVersion = 1;
	app_info.pEngineName = AppName.c_str();
	app_info.engineVersion = 1;
	app_info.apiVersion = VK_API_VERSION_1_0;

	// initialize the VkInstanceCreateInfo structure
	VkInstanceCreateInfo inst_info = {};
	inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_info.pNext = nullptr;
	inst_info.flags = 0;
	inst_info.pApplicationInfo = &app_info;
	inst_info.enabledExtensionCount = 0;
	inst_info.ppEnabledExtensionNames = nullptr;
	inst_info.enabledLayerCount = 0;
	inst_info.ppEnabledLayerNames = nullptr;

	VkInstance inst;
	VkResult res;

	res = vkCreateInstance(&inst_info, nullptr, &inst);
	if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
		std::cout << "cannot find a compatible Vulkan ICD\n";
		exit(-1);
	}
	else if (res) {
		std::cout << "unknown error\n";
		exit(-1);
	}

	vkDestroyInstance(inst, nullptr);

	/* VULKAN_KEY_END */
}

VulkanEngine::VulkanEngine()
{
	pimpl = std::make_unique<impl>();
	pimpl->init();
}

VulkanEngine::~VulkanEngine()
{
}
