#pragma once
#include <memory>

class VulkanEngine
{
public:
	VulkanEngine();
	~VulkanEngine();

private:
	class impl; std::unique_ptr<impl> pimpl;
};

