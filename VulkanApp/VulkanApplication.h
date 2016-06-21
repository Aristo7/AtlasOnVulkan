#pragma once
#include <memory>

namespace vulkan
{
	class VulkanApplication
	{
	public:
		VulkanApplication();
		~VulkanApplication();

	private:
		class impl; std::unique_ptr<impl> pimpl;
	};
}
