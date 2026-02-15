#pragma once
#include<memory>
#include "Core/PlatformDetection.h"
//#ifdef VIR_PLATFORM_WINDOWS
//#if VIR_DYNAMIC_LINK
//	#ifdef VIR_BUILD_DLL
//		#define VIR_API  _declspec(dllexport)
//	#else
//		#define VIR_API _declspec(dllimport)
//	#endif
//#else 
//#define VIR_API
//#endif
//#else
//	#error only support Windows!
//#endif


#ifdef VIR_DEBUG
	#define VIR_ENABLE_ASSERTS
#endif

#ifdef VIR_ENABLE_ASSERTS
#define VIR_ASSERT(x,...) {if(!(x)) {VIR_ERROR("Assertion Failec:{0}",__VA_ARGS__);__debugbreak();}}
#define VIR_CORE_ASSERT(x,...) {if(!(x)) {VIR_CORE_ERROR("Assertion Failec:{0}",__VA_ARGS__);__debugbreak();}}
#else 
#define VIR_ASSERT(x,...)
#define VIR_CORE_ASSERT(x,...)
#endif

#define BIT(x) (1<<x)

#define VIR_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Vireo {

	
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}


}
