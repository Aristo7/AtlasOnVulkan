// VulkanApp.cpp : Defines the entry point for the console application.
//

#include "Precompiled.h"
#include "VulkanApplication.h"
#include <ostream>
#include <iostream>
#include "VException.h"
#include "VLog.h"

int main()
{
	try
	{
		vulkan::VulkanApplication root;
	}
	catch (vulkan::VException& ve)
	{
		vulkan::log() << "Hit exception: " << ve.what() << std::endl;
	}

	std::cout << "Finished running. Hit ENTER to quit..." << std::endl;

	getchar();
    return 0;
}
