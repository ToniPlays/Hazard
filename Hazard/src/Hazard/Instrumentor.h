#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>

#include "Logging/Logger.h"
#include "Hazard/Utils/File.h"

namespace Hazard {

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string Name;

		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};
	struct InstrumentationSession
	{
		std::string sessionName;
	};
	class Instrumentor {
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string& name, const std::filesystem::path& filePath) {
			std::lock_guard lock(m_Mutex);
			
			if (m_CurrentSession) {
				
				if (Logging::Logger::GetCoreLogger()) 
				{
					std::cout << "Instrumentor::BeginSession(" << name << ") when session '" << m_CurrentSession->sessionName << "' already open." << std::endl;
				}
				EndSessionInternal();
			}
			m_OutputStream.open(filePath);

			if (!File::DirectoryExists(filePath.string())) 
				File::CreateDir(filePath.parent_path().string());

			if (m_OutputStream.is_open()) {
				m_CurrentSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else if(Logging::Logger::GetCoreLogger()) {
				std::cout << "Instrumentor could not open results file " << filePath << std::endl;
			}
		}
		void EndSession() {
			std::lock_guard lock(m_Mutex);
			EndSessionInternal();
		}
		void WriteProfile(const ProfileResult& result) {
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}
		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:

		Instrumentor() : m_CurrentSession(nullptr) {}

		~Instrumentor()
		{
			EndSession();
		}
		void WriteHeader() {
			m_OutputStream << "{\"otherData\": {}, \"traceEvents\": [{}";
			m_OutputStream.flush();
		}
		void WriteFooter() {
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}
		void EndSessionInternal() {
			if (!m_CurrentSession) return;

			WriteFooter();
			m_OutputStream.close();

			delete m_CurrentSession;
			m_CurrentSession = nullptr;
		}

	private:
		std::mutex m_Mutex;
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
	};

	class InstrumentationTimer {
	public:
		InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}
		~InstrumentationTimer() {
			if (!m_Stopped) {
				Stop();
			}
		}
		void Stop() {

			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};
	namespace InstrumentorUtils {

		template<size_t N>
		struct ChangeResult {
			char Data[N];
		};

		template<size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K]) {
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;

			while (srcIndex < N) {
				size_t matchIndex = 0;

				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex]) {
					matchIndex++;
				}
				if (matchIndex == K - 1) srcIndex += matchIndex;

				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\"' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}
