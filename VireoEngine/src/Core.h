#pragma once

#ifdef VIR_PLATFORM_WINDOWS
	#ifdef VIR_BUILD_DLL
		#define VIR_API _declspec(dllexport)
	#else
		#define VIR_API _declspec(dllimport)
	#endif
#else
	#error only support Windows!
#endif