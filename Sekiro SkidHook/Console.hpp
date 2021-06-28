#pragma once
#include "pch.h"

namespace SkidBase::Console
{

	enum class ConsoleColors : std::uint16_t {
		BLACK = 0,
		DARKBLUE = FOREGROUND_BLUE,
		DARKGREEN = FOREGROUND_GREEN,
		DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
		DARKRED = FOREGROUND_RED,
		DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
		DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
		DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		GRAY = FOREGROUND_INTENSITY,
		BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
		MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	};

	class ConsoleMgr
	{

	public:

		void Attach()
		{

			const auto IsConsoleAllocated = AttachConsole(GetCurrentProcessId());

			if (IsConsoleAllocated == false)
				AllocConsole();

			ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

			if (ConsoleHandle != nullptr)
			{
				SetConsoleTitleA("Space Engine");
				freopen("CONOUT$", "w", stdout);
				freopen("CONIN$", "r", stdin);
			}

			char DocumentsFolderPath[MAX_PATH];
			const auto result =
				SHGetFolderPathA(nullptr, CSIDL_PERSONAL, nullptr, SHGFP_TYPE_CURRENT, DocumentsFolderPath);

			if (SUCCEEDED(result))
			{

				std::string LogFilePath = DocumentsFolderPath;
				LogFilePath += "\\";
				LogFilePath += MENU_NAME;
				LogFilePath += "\\";
				LogFilePath += MENU_NAME;
				LogFilePath += ".log";

				LogInfo("Creating log file on {}", LogFilePath.c_str());
				FileOutStream.open(LogFilePath.c_str(), std::ofstream::out | std::ofstream::trunc);

				if (!FileOutStream.is_open())
					LogError("Cant create log file!");
			}

		}
		void Detach()
		{
			if (ConsoleHandle != nullptr) {

				FreeConsole();

				fclose(stdout);
				fclose(stdin);

				FileOutStream.close();
			}
		}

	public:

		void SetCurrentConsoleColor(ConsoleColors Color)
		{
			SetConsoleTextAttribute(ConsoleHandle, static_cast<std::uint16_t>(Color));
		}

		template <typename ...Args>
		void Log(ConsoleColors PrefixColor, std::string_view Prefix, std::string_view Format, Args... args)
		{

			auto Message = fmt::format(Format, args...);

			SetCurrentConsoleColor(PrefixColor);
			std::cout << "| Space Engine |" << "|" << Prefix << "|";

			SetCurrentConsoleColor(ConsoleColors::WHITE); /* text color*/
			std::cout << Message << "\n" << std::flush;

			FileOutStream << "|" << Prefix << "|" << Message << "\n" << std::flush;
		}

		template <typename ...Args>
		void LogInfo(std::string_view Format, Args... args)
		{
			Log(ConsoleColors::GRAY, "INFO", Format, args...);
		}

		template <typename ...Args>
		void LogScript(std::string_view Format, Args... args)
		{
			Log(ConsoleColors::GREEN, "ScriptMgr", Format, args...);
		}

		template <typename ...Args>
		void LogError(std::string_view Format, Args... args)
		{
			Log(ConsoleColors::RED, "ERROR", Format, args...);
		}

		template <typename ...Args>
		void LogPattern(std::string_view Format, Args... args)
		{
			Log(ConsoleColors::CYAN, "PATTERN SCANNER", Format, args...);
		}

		template <typename ...Args>
		void LogHook(std::string_view Format, Args... args)
		{
			Log(ConsoleColors::BLUE, "HOOKING", Format, args...);
		}

	private:

		HANDLE        ConsoleHandle;

		std::ofstream FileOutStream;


	}; 


}