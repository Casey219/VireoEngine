#pragma once
#include "Application.h"
#include "Logger.h"

#ifdef VIR_PLATFORM_WINDOWS

extern Vireo::Application* Vireo::CreateApplication();

int main(int argc,char** argv) {

	Vireo::Logger::Init();
	VIR_CORE_WARN("Initialized engine");
	int a = 6;
	VIR_INFO("HELLO,{0}", a);

	auto app = Vireo::CreateApplication();
	app->Run();
	delete app;
}

#endif