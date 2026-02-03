#pragma once
#include "Application.h"
#include "Logger.h"
#include"Debug/Instrumentor.h"

#ifdef VIR_PLATFORM_WINDOWS

extern Vireo::Application* Vireo::CreateApplication();

int main(int argc,char** argv) {

	Vireo::Logger::Init();
	
	VIR_PROFILE_BEGIN_SESSION("Startup", "VireoProfile-Startup.json");
	auto app = Vireo::CreateApplication();
	VIR_PROFILE_END_SESSION();

	VIR_PROFILE_BEGIN_SESSION("Runtime", "VireoProfile-Runtime.json");
	app->Run();
	VIR_PROFILE_END_SESSION();

	VIR_PROFILE_BEGIN_SESSION("Startup", "VireoProfile-Shutdown.json");
	delete app;
	VIR_PROFILE_END_SESSION();
}

#endif