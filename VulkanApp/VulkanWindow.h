#pragma once
#include <string>

namespace vulkan
{
	class VulkanWindow
	{
	public:
		explicit VulkanWindow(const std::string& window_name);
		~VulkanWindow();
		HINSTANCE getInstanceHandle() const;
		HWND setWindowHandle() const;

	private:
		class impl; std::unique_ptr<impl> pimpl;
	};	
}
