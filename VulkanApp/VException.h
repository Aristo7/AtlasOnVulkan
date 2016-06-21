#pragma once
#include <exception>
#include <string>

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
