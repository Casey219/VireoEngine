#include"Core.h"
#include <memory>
#include<spdlog/spdlog.h>

namespace Vireo {

	class VIR_API Logger {
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
#define VIR_CORE_Trace(...)	::Vireo::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define VIR_CORE_INFO(...)	::Vireo::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define VIR_CORE_WARN(...)	::Vireo::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define VIR_CORE_ERROR(...) ::Vireo::Logger::GetCoreLogger()->error(__VA_ARGS__)

//
#define VIR_Trace(...)		::Vireo::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define VIR_INFO(...)		::Vireo::Logger::GetClientLogger()->info(__VA_ARGS__)
#define VIR_WARN(...)		::Vireo::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define VIR_ERROR(...)		::Vireo::Logger::GetClientLogger()->error(__VA_ARGS__)