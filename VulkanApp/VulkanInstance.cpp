#include "Precompiled.h"
#include "VulkanInstance.h"
#include "VException.h"
#include <cassert>
#include "VulkanPhysicalDevice.h"

using namespace vulkan;

class VulkanInstance::impl
{
public:
	explicit impl(const std::string& app_name) : app_name(app_name) {}
	~impl()
	{
		vkDestroyInstance(inst, nullptr);
	}

	VkApplicationInfo app_info = {};
	VkInstanceCreateInfo inst_info = {};
	VkInstance inst = {};
	std::string app_name;

	void create()
	{
		// initialize the VkApplicationInfo structure
		app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.pNext = nullptr;
		app_info.pApplicationName = app_name.c_str();
		app_info.applicationVersion = 1;
		app_info.pEngineName = app_name.c_str();
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

	auto enumeratePhysicalDevices()
	{
		std::vector<VkPhysicalDevice> devices;

		uint32_t gpu_count = 1;
		auto res = vkEnumeratePhysicalDevices(inst, &gpu_count, nullptr);
		assert(!res && gpu_count);
		log() << "GPUs= " << gpu_count << std::endl;

		devices.resize(gpu_count);

		res = vkEnumeratePhysicalDevices(inst, &gpu_count, devices.data());
		assert(!res && gpu_count >= 1);

		std::vector<std::shared_ptr<VulkanPhysicalDevice>> actual_devices;
		for (auto& device : devices)
			actual_devices.push_back(std::make_unique<VulkanPhysicalDevice>(device));

		return actual_devices;
	}
};

VulkanInstance::VulkanInstance(const std::string& app_name)
{
	pimpl = std::make_unique<impl>(app_name);

	pimpl->create();
}

VulkanInstance::~VulkanInstance()
{
}

std::vector<std::shared_ptr<VulkanPhysicalDevice>> VulkanInstance::enumeratePhysicalDevices()
{
	return pimpl->enumeratePhysicalDevices();
}

VkInstance VulkanInstance::getInstance() const
{
	return pimpl->inst;
}
