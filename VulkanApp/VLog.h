#pragma once
#include <string>
#include <iostream>

namespace vulkan
{
	inline auto& log()
	{
		return std::cout;
	}
}
