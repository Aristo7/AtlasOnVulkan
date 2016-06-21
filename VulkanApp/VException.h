#pragma once

#include "VLog.h"
#include <exception>
#include <string>

namespace vulkan
{
	class VException : public std::exception
	{
	public:
		explicit VException(const std::string& message) : Message(message)
		{
			log() << message << std::endl;
		}
		~VException() {}

		char const* what() const override
		{
			return Message.c_str();
		}

	private:
		std::string Message;
	};
}
