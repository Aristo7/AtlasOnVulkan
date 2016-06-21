#include "Precompiled.h"
#include "VulkanApplication.h"
#include "AutoModule.h"
#include <cassert>
#include <vector>

#include "VLog.h"
#include "VulkanInstance.h"
#include "VulkanDevice.h"
#include "VulkanWindow.h"

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

	unique_ptr<VulkanWindow> Window;
};

void VulkanApplication::impl::init()
{
	Instance = make_unique<VulkanInstance>(AppName);

	PhysicalDevices = Instance->enumeratePhysicalDevices();
	assert(PhysicalDevices.size() > 0);
	log() << "GPUs= " << PhysicalDevices.size() << std::endl;

	LogicalDevice = make_unique<VulkanDevice>(PhysicalDevices[0]);

	Window = make_unique<VulkanWindow>(AppName);
}

void VulkanApplication::impl::shutdown()
{
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
