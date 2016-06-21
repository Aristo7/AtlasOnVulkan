#pragma once

#include "Precompiled.h"

class AutoModule
{
public:
	AutoModule() : Module(nullptr) {}
	explicit AutoModule(HMODULE module) : Module(module) {}
	~AutoModule()
	{
		if (Module)
			FreeLibrary(Module);
	}

	void reset(HMODULE hinstance)
	{
		if (Module)
			FreeLibrary(Module);

		Module = hinstance;
	}

	AutoModule(AutoModule& other) = delete;

	auto get() const { return Module; }

private:
	HMODULE Module;
};
