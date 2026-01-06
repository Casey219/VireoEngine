#pragma once

#ifdef VIR_PLATFORM_WINDOWS

extern Vireo::Application* Vireo::CreateApplication();

int main(int argc,char** argv) {
	auto app = Vireo::CreateApplication();
	app->Run();
	delete app;
}

#endif