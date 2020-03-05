#pragma once

#include <sstream>
#include <string>
#include <windows.h>
#include <cstdarg>

//#ifdef __linux__
//
//#include <unistd.h>
//
//#elif _WIN32
//
//#include <windows.h>
//
//#endif

class Subprocess final
{
public:

	std::stringstream arguments;

	STARTUPINFO startupInfo{};
	PROCESS_INFORMATION processInfo{};

	char** argv{};

	explicit Subprocess(const std::string& processName);
	void SetArrayArgs(const std::initializer_list<std::string>& args);

	template <typename ...Args, typename T>
	void SetArgs(const T& head, const Args&... args)
	{
		arguments << head << " ";
		return SetArgs(args...);
	}

	
	void CreateSubprocess(bool waiting = true);
	~Subprocess();
private:
	
	static void SetArgs()
	{
	}

	static std::string GetErrorDescription();
};
