#pragma once
#include"Core.h"

#include<spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Vireo {

	class  Logger {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

//core log
#define VIR_CORE_TRACE(...)	::Vireo::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define VIR_CORE_INFO(...)	::Vireo::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define VIR_CORE_WARN(...)	::Vireo::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define VIR_CORE_ERROR(...) ::Vireo::Logger::GetCoreLogger()->error(__VA_ARGS__)

//
#define VIR_TRACE(...)		::Vireo::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define VIR_INFO(...)		::Vireo::Logger::GetClientLogger()->info(__VA_ARGS__)
#define VIR_WARN(...)		::Vireo::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define VIR_ERROR(...)		::Vireo::Logger::GetClientLogger()->error(__VA_ARGS__)