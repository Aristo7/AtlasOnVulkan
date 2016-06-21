#include "Precompiled.h"
#include "VulkanApplication.h"
#include "AutoModule.h"
#include <cassert>
#include <vector>

#include "VLog.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"

using namespace vulkan;
using std::make_unique;
using std::vector;
using std::string;
using std::unique_ptr;

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

	void create_device();

	AutoModule VulkanLibrary;
	std::string AppName{ "Atlas On Vulkan" };
	unique_ptr<VulkanInstance> Instance;

	vector<std::shared_ptr<VulkanPhysicalDevice>> Devices;

	VkDeviceQueueCreateInfo queue_info = {};

	VkDeviceCreateInfo device_info = {};

	unique_ptr<VulkanDevice> Device;
};

void VulkanApplication::impl::init()
{
	Instance = make_unique<VulkanInstance>(AppName);

	Devices = Instance->enumeratePhysicalDevices();
	log() << "GPUs= " << Devices.size() << std::endl;	

	create_device();
}

void VulkanApplication::impl::shutdown()
{
	Device.reset();
	Devices.clear();
	Instance.reset();
}

void VulkanApplication::impl::create_device()
{
	/* VULKAN_KEY_START */
	auto queue_props = Devices[0]->getQueueFamilyProperties();

	auto found = false;
	for (unsigned int i = 0; i < queue_props.size(); i++)
	{
		if (queue_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
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

	Device = make_unique<VulkanDevice>(Devices[0], device_info);
}

VulkanApplication::VulkanApplication()
{
	pimpl = std::make_unique<impl>();
}

VulkanApplication::~VulkanApplication()
{
	// empty
}
