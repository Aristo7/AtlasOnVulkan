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

	AutoModule VulkanLibrary;
	std::string AppName{ "Atlas On Vulkan" };

	unique_ptr<VulkanInstance> Instance;
	vector<std::shared_ptr<VulkanPhysicalDevice>> PhysicalDevices;
	unique_ptr<VulkanDevice> LogicalDevice;
};

void VulkanApplication::impl::init()
{
	Instance = make_unique<VulkanInstance>(AppName);

	PhysicalDevices = Instance->enumeratePhysicalDevices();
	log() << "GPUs= " << PhysicalDevices.size() << std::endl;

	LogicalDevice = make_unique<VulkanDevice>(PhysicalDevices[0]);
}

void VulkanApplication::impl::shutdown()
{
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
