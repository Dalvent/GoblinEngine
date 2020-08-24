#pragma once

#include "spdlog/spdlog.h"
#include "GoblinEngine/Core/Base.h"

namespace GoblinEngine
{
	class Log
	{
	public:
		static void Init();

		static Ref<spdlog::logger> GetCoreLogger() { return _coreLogger; }
		static Ref<spdlog::logger> GetClientLogger() { return _clientogger; }
	private:
		static Ref<spdlog::logger> _coreLogger;
		static Ref<spdlog::logger> _clientogger;
	};
}

// Core log macros.
#define GE_LOG_CORE_TRACE(...)  ::GoblinEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define GE_LOG_CORE_INFO(...)   ::GoblinEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define GE_LOG_CORE_WARN(...)   ::GoblinEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define GE_LOG_CORE_ERROR(...)  ::GoblinEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define GE_LOG_CORE_FATAL(...)  ::GoblinEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)
// Client log macros.
#define GE_LOG_TRACE(...)		::GoblinEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define GE_LOG_INFO(...)		::GoblinEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define GE_LOG_WARN(...)		::GoblinEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define GE_LOG_ERROR(...)		::GoblinEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define GE_LOG_FATAL(...)		::GoblinEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)