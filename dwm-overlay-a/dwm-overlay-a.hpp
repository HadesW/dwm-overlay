#pragma once
#include "msgs.hpp"
#include "hooks.hpp"

unsigned int __stdcall main_thread(void*)
{
	//
	if (!msg->initializer())
	{
		return EXIT_FAILURE;
	}

	//
	if (!hooks::dwmhook())
	{
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}